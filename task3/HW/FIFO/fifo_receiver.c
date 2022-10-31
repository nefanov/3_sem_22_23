#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

    
#define FIFO "sample_text"
#define RED "\x1b[31m"
#define ESC "\x1b[0m"

#include "../test.h"

int main(int argc, char *argv[]) {

    FILE *out = fopen("../output.txt", "w");
    if (!out) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }
  
    mknod(FIFO, S_IFIFO | 0666, 0);
    printf("Waiting for a writer\n");
    int fd = open(FIFO, O_RDONLY);
    printf("A writer is connected\n");

    if ( fd < 0 ) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE CREATING NAMED PIPE", ESC);
        return -1;
    }

    int size = 0;
    char buf[SIZE] = {}; // size is not affecting anything

    while (( size = read(fd, buf, sizeof(buf))) > 0) {
        fwrite(buf, sizeof(char), size, out);
    }
    fclose(out);
    close(fd);
    return 0;
}