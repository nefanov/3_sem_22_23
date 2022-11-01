#include <stdio.h>
#include "integral.h" 

static pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;

double integ_function(double x) 
{
    return x*x*x;
}

void* generate_points(void *Args) 
{
    Arg *Args_pointer = (Arg *)Args;
    double range = Args_pointer->Interval.b - Args_pointer->Interval.a;
    
    Point New_point;
    int points_under;

    u_int id = time(NULL) + pthread_self();
    srand(id);

    for (int i = 0; i < Args_pointer->N; i++) 
    {
        New_point.x = (double)rand_r(&id) / (RAND_MAX) * range + Args_pointer->Interval.a; // x in [a,b]
        New_point.y = (double)rand_r(&id) / (RAND_MAX) * Args_pointer->Interval.b; // y in [0,b]

        if (New_point.y - integ_function(New_point.x) < 0)
            points_under++;
    }

    pthread_mutex_lock(&pmutex); //in critical section
    
    Args_pointer->shm[0] += 1;
    Args_pointer->shm[1] += points_under;

    pthread_mutex_unlock(&pmutex); //from critical section
  
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

void Send(int *shm, Interval Interval) 
{
    pthread_t threads[THREADS_NUM];
    Arg Args[THREADS_NUM];

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

        Args[i].shm = shm;
        Args[i].N = N;
        Args[i].Interval = Interval;

        pthread_create(threads + i, NULL, generate_points, (Args + i));
    }

    for (int i = 0; i < THREADS_NUM; i++) 
    {
        pthread_join(*(threads + i), NULL);
    }
}

void Receive(int *shm, Interval Interval) 
{
    double result;

    while(shm[0] != THREADS_NUM);

    double range = Interval.b - Interval.a;
    result = ((double)shm[1] / POINTS_NUM) * range * range;

    printf("I = %.5f\n", result);
}

int main() 
{    
    Interval Interval;
    Interval.a = 0;
    Interval.b = 1; // Wait I = 0,(6) as result

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
        Send(shmptr, Interval);
        return 0;
    } 
    else 
    {
        Receive(shmptr, Interval);
        clock_gettime(CLOCK_MONOTONIC, &stop); //stop 
        
        double sec = (double)(stop.tv_sec - start.tv_sec);
        double nsec = (double)(stop.tv_nsec - start.tv_nsec);
        nsec /= (double)(1000000000);

        exec_time = sec + nsec;

    }

    printf("Time = %.5f\n", exec_time);
    
    return 0;
}