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
BaseSequentialStream *stream = (BaseSequentialStream *)&SD2;




void print_buffer(uint8_t b[]);
void print_newBuffer(uint8_t b[]);
void error_check(uint8_t *a, uint8_t *b);

int main(void) {

  halInit();
  chSysInit();
  eflStart(&EFLD1, NULL);
  sdStart(&SD2, NULL);
  mfsStart(&mfs1, &mfscfg1);

  mfs_error_t err;
  size_t size;
  uint8_t buffer[3] = "";
  uint8_t buffer_new[3] = "";

  size = sizeof buffer_new;

  err = mfsReadRecord(&mfs1, 3, &size, buffer); //check there is a record
  if(err != MFS_ERR_NOT_FOUND){
    chprintf(stream, "Record was already present\n");
    chprintf(stream, "Record: %s\n",buffer);// print existing record

    chprintf(stream, "Do you want to erase this record? Y for yes, N for no\n");
    if (sdGet(&SD2) == 'Y') {
      err = mfsErase(&mfs1);
      if(err != MFS_NO_ERROR){chprintf(stream, "Storage erase error\n");}
      print_buffer(buffer);
      error_check(buffer, buffer_new);
    }
  }
  else{ // if there is no record
    print_buffer(buffer);
    error_check(buffer, buffer_new);
  }
}


/******************FUNCTIONS******************/

void print_buffer(uint8_t *b){
  chprintf(stream, "Enter the number: "); // request a new record
  for(int i=0; i<3; i++){
   b[i] = sdGet(&SD2);
   chprintf(stream, "%c", b[i]); // print buffer
   }
  chprintf(stream, "\n");
}
void print_newBuffer(uint8_t *b){
  chprintf(stream, "New buffer: ");
  for(int i=0; i<3; i++){
   chprintf(stream, "%c", b[i]); // print new buffer
   }
  chprintf(stream, "\n");
}

void error_check(uint8_t *a, uint8_t *b){
  mfs_error_t err;
    err = mfsWriteRecord(&mfs1, 3, sizeof a, a); // write into buffer
    if(err != MFS_NO_ERROR){chprintf(stream, "error creating record\n");}
    err = mfsReadRecord(&mfs1, 3, sizeof b, b); // read and take it into buffer_new
    if(err != MFS_NO_ERROR){chprintf(stream, "record not found\n");}
    if(sizeof b != sizeof a){chprintf(stream, "unexpected record length\n ");}
    if(memcmp(a, b, sizeof b) != 0){chprintf(stream, "wrong record content\n");} // compare contents
    else
      print_newBuffer(b);
}







