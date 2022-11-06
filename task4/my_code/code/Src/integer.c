#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

const size_t DPS = 10000000;


double f (double x);

double rand_d(double min, double max);

double one_stream_F(double a, double b, double y_min, double y_max, double f(double x));

typedef struct tread_data
{
    double a;
    double b; 
    double y_min; 
    double y_max;
    size_t tread_num;
    size_t my_tread;
} data;



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


double f (double x)
{
    return x;
}


double rand_d(double min, double max)
{
   
    double range = (double)RAND_MAX/(max - min);
    double offset = rand()/range;

    return min + offset; 
}

double one_stream_F (double a, double b, double y_min, double y_max, double f(double x))
{
    assert(b > a);
    assert(y_max > y_min);
    
    double S_sell = (y_max - y_min)*(b - a);

    size_t dot_num = S_sell*(double)DPS;
    size_t dot_ander = 0;

    size_t itter = 0;

    for(itter = 0; itter < dot_num; itter++)
    {
        double rand_x = rand_d(a, b);
        double rand_y = rand_d(y_min, y_max);

        if(rand_y < (*f)(rand_x)) dot_ander++;
    }
    
    //printf("a:%lf, b:%lf, y_min:%lf, y:max:%lf, ans:%lf, dot num:%lu \n", a, b, y_min, y_max, (S_sell*dot_ander)/dot_num, dot_num);

    return ((S_sell*dot_ander)/dot_num);
}

void F_init()
{
    srand (time ( NULL));
    clean_ineger();
}

void* sell_F (void* arg)
{

    data args = *(data*)arg;

    double a = args.a;
    double b = args.b;
    double y_min = args.y_min;
    double y_max = args.y_max;
    size_t tread_num = args.tread_num;

    size_t my_tread = args.my_tread;

    add_to_ineger(one_stream_F(a + ((b - a) * my_tread)/tread_num, a + ((b - a) * (my_tread + 1))/tread_num, y_min, y_max, f));
    
}

double F (double a, double b, double y_min, double y_max, size_t treads_num)
{
    assert(treads_num > 0);
    
    size_t tread_itter = 0;

    pthread_t* My_treads = calloc(treads_num, sizeof(pthread_t));
    data* args_data = calloc(treads_num, sizeof(data));

    for(tread_itter = 0; tread_itter < treads_num; tread_itter ++)
    {
        (args_data + tread_itter)->a = a;
        (args_data + tread_itter)->b = b;
        (args_data + tread_itter)->y_max = y_max;
        (args_data + tread_itter)->y_min = y_min;
        (args_data + tread_itter)->my_tread = tread_itter;
        (args_data + tread_itter)->tread_num = treads_num;
        
        pthread_create(&(My_treads[tread_itter]), NULL, sell_F, (args_data + tread_itter));
    }
    for(tread_itter = 0; tread_itter < treads_num; tread_itter ++)
    {
        pthread_join(My_treads[tread_itter], NULL);
    }

    double ans = integer;

    clean_ineger();
    free(My_treads);
    free(args_data);

    return ans;

}

int main(int argc, char* argv[])
{
    double a = 0;
    double b = 1;

    if(argc != 2)
    {
        printf("arg err\n");
        return 1;
    }
    
    size_t tread_num = 0;
    
    sscanf(argv[1], "%lu", &tread_num);

    F_init();

    printf("F[%.2lf:%.2lf] = %.3lf \n", a, b, F(a, b, f(a), f(b), tread_num));

    pthread_mutex_destroy(&integer_mutex);


}