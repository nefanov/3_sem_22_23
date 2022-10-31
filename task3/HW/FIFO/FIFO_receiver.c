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
    printf("Writing waiting.\n");
    
    int fd_FIFO = open("FIFO", O_RDONLY);
    printf("Connected with writer!\n");

    int size = 0;

    char buf[DEF_SIZE];

    FILE* out = fopen("../output.txt", "w");
    if(out < 0)
    {
        printf("Error! output file not found!\n");
        return -1;
    }

    while((size = read(fd_FIFO, buf, sizeof(buf))) > 0)
        fwrite(buf, sizeof(char), size, out);

    close(fd_FIFO);
    fclose(out);
    return 0;
} 