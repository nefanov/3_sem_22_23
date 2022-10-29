#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>


#define THREADS 2
#define DOMAIN_UP 1
#define DOMAIN_DOWN 0
#define RANGE_UP 1
#define RANGE_DOWN 0


double integral = 0;
pthread_t tid[THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


double func(double x);
void *counter(void *arg);
void add_in_integral(double integral_i);


double func(double x)
{
    return x * x * x;
}


void *counter(void *arg)
{
    double x1, y1, x2, y2, x, y, value;

    x1 = ((double*)arg)[0];
    x2 = ((double*)arg)[1];
    y1 = ((double*)arg)[2];
    y2 = ((double*)arg)[3];

    printf("Аргументы %f %f %f %f\n", x1, x2, y1, y2);
    
    double square = 0;

    int points = 100000; 
    
    srand((unsigned)time(NULL));

    for (int dot = 0; dot < points; dot++) 
    {
    	x = x1 + (x2 - x1) * ((double)rand()/RAND_MAX);
        y = y1 + (y2 - y1) * ((double)rand()/RAND_MAX);
    	
        value = func(x);

    	if ((value > 0 && y < value) || (value < 0 && y > value))
        {
            square += 1;
        }
    }

    square = (square / (double)points) * (y2 - y1) * (x2 - x1);
    add_in_integral(square);
    
    return NULL;
}


void add_in_integral(double integral_i)
{       
    pthread_mutex_lock(&mutex);

    integral += integral_i;
    printf("Новое значение %f\n", integral);
 
    pthread_mutex_unlock(&mutex);
}