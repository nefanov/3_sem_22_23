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

int main ()
{

    int shm_fd = -1;
    void* shm_pointer = NULL;
 
    shm_fd = shm_open (KEY, O_CREAT | O_RDWR, 0666);
 
    ftruncate(shm_fd, BUF_SIZE);
    
    shm_pointer = mmap(0, BUF_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0); 
    
    if (shm_pointer == MAP_FAILED)
    {
        perror ("mmap file: ");
        return 1;
    }
    else printf("Access got!\n");


    shm_unlink (shm_pointer);

    return 0;
}