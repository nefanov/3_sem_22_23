#ifndef _INTEGRAL_H_
#define _INTEGRAL_H_

#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "CommonFunctions.hpp"

const int NUM_POINTS = 100000000;

struct interval {
    double left = 0.0;
    double right = 0.0;
};

struct point {
    double x = 0.0;
    double y = 0.0;
};

struct data {
    int numPoints = 0; 
    interval range = {};
};

point CreatePoint(const interval range, unsigned int *seed);

bool IsUnder(const point p);

double CalcFuncValue(const double x);

void *CalcIntegrate(void *varss);

#endif