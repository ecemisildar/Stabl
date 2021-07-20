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

int main(void) {

  halInit();
  chSysInit();
  eflStart(&EFLD1, NULL);
  sdStart(&SD2, NULL);
  BaseSequentialStream *stream = (BaseSequentialStream *)&SD2;

  mfsStart(&mfs1, &mfscfg1);
  mfs_error_t err;
  size_t size;

  uint8_t buffer[3];
  uint8_t buffer_new[3];

  for(int i=0; i<3; i++){
   buffer[i] = sdGet(&SD2);
   chprintf(stream, "%c", buffer[i]);
  }

    err = mfsWriteRecord(&mfs1, 1, sizeof buffer, buffer);
    if(err != MFS_NO_ERROR){chprintf(stream, "error creating record");}

    size = sizeof buffer_new;

    err = mfsReadRecord(&mfs1, 1, &size, buffer_new);
    if(err != MFS_NO_ERROR){chprintf(stream, "record not found");}
    if(size != sizeof buffer){chprintf(stream, "unexpected record length");}
    if(memcmp(buffer, buffer_new, size) != 0){chprintf(stream, "wrong record content");}


  /*
  err = mfsWriteRecord(&mfs1, 1, sizeof mfs_pattern16, mfs_pattern16);
  if(err != MFS_NO_ERROR){chprintf(stream, "error creating record 1");}
  err = mfsReadRecord(&mfs1, 1, &size, mfs_buffer);
  if(err != MFS_NO_ERROR){chprintf(stream, "record not found");}
  if(size != sizeof mfs_pattern16){chprintf(stream, "unexpected record length");}
  if(memcmp(mfs_pattern16, mfs_buffer, size) != 0){chprintf(stream, "wrong record content");}*/

  chprintf(stream, "buffer new: ");
  for(int i=0; i<3; i++){
  chprintf(stream, "%c",buffer_new[i]);
  }
  while (true) {
    if (!palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD2, &mfs_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}


