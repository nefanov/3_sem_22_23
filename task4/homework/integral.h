#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define YMAX 225
#define XMAX 15
#define STEPSCALE 0.1
#define EPS 0.000001

double func(double x);

void calculate(double(*func)(double), int xmax, int ymax);

#endif