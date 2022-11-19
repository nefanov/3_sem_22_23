#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int fifo_snd(const char* fname);
int fifo_rcv(const char* fname);