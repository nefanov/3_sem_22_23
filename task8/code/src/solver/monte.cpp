#include "header.hpp"


int HIT = 0;
pthread_mutex_t mutex {};


int SQR_ROOT_THREAD_NUM   = 4;
int THREAD_NUM    = SQR_ROOT_THREAD_NUM * SQR_ROOT_THREAD_NUM;


//HARDCODE HERE!!!!
double MIN_X = 0;
double MAX_X = 4;
double MIN_Y = 0;
double MAX_Y = 9;


double count_monte (int pthread_count, double x_min, double x_max,
                    double y_min, double y_max) {

    MIN_X = x_min;
    MAX_X = x_max;
    MIN_Y = y_min;
    MAX_Y = y_max;
    SQR_ROOT_THREAD_NUM = pthread_count / 2;
    THREAD_NUM = SQR_ROOT_THREAD_NUM * SQR_ROOT_THREAD_NUM;

/////////////////////////////////////////////////INIT MUTEX
    if (pthread_mutex_init (&mutex, NULL) < 0) {
        perror ("MUTEX INIT ERROR");
        exit (-1);
    } 
    
/////////////////////////////////////////////////INIT STEP
    double step_x  = (MAX_X - MIN_X) / SQR_ROOT_THREAD_NUM;
    double step_y  = (MAX_Y - MIN_Y) / SQR_ROOT_THREAD_NUM;
    double cur_x   = MIN_X;
    double cur_y   = MIN_Y;

/////////////////////////////////////////////////FILLIN
//INSTEAD OF TWO-DIMENSIONAL ARRAYS
    Args     args [THREAD_NUM] {};
    pthread_t tid [THREAD_NUM] {};

/////////////////////////////////////////////////
    for (int i = 0; i < SQR_ROOT_THREAD_NUM; ++i) {
        double cur_x = MIN_X;

        for (int j = 0; j < SQR_ROOT_THREAD_NUM; ++j) {

            args [i * SQR_ROOT_THREAD_NUM + j].min_x = cur_x;
            args [i * SQR_ROOT_THREAD_NUM + j].min_y = cur_y;
            
            args [i * SQR_ROOT_THREAD_NUM + j].max_x = cur_x + step_x;
            args [i * SQR_ROOT_THREAD_NUM + j].max_y = cur_y + step_y;

            args [i * SQR_ROOT_THREAD_NUM + j].func = func;
            
            if (pthread_create (tid + i * SQR_ROOT_THREAD_NUM + j, NULL, 
                                find_all_hits, args + i * SQR_ROOT_THREAD_NUM + j)
                                < 0) {
                perror ("CREATE ERROR");
                exit (-1);
            }
            cur_x += step_x;
        }
        cur_y += step_y;
    }


/////////////////////////////////////////////////WAIT FOR ALL THREADS
    for (int i = 0; i < (THREAD_NUM); i++)
        if (pthread_join (tid[i], NULL) < 0) {
            perror ("WAIT ERROR");
            exit (-1);
        }

/////////////////////////////////////////////////FREE MUTEX
    if (pthread_mutex_destroy (&mutex) < 0) {
        perror ("MUTEX DESTROY ERROR");
        exit (-1);
    }

/////////////////////////////////////////////////ANSWER

    double answer = ((MAX_X - MIN_X) * (MAX_Y - MIN_Y)) * HIT / POINT_NUM;
    fprintf (stderr, "ANSWER: %.2lf\n", answer);

    return answer;
}
