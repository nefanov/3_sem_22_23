#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define FIRST
// #define SECOND
// #define THIRD

#if defined FIRST
    const int MAX_SIZE = 120;
#elif defined SECOND
    const int MAX_SIZE = 1024;
#elif defined THIRD
    const int MAX_SIZE = 10000;
#endif


int main (void) {

    FILE *out = fopen ("files/out", "w");
    if  (!out) {
        perror ("FILE OPEN ERROR");
        exit (-1);
    }

/////////////////////////////////////////////////
    char FIFO[] = "fifo";

    (void) umask (0);

    if (mknod (FIFO, S_IFIFO | 0666, 0) < 0) {
        perror ("MKNOD ERROR");
    //     exit (-1);
    }


    int fd_fifo  = open (FIFO, O_RDWR);
    if (fd_fifo < 0) {
        perror ("OPEN ERROR");
        exit (-1);
    }
    // fprintf (stderr, "%d\n", fd_fifo);
/////////////////////////////////////////////////
    int cnt = 0;

    char buf[MAX_SIZE] {};

    while (1)  {
        
        cnt = read (fd_fifo, buf, MAX_SIZE);
        // printf ("%d\n", cnt);
        // puts (buf);
        fwrite (buf, sizeof(char), cnt, out);
        if (cnt != MAX_SIZE) {
            break;
        }
    }

    close  (fd_fifo);
    fclose (out);
}