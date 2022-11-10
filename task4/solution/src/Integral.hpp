#ifndef _INTEGRAL_H_
#define _INTEGRAL_H_

#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "CommonFunctions.hpp"

const size_t NUM_POINTS = 1000000;

struct interval {
    double left;
    double right;
};

struct point {
    double x;
    double y;
};

struct data {
    interval range;
    size_t numPoints; 
};

// struct rectangle {
//     point leftPoint;
//     point rightPoint;
// };

point CreatePoint(const interval range);

bool IsUnder(const point p);

double CalcFuncValue(const double x);

void *CalcIntegrate(void *varss);

#endif