#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define POINTS_NUM 100000000
#define THREADS_NUM 35
#define SHM_SZ 2

#define KEY 1337

typedef struct Point {
  double x;
  double y;
} Point;

typedef struct Interval {
  double a;
  double b;
} Interval;

typedef struct pthread_args {
  int *shm;
  long N;
  Interval Interval;
} Arg;