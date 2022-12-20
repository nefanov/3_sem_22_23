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


size_t get_filesize (FILE *in);


int main (void) {

   #if defined SMALL
        int in = open ("files/small",  O_RDONLY);
    #elif defined MEDIUM
        int in = open ("files/medium", O_RDONLY);
    #elif defined BIG
        int in = open ("files/big", O_RDONLY);
    #endif

    if (!in) {
        perror ("FILE OPEN ERROR");
        exit (-1);
    }

/////////////////////////////////////////////////
    // int filesize = get_filesize (in);
    // printf ("%d\n", filesize);


/////////////////////////////////////////////////
    char FIFO[] = "fifo";

    (void) umask (0);

    if (mknod (FIFO, S_IFIFO | 0666, 0) < 0) {
    //     perror ("MKNOD ERROR");
    //     exit (-1);
    }

    // fprintf (stderr, "HERE");

    int fd_fifo  = open (FIFO, O_RDWR);
    perror ("KEK");
    if (fd_fifo < 0) {
        perror ("OPEN ERROR");
        exit (-1);
    }
    // perror ("KEK");
/////////////////////////////////////////////////
    int cnt = 0;

    char buf[MAX_SIZE] {};

    // fprintf (stderr, "HERE");
    int size = 0;
    while (1)  {
    
        size = read (in, buf, MAX_SIZE);
        write (fd_fifo, buf, size);
        
        if (size != MAX_SIZE)
            break;
    }

    close  (fd_fifo);
    close (in);
}



size_t get_filesize (FILE *in) {

    assert (in);

    struct stat buff = {};

    fstat (fileno (in), &buff);

    return buff.st_size;
}