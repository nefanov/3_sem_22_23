#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>


#define PTHREAD_SQR 2
#define NPOINTS 100000000
#define YMAX 8886110
#define XMAX 4
#define YMIN 0
#define XMIN -1

#define EPS 0.000001
//-----------------------------------------
struct PthreadData
{
    double xmin;
    double ymin;

    double xmax;
    double ymax;

    double(*func)(double);
};


pthread_mutex_t mutex;

static int P = 0;

//------------------------------------------


int integralMonteCarlo(double xmax,double ymax,double xmin,double ymin,int count,double(*func)(double));

short isEqual(double x, double y);

void calculate(double(*func)(double), int xmax, int ymax, int xmin, int ymin);

void* pthreadCalcStep(void* data);

void printAnswer(double square, int xmax, int xmin);


#endif