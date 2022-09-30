/* This program calculates the time required to
 * execute the program specified as its first argument.
 * The time is printed in seconds, on standard out.

 * The code is taken from QNX OS examples and modified to use CLOCK_MONOTONIC from HPET timer. It is fully Linux compatible
 */
 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BILLION  1000000000L;

int main( int argc, char** argv )
  {
    struct timespec start, stop;
    double accum;

    if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    system( argv[1] );

    if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    accum = ( stop.tv_sec - start.tv_sec )
             + (double)( stop.tv_nsec - start.tv_nsec )
/                (double)BILLION;
    printf( "%lf seconds\n", accum );
    return EXIT_SUCCESS;
  }
