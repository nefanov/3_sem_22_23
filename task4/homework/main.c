#include "integral.h"

#define $ fprintf(stderr, "HERE %d\n", __LINE__); 


extern int P;

double func(double x)
{
    return exp(x*x);
}

short isEqual(double x, double y)
{
    if (abs(x-y) < EPS) return 1;

    return 0;
}

int integralMonteCarlo(double xmax, double ymax, double xmin, double ymin, int count, double(*func)(double))
{
    unsigned int id = time(NULL) + pthread_self();
    srand(id);

    double x = 0, y = 0;
    int points = 0;
     
    for (int i = 0; i < count; i++)
    {
        x = (double)rand_r(&id) / RAND_MAX * (xmax - xmin) + xmin;  
        y = (double)rand_r(&id) / RAND_MAX * (ymax - ymin) + ymin;
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

    int points = integralMonteCarlo(xmax, ymax, xmin, ymin, NPOINTS/(PTHREAD_SQRT * PTHREAD_SQRT), integralData->func);

    pthread_mutex_lock(&mutex);

    P += points;

    pthread_mutex_unlock(&mutex);
}

void calculate(double(*func)(double), int xmax, int ymax, int xmin, int ymin)
{
    pthread_mutex_init(&mutex, NULL);
    
    double stepScale = (1.0/PTHREAD_SQRT);
    double square = (xmax-xmin) * (ymax-ymin);
    double xStep  = (xmax-xmin) * stepScale;
    double yStep  = (ymax-ymin) * stepScale;
    double curX   = xmin;
    double curY   = ymin;

    struct PthreadData integralData[PTHREAD_SQRT * PTHREAD_SQRT];

    pthread_t           tid[PTHREAD_SQRT * PTHREAD_SQRT];

    for (int i = 0; i < PTHREAD_SQRT; i++)
    {   
        curY = ymin;
        for (int j = 0; j < PTHREAD_SQRT; j++)
        {
            integralData[i*PTHREAD_SQRT + j].xmin = curX;
            integralData[i*PTHREAD_SQRT + j].ymin = curY;
            integralData[i*PTHREAD_SQRT + j].xmax = curX + xStep;
            curY += yStep;
            integralData[i*PTHREAD_SQRT + j].ymax = curY;
            integralData[i*PTHREAD_SQRT + j].func = func;
            pthread_create(tid + i*PTHREAD_SQRT + j, NULL, pthreadCalcStep, (void*)(integralData + i*PTHREAD_SQRT + j));
        }
        curX += xStep;
    }


    for(int i = 0; i < (PTHREAD_SQRT * PTHREAD_SQRT); i++)
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
    fprintf(stdout, "Integral from xmin = %d to xmax = %d\nValue:%lf\n", xmin, xmax, (square*P)/NPOINTS);
}

int main()
{
    calculate(func, XMAX, YMAX, XMIN, YMIN);

    return 0;
}

// pthread 1 = 2,301s pthread 4 = 0,609s pthread 2 = 1,139s pthread 3 = 0,980s pthread 9 = 0,610 etc
