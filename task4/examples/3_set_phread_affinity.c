//source: https://stackoverflow.com/questions/1407786/how-to-set-cpu-affinity-of-a-particular-pthread
//compilation: gcc -o affinity affinity.c -lpthread

#define _GNU_SOURCE
#include <sched.h>   //cpu_set_t , CPU_SET
#include <pthread.h> //pthread_t
#include <stdio.h>

void *th_func(void * arg); 

int main(void) {
  pthread_t thread; //the thread

  pthread_create(&thread,NULL,th_func,NULL); 

  pthread_join(thread,NULL);   

  return 0;
}


void *th_func(void * arg)
{  
  //we can set one or more bits here, each one representing a single CPU
  cpu_set_t cpuset; 

  //the CPU we whant to use
  int cpu = 2;

  CPU_ZERO(&cpuset);       //clears the cpuset
  CPU_SET( cpu , &cpuset); //set CPU 2 on cpuset


  /*
   * cpu affinity for the calling thread 
   * first parameter is the thread id, 0 = calling thread
   * second parameter is the size of your cpuset
   * third param is the cpuset in which your thread will be
   * placed. Each bit represents a CPU
   */
  sched_setaffinity(0, sizeof(cpuset), &cpuset);

  while (1);
       ; //burns the CPU 2

  return 0;
}
