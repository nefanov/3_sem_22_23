#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common.h"

int main()
{
    mknod ("FIFO", S_IFIFO | 0666, 0);

    printf ("Waiting for a writer\n");

    int fd_fifo = open ("FIFO", O_RDONLY);
    printf ("Writer connected\n");

    int size = 0;

    char buf[BUF_SIZE];

    while (size = read (fd_fifo, buf, sizeof(buf)) > 0);

    printf ("FINISHED\n");

    close (fd_fifo);
    
    return 0;
}