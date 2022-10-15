#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define DEFAULT_SIZE 4096
#define FIFO "fifo"

#define MEDIUM_SIZE

#ifdef SMALL_SIZE
#define SIZE 4096
#endif

#ifdef MEDIUM_SIZE
#define SIZE 65536
#endif

#ifdef LARGE_SIZE
#define SIZE 2147483648
#endif


int main()
{
    mknod(FIFO, S_IFIFO | 0666, 0);
    printf("Waiting for a reader\n");
    int fd_FIFO = open(FIFO, O_WRONLY);
    int fd_in   = open("input.txt", O_RDONLY);
    printf("A reader is connected\n");

    int size = 0;
    char buf[SIZE];

    #ifndef LARGE_SIZE

    while (( size = read(fd_in, buf, sizeof(buf))) > 0) 
    {
        write(fd_FIFO, buf, size);
    }

    close(fd_FIFO);
    close(fd_in);

    #endif

    return 0;
}