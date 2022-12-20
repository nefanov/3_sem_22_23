#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#include "../Inc/test_size.h"


int min(ssize_t a, ssize_t b)
{
    return (a > b)? b: a;
}

int main (void)
{

    char* name = SHM_NAME;
    int shm_fd = 0;
    void* buffer_mem = NULL;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, MASSAGE_SIZE);
    buffer_mem = mmap(0, MASSAGE_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    sem_t *sem_A = sem_open(SEMAFOR_A, O_CREAT, 0776, 0);
    sem_t *sem_B = sem_open(SEMAFOR_B, O_CREAT, 0776, 0);

    FILE* input_file = fopen(FILE_NAME, "rb");

 

    size_t writed = 0;
    ssize_t ms_size = 0;

    while(writed < FILE_SIZE)
    {
        sem_wait(sem_B);
        writed += fread((char*) buffer_mem, sizeof(char), MASSAGE_SIZE, input_file);
        printf("\rsending [%3lu\%]", (writed*100)/FILE_SIZE);
        fflush(stdout);
        sem_post(sem_A);
    }

    //printf("\rsending_complit\n");
    fflush(stdout);
    sem_wait(sem_B);
    
    fclose(input_file);

    shm_unlink(name);
    
    sem_close(sem_A);
    sem_close(sem_B);

    return 0;
}