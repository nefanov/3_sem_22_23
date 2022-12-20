#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Inc/test_size.h"


int main (void)
{

    mknod("TEST_FIFO", S_IFIFO | 0666, 0);


    int FIFO_file = open("TEST_FIFO", O_WRONLY);

    fflush(stdout);

    char buffer[MASSAGE_SIZE] = {0};

    FILE* input_file = fopen(FILE_NAME, "r");

    fflush(stdout);

    size_t size_of_wright = 0;

    while ((size_of_wright = fread(buffer, sizeof(char), MASSAGE_SIZE, input_file)) > 0)
    {
        write(FIFO_file, buffer, size_of_wright);
    }
    
    close(FIFO_file);
    fclose(input_file);

    return 0;
}