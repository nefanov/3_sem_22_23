#pragma once



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>

#ifndef BUF_SIZE
  #define BUF_SIZE 1024  
#endif

typedef struct {
  char mdata[BUF_SIZE];
  size_t sent;
} buffer_t;
typedef struct {
  long mtype;
  buffer_t buffer;
} message;

int msq_snd(const char* fname);
int msq_rcv(const char* fname);