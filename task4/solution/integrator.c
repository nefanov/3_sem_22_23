#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define FUNCTION(x) exp(x)

#ifndef THREADS_NUM
#define THREADS_NUM 8
#endif
//10 billion points
#define POINTS_NUM 1000000000ll
// this define used for True Random algo, because pseudo random gives not exact results
#define TRUE_RANDOM
static pthread_mutex_t pmutex;

typedef struct {
  double x, y;
}point_t;


typedef struct {
  point_t min, max;
} interval_t;

typedef struct {
  long long *shm_dat;
  long long n_points;
  interval_t rect;
} arg_t;

int is_belongs(point_t point) {
  return point.y < FUNCTION(point.x) ? 1 : 0;
}


void *getshm(const char *path, size_t shm_sz, int* fd) {
  key_t key = ftok(path, 1);
  if (key == -1) {
    perror("ftok error for");
    exit(1);
  }

  *fd = shmget(key, shm_sz, IPC_CREAT | 0666);
  if (*fd == -1) {
    printf("%s %d, %ld", path, key, shm_sz);
    perror(" shmget error");
    exit(1);
  }

  void* shmptr = shmat(*fd, NULL, 0666);
  if (shmptr == NULL) {
    perror("shmat error");
    exit(1);
  }

  return shmptr;
}

void dump_data(arg_t args, unsigned long thread_num, long long n_hits) {
  fprintf(stdout, "Thread with number %lu started\n", thread_num);
  fprintf(stdout, "Geometry limits: \n\tx=[%.5lf, %.5lf]\n\ty=[%.5lf, %.5lf]\n", 
          args.rect.min.x, args.rect.max.x, args.rect.min.y, args.rect.max.y);
  fprintf(stdout, "Hits: %lld of %lld\n", n_hits, args.n_points);
}

void* integrate_cell(void* args){
  arg_t* arg = (arg_t*) args;
  double x_diff = arg->rect.max.x - arg->rect.min.x;
  double y_diff = arg->rect.max.y - arg->rect.min.y;
  point_t point;
  long long s_hit = 0;
  struct drand48_data data;
  srand48_r(pthread_self() % LONG_MAX, &data);
  unsigned short randstate[3];
#ifdef TRUE_RANDOM  
  FILE* urandom = fopen ("/dev/urandom", "r");
  setvbuf (urandom, NULL, _IONBF, 0);


  
  randstate[0] = (fgetc (urandom) << 8) | fgetc (urandom);
  randstate[1] = (fgetc (urandom) << 8) | fgetc (urandom);
  randstate[2] = (fgetc (urandom) << 8) | fgetc (urandom);
  fclose (urandom);
#else  //gives bad results in aqquracy
  randstate[0] = pthread_self() % SHRT_MAX;
  randstate[0] = (pthread_self() << 2) % SHRT_MAX;
  randstate[0] = (pthread_self() >> 1) % SHRT_MAX;
#endif
  double rand_x, rand_y;
  
  for (int i = 0; i < arg->n_points; i++) {
    erand48_r(randstate, &data, &rand_x);
    erand48_r(randstate, &data, &rand_y);
    point.x = arg->rect.min.x + rand_x * x_diff;
    point.y = arg->rect.min.y + rand_y * y_diff;

    if (is_belongs(point)) {
      s_hit++;
    }
  }

  pthread_mutex_lock(&pmutex);
  arg->shm_dat[0] += s_hit;

  pthread_mutex_unlock(&pmutex);
#ifdef DEBUG  
  dump_data(*arg, pthread_self(), s_hit);
#endif
  pthread_exit(NULL);
}

void start_threads(long long *shm,  interval_t interval) {
  pthread_t threads[THREADS_NUM];
  arg_t args[THREADS_NUM];

  long K = (long)(POINTS_NUM / THREADS_NUM);
  double delta_x = (interval.max.x - interval.min.y) / THREADS_NUM;
  double cur_x = interval.min.x;
  interval_t res;
  res.min.y = interval.min.y;
  res.max.y = interval.max.y;
  for (int i = 0; i < THREADS_NUM; i++) {
    long N;
    if ((i + 1) == THREADS_NUM) {
      N = POINTS_NUM - (i * K);
    } else {
      N = K;
    }
    res.min.x = cur_x;
    res.max.x = cur_x + delta_x;
    cur_x += delta_x;
    
    args[i].shm_dat = shm;
    args[i].n_points = N;
    args[i].rect = res;

    pthread_create(threads + i, NULL,
                   integrate_cell, &args[i]);
  }
  for (int i = 0; i < THREADS_NUM; i++) {
    pthread_join(*(threads + i), NULL);
  }
}

double square(interval_t interval) {
  return (interval.max.x - interval.min.x) * (interval.max.y - interval.min.y);
}

int main(int argc, char* argv[]) {
  interval_t interval = {{0.0, 0.0},{20.0, FUNCTION(20.0)}};
  size_t size = sizeof(long long);
  int fd;
  struct timespec start, stop;
  FILE* file_desc = fopen("res.txt", "a");
  clock_gettime(CLOCK_MONOTONIC, &start);
  long long* shm = getshm("./integrator.c", size, &fd);
  start_threads(shm, interval);
  printf("Result: %lf\n", square(interval) * (*shm) / POINTS_NUM);
  shmctl(fd, IPC_RMID, NULL);
  clock_gettime(CLOCK_MONOTONIC, &stop);
  double sec = (double)(stop.tv_sec - start.tv_sec);
  double nsec = (double)(stop.tv_nsec - start.tv_nsec);
  nsec /= 1000000000.0;
  double exec_time = sec + nsec;
  fprintf(file_desc, "%d %lf\n", THREADS_NUM, exec_time);
  fclose(file_desc);
  return 0;
}