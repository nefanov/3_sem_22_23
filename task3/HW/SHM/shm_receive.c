#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#include "../test.h"

int main( ) {

    FILE *out = fopen("../output.txt", "w");
    
    if (!out) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }
 

    const char* name = NAME;
    int shm_fd;
    void* ptr;                 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    sem_t *sem, *sem1;
    if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0776, 0)) == SEM_FAILED ) {
            perror("sem_open");
            return 1;
    }

    if ( (sem1 = sem_open(SEMA_NAME, O_CREAT, 0776, 0)) == SEM_FAILED ) {
            perror("sem_open");
            return 1;
    }
    sem_wait(sem);
    size_t file_size = *(size_t*)ptr;
    int size = (int)file_size;
    printf("%lu\n", file_size);
    printf("%lu\n", *(size_t*)ptr);
    int ptr_size = 0;
    sem_post(sem1);
     
    while ( size > 0) { 

        sem_wait(sem);
        ptr_size = fwrite((char*)ptr, sizeof(char), SIZE, out);
        size -= ptr_size;
        sem_post(sem1);

    }

    sem_close(sem);
    fclose(out);
    shm_unlink(name);  
    return 0;
}    