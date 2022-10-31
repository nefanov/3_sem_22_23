#ifndef HEADER_HPP
#define HEADER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


typedef struct Args {

    double min_x = 0;
    double max_x = 0;

    double min_y = 0;
    double max_y = 0;

    double (*func) (double) = nullptr;
} Args;


const int SQR_ROOT_THREAD_NUM   = 4;
const int THREAD_NUM    = SQR_ROOT_THREAD_NUM * SQR_ROOT_THREAD_NUM;
const int POINT_NUM     = 5000000;
const double EPS        = 0.0001;

//HARDCODE HERE!!!!
const double MIN_X = 0;
const double MAX_X = 1;
const double MIN_Y = 0;
const double MAX_Y = 1;



int   find_local_hits (Args *args);
void *find_all_hits   (void *args);

bool   equal(double x, double y);
double func (double x);
size_t hash_crc_32 (void *ptr, size_t len);

#endif