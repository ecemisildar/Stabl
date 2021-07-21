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
  size = sizeof buffer_new;

  err = mfsReadRecord(&mfs1, 3, &size, buffer); //check there is a record
  if(err != MFS_ERR_NOT_FOUND){

  chprintf(stream, "record was already present ");
  chprintf(stream, "Record: "); // print existing record
    for(int i=0; i<3; i++){
    chprintf(stream, "%c",buffer[i]);
    }

    chprintf(stream, "Do you want to erase this record? Y for yes, N for no");
    if (sdGet(&SD2) == 'Y') {

      err = mfsErase(&mfs1);
      if(err != MFS_NO_ERROR){chprintf(stream, "storage erase error");}
      /*err = mfsEraseRecord(&mfs1, 3);
      if(err != MFS_NO_ERROR){chprintf(stream, "error erasing the record");}
      err = mfsReadRecord(&mfs1, 3, &size, mfs_buffer);
      if(err != MFS_ERR_NOT_FOUND){chprintf(stream, "record not erased");}*/

      //test_execute((BaseSequentialStream *)&SD2, &mfs_test_suite);

      chprintf(stream, "Enter the number: "); // request a new record
        for(int i=0; i<3; i++){
         buffer[i] = sdGet(&SD2);
         chprintf(stream, "%c", buffer[i]); // print buffer
         }
          err = mfsWriteRecord(&mfs1, 3, sizeof buffer, buffer); // write into buffer
          if(err != MFS_NO_ERROR){chprintf(stream, "error creating record ");}



          err = mfsReadRecord(&mfs1, 3, &size, buffer_new); // read and take it into buffer_new
          if(err != MFS_NO_ERROR){chprintf(stream, "record not found ");}
          if(size != sizeof buffer){chprintf(stream, "unexpected record length ");}
          if(memcmp(buffer, buffer_new, size) != 0){chprintf(stream, "wrong record content ");} // compare contents

          else{
          chprintf(stream, "buffer new: "); // wenn alles okay, print new buffer
            for(int i=0; i<3; i++){
            chprintf(stream, "%c",buffer_new[i]);
            }
          }
    }

  }
  else{ // if there is no record
    chprintf(stream, "Enter the number: "); // request a new record
      for(int i=0; i<3; i++){
       buffer[i] = sdGet(&SD2);
       chprintf(stream, "%c", buffer[i]); // print buffer
      }
        err = mfsWriteRecord(&mfs1, 3, sizeof buffer, buffer); // write into buffer
        if(err != MFS_NO_ERROR){chprintf(stream, "error creating record ");}

        size = sizeof buffer_new;

        err = mfsReadRecord(&mfs1, 3, &size, buffer_new); // read and take it into buffer_new
        if(err != MFS_NO_ERROR){chprintf(stream, "record not found ");}
        if(size != sizeof buffer){chprintf(stream, "unexpected record length ");}
        if(memcmp(buffer, buffer_new, size) != 0){chprintf(stream, "wrong record content ");} // compare contents

        else{
        chprintf(stream, "buffer new: "); // wenn alles okay, print new buffer
          for(int i=0; i<3; i++){
          chprintf(stream, "%c",buffer_new[i]);
          }
        }
  }


 /*while (true) {
    if (!palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD2, &mfs_test_suite);
    }
    chThdSleepMilliseconds(500);
  }*/
}


