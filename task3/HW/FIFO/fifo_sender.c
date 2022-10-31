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

    if (argc < 2) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR, FILE WASN'T INPUTED", ESC);
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }

    mknod(FIFO, S_IFIFO | 0666, 0);
    printf("Waiting for a reader\n");
    int fd = open(FIFO, O_WRONLY);

    if ( fd < 0 ) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE CREATING NAMED PIPE", ESC);
        return -1;
    }
    printf("A reader is connected\n");

    int size = 0;
    char buf[SIZE] = {};

    while (( size = fread(buf, sizeof(char), SIZE - 1, input)) > 0) {
        buf[size] = '\0';
        //printf("Send: %s", buf);    
        write(fd, buf, size);
    }
    fclose(input);
    close(fd);
    return 0;
}