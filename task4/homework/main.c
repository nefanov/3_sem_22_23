#include "integral.h"

#define $ fprintf(stderr, "HERE %d\n", __LINE__); 


extern int P;
// y = x^2

double func(double x)
{
    return x*x;
}

short isEqual(double x, double y)
{
    if (abs(x-y) < EPS) return 1;

    return 0;
}

int integralMonteCarlo(int xmax, int ymax, int xmin, int ymin, int count, double(*func)(double))
{
    unsigned int seed = time(NULL);

    double x = 0, y = 0;
    int points = 0;
     
    for (int i = 0; i < count; i++)
    {

        x = xmin + (double)(rand_r(&seed) * (xmax - xmin)/(double)RAND_MAX);
        y = ymin + (double)(rand_r(&seed) * (ymax - ymin)/(double)RAND_MAX);

        if ( y <= func(x)) points++;
    }
    return points;
}

void* pthreadCalcStep(void* data)
{
    if (!data)
    {
        fprintf(stderr, "ERR:THREAD DATA HAS BEEN LOST\n");
        return NULL;
    }

    struct PthreadData* integralData = (struct PthreadData*)data;

    double xmax = integralData->xmax;
    double ymax = integralData->ymax;
    double xmin = integralData->xmin;
    double ymin = integralData->ymin;

    int points = integralMonteCarlo(xmax, ymax, xmin, ymin, NPOINTS/PTHREAD_MAX, integralData->func);

    pthread_mutex_lock(&mutex);
    
    P += points; 
    pthread_mutex_unlock(&mutex);
}

void calculate(double(*func)(double), int xmax, int ymax, int xmin, int ymin)
{
    pthread_mutex_init(&mutex, NULL);
    
    double stepScale = (1.0/PTHREAD_MAX);
    double square = (xmax-xmin) * (ymax - ymin);
    double xStep  = (xmax-xmin) * stepScale;
    double yStep  = (ymax-ymin) * stepScale;
    double curX   = 0;
    double curY   = 0;

    struct PthreadData integralData[PTHREAD_MAX];

    pthread_t tid[PTHREAD_MAX];

    for (int i = 0; i < PTHREAD_MAX; i++)
    {
        integralData[i].xmin = curX;
        integralData[i].ymin = curY;
        curX += xStep;
        curY += yStep;
        integralData[i].xmax = curX;
        integralData[i].ymax = curY;
        integralData[i].func = func;

        pthread_create(tid + i, NULL, pthreadCalcStep,(void*)(integralData + i));
    }
    
    for(int i = 0; i < PTHREAD_MAX; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    printAnswer(square, xmax, xmin);
}

void printAnswer(double square, int xmax, int xmin)
{
    if (P == 0)
    {
        fprintf(stderr, "ERR: No selected points (P = 0)\n");
        return;
    }
    fprintf(stdout, "Integral from xmin = %d to xmax = %d\nValue:%lf\n", xmin, xmax, square*P/NPOINTS);
}

int main()
{
    calculate(func, XMAX, YMAX, XMIN, YMIN);

    return 0;
}