#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FIFO "fifo"

#define DEFAULT_SIZE 65536

#ifdef SMALL_SIZE
#define SIZE 4096
#endif

#ifdef MEDIUM_SIZE
#define SIZE 104857600
#endif

#ifdef LARGE_SIZE
#define SIZE 2147483648
#endif


int main()
{
    mknod(FIFO, S_IFIFO | 0666, 0);
    printf("Waiting for a writer\n");
    int fd = open(FIFO, O_RDONLY);
    printf("A writer is connected\n");
    
    int size = 0;

    char buf[DEFAULT_SIZE];

    FILE* out = fopen("./FIFO/output.txt", "w");
    
    while ((size = read(fd, buf, sizeof(buf))) > 0) 
    {
        fwrite(buf, sizeof(char), size, out);
    }
    
    close(fd);
    fclose(out);
    return 0;
}