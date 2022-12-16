#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

const int N = 10000000;

int RES = 0;

int THREAD_NUM = 100;

pthread_mutex_t mutex;

double integral_func (const double x) {

    return exp ( -x*x / 2);  // integral is 0.8556243918921488
}


int integral (const double x_min, const double x_max, const double y_min, const double y_max, const int count,unsigned int seed){

    double x = 0, y = 0;
    int P = 0;
     
    for (int i = 0; i < count; i++){

        x = x_min + (double)(rand_r(&seed) * (x_max - x_min)/(double)RAND_MAX);
        y = y_min + (double)(rand_r(&seed) * (y_max - y_min)/(double)RAND_MAX);
        if ( y <= integral_func(x)) P++;
    }
    return P;
}

typedef struct  integral_data_t {

    double x_min;
    double x_max;
    double y_min;
    double y_max;
} thread_data;

void* thread_func(void* integral_data){
    
    if (! integral_data ){
        printf("ERROR, NULL PTR\n");
        return NULL;
    }
    thread_data* data = (thread_data*)integral_data;

    double x_min = data -> x_min;
    double x_max = data -> x_max;
    double y_min = data -> y_min;
    double y_max = data -> y_max;

    unsigned int seed = time(NULL);  /*_Thread_local*/
    int P = integral( x_min, x_max, y_min, y_max, N / THREAD_NUM, seed);

    pthread_mutex_lock(&mutex);
    
    RES += P;   // CS

    pthread_mutex_unlock(&mutex);
    
    return NULL;
}


int main(){

    pthread_mutex_init(&mutex, NULL);

    double y_min = 0, y_max = 1, x_min = 0, x_max = 1;
    double tmp_x_min = 0, tmp_x_max = 0;
    double step_x = (x_max - x_min) / THREAD_NUM;
    
    pthread_t tid[THREAD_NUM];
    thread_data integral_data[THREAD_NUM];
    
    for (int i = 0; i < THREAD_NUM; i++) {

        tmp_x_max += step_x;
        integral_data[i].x_min = tmp_x_min;
        integral_data[i].y_min = y_min;
        integral_data[i].x_max = tmp_x_max;
        integral_data[i].y_max = y_max;
        
        pthread_create(tid + i, NULL, thread_func, (void*)(integral_data+ i));
        
        tmp_x_min = tmp_x_max;
                
    }
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);


    double s = (y_max - y_min) * (x_max - x_min);
    double result = (double)RES / (N) * s;
    printf(" integral equals %lf\n", result);

    return 0;  

}