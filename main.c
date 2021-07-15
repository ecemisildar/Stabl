#include "ch.h"
#include "hal.h"
#include "hal_mfs.h"
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

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  while (true) {
    if (!palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD2, &mfs_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}


