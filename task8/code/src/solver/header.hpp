#ifndef HEADER_HPP
#define HEADER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "../../include/query.hpp"

typedef struct Args {

    double min_x = 0;
    double max_x = 0;

    double min_y = 0;
    double max_y = 0;

    double (*func) (double) = nullptr;
} Args;


const int POINT_NUM     = 5000000;
const double EPS        = 0.0001;


int   find_local_hits (Args *args);
void *find_all_hits   (void *args);

bool   equal(double x, double y);
double func (double x);
size_t hash_crc_32 (void *ptr, size_t len);


double count_monte (int pthread_count, double x_min, double x_max,
                    double y_min, double y_max);
#endif