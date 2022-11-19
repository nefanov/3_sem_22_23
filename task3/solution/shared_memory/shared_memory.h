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


#ifndef BUF_SIZE
  #define BUF_SIZE 1024  
#endif

typedef enum {
  WRITE = 0,
  READ = 1,
  MUTEX = 2,
} sem_name;
void* get_mem(const char* path, size_t buf_size, int* shm_d);
int get_shm(key_t key, size_t segment_size, int flags);
void lock_sem(int sem_d, sem_name semnum);
void unlock_sem(int sem_d, sem_name semnum);
int shared_mem_snd(const char* fname);
int shared_mem_rcv(const char* fname);
void init_sems(int sem_d, int semval_1, int semval_2, int semval_3);
int sem_set(int key);
void lock_rdy(int sem_d);
void unlock_rdy(int sem_d);