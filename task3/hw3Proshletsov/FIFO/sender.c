#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Wrong arguments\n");
        return 1;
    }

    mknod ("FIFO", S_IFIFO | 0666, 0);
    printf ("Writer waiting\n");

    int fd_fifo = open ("FIFO", O_WRONLY);
    FILE* input = fopen (argv[1], "r");

    printf ("Connected with reader\n");

    size_t size = 0;
    size_t size_sent = 0;
    char buf[BUF_SIZE];

    while ((size = fread (buf, sizeof(char), BUF_SIZE, input)) && size_sent < SIZE)
    {
        size_sent += size;
        write (fd_fifo, buf, size);
    }

    printf ("FINISHED\n");

    close (fd_fifo);
    fclose (input);

    return 0;
}