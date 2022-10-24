#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

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
    /* the size (in bytes) of shared memory object */
    if ( file_size >= SHM_SIZE) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR, FILE SIZE IS GREATER THEN SHM_SIZE", ESC);
        fprintf(stderr, "FILE SIZE IS %lu\n", file_size);
        return -1;
    }

    /* name of the shared memory object */
    const char* name = NAME;
 
    /* strings written to shared memory */
    //const char* message_0 = "Hello";
    //const char* message_1 = "World!";
 
    /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    void* ptr;
    
      
    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SHM_SIZE/*file_size*/);
    
    /* memory map the shared memory object */
    ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    *(size_t*)ptr = file_size;
    size_t ptr_size = 0, tmp_size = sizeof(size_t);
    while ( (ptr_size = fread((char*)ptr + tmp_size, sizeof(char), SIZE, input) ) > 0) {
        tmp_size += ptr_size;
        //printf("DEBUG\n");
    }  

    /* write to the shared memory object */
    return 0;
}