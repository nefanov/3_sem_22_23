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

    int FIFO_file = open("TEST_FIFO", O_RDONLY);

    fflush(stdout);

    char buffer[MASSAGE_SIZE] = {0};

    FILE* output_file = fopen("../test_output", "w");

    fflush(stdout);

    size_t size_of_read = 0;

    while ((size_of_read = read(FIFO_file, buffer, MASSAGE_SIZE)) > 0)
    {
        fwrite(buffer, sizeof(char), size_of_read, output_file);
    }
    
    close(FIFO_file);
    fclose(output_file);

    return 0;
    
}