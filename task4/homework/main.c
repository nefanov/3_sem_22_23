#include "integral.h"

#define $ fprintf(stderr, "HERE %d\n", __LINE__); 

// y = x^2

int P = 0;

double func(double x)
{
    return x*x;
}

short isEqual(double x, double y)
{
    if (abs(x-y) < EPS) return 1;

    return 0;
}

void calculate(double(*func)(double), int xmax, int ymax)
{
    double xStep = xmax * STEPSCALE;
    double yStep = ymax * STEPSCALE;
    double curX  = 0;
    double curY  = 0;

    while ((int)(curY) != ymax)
    {
        while((int)(curX) != xmax)
        {
            curX += xStep;
        }

        curY += yStep;
    }
}


int main()
{
    double tmp = 1.43;
    double tmp1 = 2.00;

    // printf("trunc: %d and %d\n", (int)trunc(tmp), (int)trunc(tmp1));
    printf("round: %d and %d\n", (int)tmp, (int)tmp1);
    calculate(func, XMAX, YMAX);

    return 0;
}