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

void check_record(uint8_t a[],size_t len_a, uint8_t b[],size_t len_b);
void write_buffer(uint8_t b[],size_t len_a);
void read_buffer(uint8_t b[],size_t len_b);


int main(void) {

  halInit();
  chSysInit();
  eflStart(&EFLD1, NULL);
  sdStart(&SD2, NULL);
  mfsStart(&mfs1, &mfscfg1);

  size_t size_buffer;
  size_t size_buffer_new;
  uint8_t buffer[3] = "";
  uint8_t buffer_new[3] = "";

  size_buffer = sizeof buffer;
  size_buffer_new = sizeof buffer_new;

  check_record(buffer, size_buffer, buffer_new, size_buffer_new); //check if there is an existing record
}


/******************FUNCTIONS******************/

void check_record(uint8_t *a,size_t len_a, uint8_t *b,size_t len_b){
  mfs_error_t err;
  err = mfsReadRecord(&mfs1, 3, &len_a, a); //check there is a record
    if(err != MFS_ERR_NOT_FOUND){
      chprintf(stream, "Record was already present\n");
      chprintf(stream, "Record: ");
      for(int i=0; i<3; i++){
         chprintf(stream, "%c", a[i]);// print existing record
         }
        chprintf(stream, "\n");

      chprintf(stream, "Do you want to erase this record? Y for yes, N for no\n");
      if (sdGet(&SD2) == 'Y') {
        err = mfsErase(&mfs1); //erase old record
        if(err != MFS_NO_ERROR){chprintf(stream, "Storage erase error\n");}
        write_buffer(a, len_a); //take input data from user and write it into memory
        read_buffer(b, len_b); //read data from the memory
      }
    }
    else{ // if there is no record
      write_buffer(a, len_a); //take input data from user and write it into memory
      read_buffer(b, len_b); //read data from the memory
    }
}

void write_buffer(uint8_t *a, size_t len_a){
  mfs_error_t err;
  chprintf(stream, "Enter the number: "); // request a new record
  for(int i=0; i<3; i++){
   a[i] = sdGet(&SD2);
   chprintf(stream, "%c", a[i]); // print buffer
   }
  chprintf(stream, "\n");
  err = mfsWriteRecord(&mfs1, 3, len_a, a); // write into buffer
  if(err != MFS_NO_ERROR){chprintf(stream, "error creating record\n");}
}


void read_buffer(uint8_t *b, size_t len_b){
  mfs_error_t err;
  err = mfsReadRecord(&mfs1, 3, &len_b, b); // read and take it into buffer_new
    if(err != MFS_NO_ERROR){chprintf(stream, "record not found\n");}
    else{
      chprintf(stream, "New buffer: ");
      for(int i=0; i<3; i++){
       chprintf(stream, "%c", b[i]); // print new buffer
       }
      chprintf(stream, "\n");
  }
}






