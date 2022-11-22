#include <stdio.h>
#include "integration.h" 



int main() 
{    
    Interval_t interval;
    interval.a = 0;
    interval.b = 1; // Wait I = 0,(6) as result

    double exec_time;
    struct timespec start, stop; //init clock    
    clock_gettime(CLOCK_MONOTONIC, &start); //start

    int *shmptr = (int *)get_addr(SHM_SZ * sizeof(int));
    shmptr[0] = 0;  // Status
    shmptr[1] = 0;  // points under graph
    pid_t pid = fork(); //process for sending points and process for a result
        
    if (pid < 0) 
    { 
        perror("fork error");
        exit(1);
    }

    if (pid == 0) 
    {
        send(shmptr, interval);
        return 0;
    } 
    else 
    {
        receive(shmptr, interval);
        clock_gettime(CLOCK_MONOTONIC, &stop); //stop 
        
        double sec = (double)(stop.tv_sec - start.tv_sec);
        double nsec = (double)(stop.tv_nsec - start.tv_nsec);
        nsec /= (double)(1000000000);

        exec_time = sec + nsec;

    }

    printf("Time = %.5f\n", exec_time);
    
    return 0;
}

double function(double x) 
{
    return x*x*x*x;
}

void* generate_points (void *args) 
{
    Arg *args_pointer = (Arg *)args;
    double range = args_pointer->interval.b - args_pointer->interval.a;
    
    Point_t new_point;
    int points_under;

    u_int id = time(NULL) + pthread_self();
    srand(id);

    for (int i = 0; i < args_pointer->N; i++) 
    {
        new_point.x = (double)rand_r(&id) / (RAND_MAX) * range + args_pointer->interval.a; // x in [a,b]
        new_point.y = (double)rand_r(&id) / (RAND_MAX) * args_pointer->interval.b; // y in [0,b]

        if (new_point.y - function (new_point.x) < 0)
            points_under++;
    }

    pthread_mutex_lock(&p_mutex); //in critical section
    
    args_pointer->shm[0] += 1;
    args_pointer->shm[1] += points_under;

    pthread_mutex_unlock(&p_mutex); //from critical section
  
    pthread_exit(NULL);
}

void* get_addr(size_t shm_sz) 
{
    key_t key = KEY;

    int shmid = shmget(key, shm_sz, IPC_CREAT);
    if (shmid == -1) 
    {
        printf("%d, %ld", key, shm_sz);
        perror(" shmget error");
        exit(1);
    }

    char *shmptr = (char *)shmat(shmid, NULL, 0);
    if ((size_t)shmptr == -1) 
    {
        perror("shmat error");
        exit(1);
    }

      return shmptr;
}

void send(int *shm, Interval_t interval) 
{
    pthread_t threads[THREADS_NUM];
    Arg args[THREADS_NUM];

    long points_pthread = (long)(POINTS_NUM / THREADS_NUM);
    
    for (int i = 0; i < THREADS_NUM; i++) 
    {
        long N;
        if ((i + 1) == THREADS_NUM) 
        {
            N = POINTS_NUM - (i * points_pthread);
        } 
        else 
        {
            N = points_pthread;
        }

        args[i].shm = shm;
        args[i].N = N;
        args[i].interval = interval;

        pthread_create(threads + i, NULL, generate_points, (args + i));
    }

    for (int i = 0; i < THREADS_NUM; i++) 
    {
        pthread_join(*(threads + i), NULL);
    }
}

void receive(int *shm, Interval_t interval) 
{
    double result;

    while(shm[0] != THREADS_NUM);

    double range = interval.b - interval.a;
    result = ((double)shm[1] / POINTS_NUM) * range * range;

    printf("I = %.5f\n", result);
}