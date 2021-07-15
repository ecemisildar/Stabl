#include "ch.h"
#include "hal.h"
#include "hal_mfs.h"
#include "chprintf.h"
#include "mfs_test_root.h"

const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&EFLD1,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = 4096U,
  .bank0_start      = 128U,
  .bank0_sectors    = 2U,
  .bank1_start      = 130U,
  .bank1_sectors    = 2U
};

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

int main(void) {

  halInit();
  chSysInit();
  eflStart(&EFLD1, NULL);
  sdStart(&SD2, NULL);
  BaseSequentialStream *stream = (BaseSequentialStream *)&SD2;
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  mfsStart(&mfs1, &mfscfg1);
  mfs_error_t err;
  size_t size;

      size = sizeof mfs_buffer;
  err = mfsWriteRecord(&mfs1, 1, sizeof mfs_pattern16, mfs_pattern16);
  if(err != MFS_NO_ERROR){chprintf(stream, "error creating record 1");}
  err = mfsReadRecord(&mfs1, 1, &size, mfs_buffer);
  if(err != MFS_NO_ERROR){chprintf(stream, "record not found");}
  if(size != sizeof mfs_pattern16){chprintf(stream, "unexpected record length");}
  if(memcmp(mfs_pattern16, mfs_buffer, size) != 0){chprintf(stream, "wrong record content");}
  for(int i=0; i<16; i++){
  chprintf(stream, "%d",mfs_buffer[i]);
  }
  while (true) {
    if (!palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD2, &mfs_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}


