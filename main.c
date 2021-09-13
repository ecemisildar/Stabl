#include "ch.h"
#include "hal.h"
#include "hal_mfs.h"
#include "chprintf.h"
#include "mfs_test_root.h"

const MFSConfig mfscfg1 = {
    .flashp           = (BaseFlash *)&EFLD1,
    .erased           = 0xFFFFFFFFU,
    .bank_size        = 262144U,
    .bank0_start      = 9U,
    .bank0_sectors    = 1U,
    .bank1_start      = 10U,
    .bank1_sectors    = 1U
};
BaseSequentialStream *stream = (BaseSequentialStream *)&SD3;

void past_records(uint8_t *a,size_t len_a);
void memory(uint8_t *a,size_t len_a, uint8_t *b,size_t len_b);
void check_record(uint8_t id, uint8_t a[],size_t len_a, uint8_t b[],size_t len_b);
void write_buffer(uint8_t id, uint8_t b[],size_t len_a);
void read_buffer(uint8_t id, uint8_t b[],size_t len_b);

int main(void) {

  halInit();
  chSysInit();
  sdStart(&SD3, NULL);
  chprintf(stream, "Starting up \n");

  eflStart(&EFLD1, NULL);
  mfsObjectInit(&mfs1);
  mfsStart(&mfs1, &mfscfg1);

  size_t size_buffer;
  size_t size_buffer_new;
  uint8_t buffer[3] = "";
  uint8_t buffer_new[3] = "";

  size_buffer = sizeof buffer;
  size_buffer_new = sizeof buffer_new;

  //mfsErase(&mfs1);
  chprintf(stream, "Press P to see past records \n");
  if(sdGet(&SD3)=='P'){
      past_records(buffer, size_buffer);
  }

  memory(buffer, size_buffer, buffer_new, size_buffer_new);
}

/******************FUNCTIONS******************/

void past_records(uint8_t *a,size_t len_a){ //shows the past records in the memory

  mfs_error_t err;
  chprintf(stream, "Past Records \n");
  chprintf(stream, "------------ \n");
  for(int id=1; id<=MFS_CFG_MAX_RECORDS; id++){
    err = mfsReadRecord(&mfs1, id, &len_a, a); //check all IDs
    chprintf(stream, "ID %d : ", id);
    for(int j=0; j<3; j++){
      if(err == MFS_ERR_NOT_FOUND){chprintf(stream, " ");}
      else{chprintf(stream, "%c", a[j]);} // print existing records
    }
    chprintf(stream, "\n");
  }
  chprintf(stream, "------------ \n");
}

void memory(uint8_t *a, size_t len_a, uint8_t *b, size_t len_b){ //select which memory ID will be used

  uint8_t first;
  uint8_t second;
  uint8_t id;
  chprintf(stream, "Which memory ID do you want to write? (max 32) \n");

    first = sdGet(&SD3) - '0'; // first digit of the ID
    second = sdGet(&SD3) - '0'; // second digit of the ID
    if(first == 240){first = 0;} // if first digit is \n which makes 240 as char, we transform into 0
    id = 10 * first + second; // add numbers and find the ID
    if(id<33 && id>0){
    chprintf(stream, "ID: %d\n",id);
    check_record(id, a, len_a, b, len_b); //check if there is an existing record
    }
    else{
      chprintf(stream, "wrong ID\n");
      memory(a, len_a, b, len_b);
    }
}

void check_record(uint8_t id, uint8_t *a,size_t len_a, uint8_t *b,size_t len_b){

  mfs_error_t err;
  uint8_t token;
  err = mfsReadRecord(&mfs1, id, &len_a, a); //check there is a record
    if(err != MFS_ERR_NOT_FOUND){
      chprintf(stream, "Record was already present\n");
      chprintf(stream, "Record: ");
      for(int i=0; i<3; i++){
           chprintf(stream, "%c", a[i]);// print existing record
      }
      chprintf(stream, "\n");

      chprintf(stream, "Do you want to erase this record? Y for yes, N for no \n");
      token = sdGet(&SD3);
      if (token == 'Y') {
        err = mfsEraseRecord(&mfs1, id); //erase old record
        if(err != MFS_NO_ERROR){chprintf(stream, "Storage erase error\n");}
            chprintf(stream, "Do you want to enter a new value? Y for yes, N for no \n");
            if(sdGet(&SD3) == 'Y'){
                write_buffer(id, a, len_a); //take input data from user and write it into memory
                read_buffer(id, b, len_b); //read data from the memory
            }
            else{
                chprintf(stream, "ID %d erased", id);
            }
      }
      else if(token == 'N'){
          memory(a, len_a, b, len_b);
      }
     }
     else{ // if there is no record
        write_buffer(id, a, len_a); //take input data from user and write it into memory
        read_buffer(id, b, len_b); //read data from the memory
    }
}

void write_buffer(uint8_t id, uint8_t *a, size_t len_a){
  mfs_error_t err;
  chprintf(stream, "Enter the number: "); // request a new record
  for(int i=0; i<3; i++){
     a[i] = sdGet(&SD3);
     chprintf(stream, "%c", a[i]); // print buffer
  }

  chprintf(stream, "\n");
  err = mfsWriteRecord(&mfs1, id, len_a, a); // write into buffer
  if(err != MFS_NO_ERROR){chprintf(stream, "error creating record\n");}
}


void read_buffer(uint8_t id, uint8_t *b, size_t len_b){

  mfs_error_t err;
  err = mfsReadRecord(&mfs1, id, &len_b, b); // read and take it into buffer_new
    if(err != MFS_NO_ERROR){chprintf(stream, "record not found\n");}
    else{
      chprintf(stream, "New buffer: ");
      for(int i=0; i<3; i++){
          chprintf(stream, "%c", b[i]); // print new buffer
      }
      chprintf(stream, "\n");
  }
}
