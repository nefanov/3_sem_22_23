#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

const size_t DPS = 10000000;

#define TREAD_NUM 8

pthread_t My_treads[TREAD_NUM];


double f (double x);

double rand_d(double min, double max);

double one_stream_F(double a, double b, double y_min, double y_max, double f(double x));


const size_t SELL_H_NUM = 100;
const size_t SELL_W_NUM = 100;

const size_t NUM_OF_SELS = SELL_H_NUM * SELL_W_NUM;


double integer = 0;

pthread_mutex_t integer_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_to_ineger (double add)
{
    pthread_mutex_lock(&integer_mutex);

    integer += add;
    
    pthread_mutex_unlock(&integer_mutex);
}

void clean_ineger (void)
{
    pthread_mutex_lock(&integer_mutex);

    integer = 0;
    
    pthread_mutex_unlock(&integer_mutex);
}


size_t sell_itter = 0; 

pthread_mutex_t sell_itter_mutex = PTHREAD_MUTEX_INITIALIZER;

ssize_t take_sell_itter (void)
{
    pthread_mutex_lock(&sell_itter_mutex);

    if(sell_itter < NUM_OF_SELS)
    {
        ssize_t ret = sell_itter;
        sell_itter ++;
        
        pthread_mutex_unlock(&sell_itter_mutex);
        return ret; 
    }
    else
    {
        pthread_mutex_unlock(&sell_itter_mutex);
        return -1;
    }
}

void clean_sell_itter (void)
{
    pthread_mutex_lock(&sell_itter_mutex);

    sell_itter = 0;
    
    pthread_mutex_unlock(&sell_itter_mutex);
}

ssize_t take_sell_x(ssize_t sell_num)
{
    return sell_num % SELL_W_NUM;    
}

ssize_t take_sell_y(ssize_t sell_num)
{
    return sell_num / SELL_W_NUM;
}



double f (double x)
{
    return x;
}


double rand_d(double min, double max)
{
   
    double range = RAND_MAX*(max - min);
    double offset = rand()/range;

    return min + offset; 
}

double one_stream_F (double a, double b, double y_min, double y_max, double f(double x))
{
    assert(b > a);
    assert(y_max > y_min);
    
    double S_sell = (y_max - y_min)*(b - a);

    size_t dot_num = S_sell*DPS;
    size_t dot_ander = 0;

    size_t itter = 0;

    for(itter = 0; itter < dot_num; itter++)
    {
        double rand_x = rand_d(a, b);
        double rand_y = rand_d(y_min, y_max);

        if(rand_y < (*f)(rand_x)) dot_ander++;
    }
    
    return S_sell*((double)dot_ander/dot_num);
}

void F_init()
{
    srand (time ( NULL));
    clean_ineger();
}

void* sell_F (void* arg)
{
    ssize_t itter = 0;

    double* args = arg;

    double a = args[0];
    double b = args[1];
    double y_min = args[2];
    double y_max = args[3];

    double SELL_W = (b - a)/SELL_W_NUM;
    double SELL_H = (y_max - y_min)/SELL_H_NUM;
    
    while(1)
    {
        if((itter = take_sell_itter()) == -1) {return NULL;}

        ssize_t sell_X = take_sell_x(itter);
        ssize_t sell_Y = take_sell_y(itter);

        add_to_ineger(one_stream_F((sell_X * SELL_W) + a, ((sell_X + 1) * SELL_W) + a, (sell_Y * SELL_H) + y_min, ((sell_Y + 1) * SELL_H) + y_min, f));
    }    
}

double F (double a, double b, double y_min, double y_max)
{
    size_t tread_itter = 0;

    double args[4] = {a, b, y_min, y_max};

    for(tread_itter = 0; tread_itter < TREAD_NUM; tread_itter ++)
    {
        pthread_create(&(My_treads[tread_itter]), NULL, sell_F, &args);
    }
    for(tread_itter = 0; tread_itter < TREAD_NUM; tread_itter ++)
    {
        pthread_join(My_treads[tread_itter], NULL);
    }

    double ans = integer;

    clean_ineger();

    return ans;

}

int main(void)
{
    double a = 0;
    double b = 1;

    printf("F[%.2lf:%.2lf] = %.3lf \n", f(a), f(b), F(a, b, f(a), f(b)));
}