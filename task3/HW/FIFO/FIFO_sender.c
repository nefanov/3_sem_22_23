#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../sizes.h"

int main()
{
    mknod("FIFO", S_IFIFO | 0666, 0);
    printf("Reader waiting.\n");

    int fd_FIFO = open("FIFO", O_WRONLY);

    int fd_in = open("../input.txt", O_RDONLY);
    if(fd_in < 0)
    {
        printf("Error! input file not found!\n");
        return -1;
    }

    printf("Connected with reader\n");
    
    int size = 0;
    char buf[DEF_SIZE];

    while((size = read(fd_in, buf, sizeof(buf))) > 0)
        write(fd_FIFO, buf, size);

    close(fd_FIFO);
    close(fd_in);
    return 0;
}