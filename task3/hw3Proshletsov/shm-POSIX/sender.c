#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../common.h"

const char *KEY = "makeit";

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Wrong amount of arguments\n");
    }
    const char* name_mutex = "mut";
    
    int shm_fd = -1;    
    void* shm_pointer = NULL;

    shm_fd = shm_open (KEY, O_CREAT | O_RDWR, 0666);

    int checker = ftruncate (shm_fd, BUF_SIZE);
    if (checker)
    {
        perror ("ftuncate err: ");
    }

    shm_pointer = mmap (0, BUF_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    FILE* input = fopen (argv[1], "r");
    if (!input)
    {
        perror ("fopen err: ");
        return 1;
    }

    size_t fsize = get_file_size (input);

    size_t read = 0;
    size_t total_read = 0;
    size_t counter = 0;

    for(; total_read < SIZE; counter++)
    {
        read = fread ((char*) shm_pointer, sizeof(char), BUF_SIZE, input);
        total_read += read;
    }

    printf ("AAASent %zu in %zu cycles\n", total_read, counter);

    fclose (input);

    return 0;
}

