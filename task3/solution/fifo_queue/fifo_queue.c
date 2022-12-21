#include "fifo_queue.h"

const char* key_name = "/home/ilya/Документы/GitHub/3_sem_22_23/task3/solution/fifo_queue.fifo";

int fifo_snd(const char* fname) {
  int read = 0;
  /*if (unlink(key_name) == -1) {
    perror("[sender] unlink");
  }*/
  if (mkfifo(key_name, 0666) == -1) {
    perror("[sender]mkfifo");
    exit(1);
  }
  int fifo_d = open(key_name, O_WRONLY);
  if (fifo_d == -1) {
    perror("[sender]fifo open");
    exit(1);
  }
  
  FILE* file_d = fopen(fname, "r");
  assert(file_d != NULL);
  
  char* buffer = (char*) calloc(BUF_SIZE, sizeof(char));
  assert(buffer != NULL);
  while ((read = fread(buffer, sizeof(char), BUF_SIZE, file_d)) > 0) {
    write(fifo_d, buffer, read);
  }
  free(buffer);
  fclose(file_d);
  close(fifo_d);
  return 0;
}

int fifo_rcv(const char* fname) {
  int write = 0;
  int fifo_d = open(key_name, O_RDONLY);
  if (fifo_d == -1) {
    perror("[reciever]fifo open");
    exit(1);
  }
  
  FILE* file_d = fopen(fname, "w");
  assert(file_d != NULL);
  
  char* buffer = (char*) calloc(BUF_SIZE, sizeof(char));
  assert(buffer != NULL);

  while ((write = read(fifo_d, buffer, BUF_SIZE)) > 0) {
    fwrite(buffer, sizeof(char), write, file_d);
  }
  free(buffer);
  fclose(file_d);
  close(fifo_d);
  unlink(key_name);
  return 0;
}