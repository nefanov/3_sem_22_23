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

const long POINTS_NUM = 100000000;
const int THREADS_NUM = 25;
const int SHM_SZ = 2;

const int KEY = 123456;

static pthread_mutex_t p_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Point_t 
{
    double x;
    double y;
} Point_t;

typedef struct Interval_t 
{
    double a;
    double b;
} Interval_t;

typedef struct Pthread_args_t 
{
    int *shm;
    long N;
    Interval_t interval;
} Arg;

double function(double x);
void* generate_points (void *args);
void* get_addr(size_t shm_sz);

void send    (int *shm, Interval_t interval);
void receive (int *shm, Interval_t interval);


