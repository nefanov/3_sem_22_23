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

    struct stat code_stat = { };

    if ( fstat ( fileno(input), &code_stat) == -1 ) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR GETTING FILE SIZE", ESC);
        return -1;
    }

    size_t file_size = (size_t)code_stat.st_size;

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

    sem_t* sem, *sem1;
    if ( (sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED ) {
        perror("sem_open");
        return 1;
    }

    if ( (sem1 = sem_open(SEMA_NAME, 0)) == SEM_FAILED ) {
        perror("sem_open");
        return 1;
    }


    *(size_t*)ptr = file_size;
    printf("%lu\n", *(size_t*)ptr);
    size_t ptr_size = 0;
    sem_post(sem); 
 
    while ( file_size > 0){     
 
        sem_wait(sem1);
        ptr_size = fread((char*)ptr, sizeof(char), SIZE, input); 
        file_size -= ptr_size;
        sem_post(sem);

    }

    shm_unlink(name);
    sem_close(sem);
    fclose(input); 

    return 0;
}