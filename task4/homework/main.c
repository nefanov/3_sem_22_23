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

    printf("POINTS Integral %d\n", points);
    printf("xmax:%lf xmin:%lf ymax:%lf ymin:%lf\n", xmax, xmin, ymax, ymin);
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

    pthread_mutex_lock(&mutex);

    int points = integralMonteCarlo(xmax, ymax, xmin, ymin, NPOINTS/(PTHREAD_SQR * PTHREAD_SQR), integralData->func);

    
    P += points;

    pthread_mutex_unlock(&mutex);
}

void calculate(double(*func)(double), int xmax, int ymax, int xmin, int ymin)
{
    pthread_mutex_init(&mutex, NULL);
    
    double stepScale = (1.0/PTHREAD_SQR);
    double square = (xmax-xmin) * (ymax-ymin);
    double xStep  = (xmax-xmin) * stepScale;
    double yStep  = (ymax-ymin) * stepScale;
    double curX   = 0;
    double curY   = 0;

    struct PthreadData integralData[PTHREAD_SQR * PTHREAD_SQR];

    pthread_t tid[PTHREAD_SQR * PTHREAD_SQR];
    $
    for (int i = 0; i < PTHREAD_SQR; i++)
    {   
        curY = 0;
        $
        for (int j = 0; j < PTHREAD_SQR; j++)
        {
            printf("pos:%d\n", i*PTHREAD_SQR + j);
            integralData[i*PTHREAD_SQR + j].xmin = curX;
            integralData[i*PTHREAD_SQR + j].ymin = curY;
            integralData[i*PTHREAD_SQR + j].xmax = curX + xStep;
            curY += yStep;
            integralData[i*PTHREAD_SQR + j].ymax = curY;
            $
            pthread_create(tid + i*PTHREAD_SQR + j, NULL, pthreadCalcStep, (void*)(integralData + i*PTHREAD_SQR + j));
            $
        }
        curX += xStep;
    }
    $

    // for (int i = 0; i < (PTHREAD_SQR * PTHREAD_SQR); i++)
    // {
    //     integralData[i].xmin = curX;
    //     integralData[i].ymin = curY;
    //     curX += xStep;
    //     curY += yStep;
    //     integralData[i].xmax = curX;
    //     integralData[i].ymax = curY;
    //     integralData[i].func = func;
        
        
    //     pthread_create(tid + i, NULL, pthreadCalcStep,(void*)(integralData + i));
    // }

    for(int i = 0; i < (PTHREAD_SQR * PTHREAD_SQR); i++)
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