/* example on condition variable usage for pthread condition timewait. This code is gotten from QNX OS examples, but it is fully Linux compatible. */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c;

int main(int argc, char* argv[])
{
    struct timespec to;
    int retval;
    pthread_condattr_t attr;

    fprintf(stderr, "starting...\n");

    /* Set up the condvar attributes to use CLOCK_MONOTONIC. */

    pthread_condattr_init( &attr);
    pthread_condattr_setclock( &attr, CLOCK_MONOTONIC);
    pthread_cond_init( &c, &attr);

    /*
     Here's the interesting bit; we'll wait for
     five seconds FROM NOW when we call 
     pthread_cond_timedwait().
    */
    clock_gettime(CLOCK_MONOTONIC, &to);
    to.tv_sec += 5;

    if ((retval = pthread_mutex_lock(&m))) {
        fprintf(stderr, "pthread_mutex_lock %s\n", 
                strerror(retval));

        exit(EXIT_FAILURE);
    }
    
    if ((retval = pthread_cond_timedwait(&c, &m, &to)))
    {
        fprintf(stderr, "pthread_cond_timedwait %s\n",
                strerror(retval));

        exit(EXIT_FAILURE);
    }

    if ((retval = pthread_mutex_unlock(&m))) {
        fprintf(stderr, "pthread_mutex_unlock %s\n", 
                strerror(retval));

        exit(EXIT_FAILURE);
    }
 
    return EXIT_SUCCESS;
}
