#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../test.h"

int main( ) {

    FILE *out = fopen("../output.txt", "w");
    
    if (!out) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }

    /* name of the shared memory object */
    const char* name = NAME;
 
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
    size_t file_size = *(size_t*)ptr;
    size_t ptr_size = 0, tmp_size = sizeof(size_t);
    
    while ( tmp_size < file_size && (ptr_size = fwrite( (char*)ptr + tmp_size, sizeof(char), SIZE, out)) > 0) {
        tmp_size += ptr_size;
        //printf("debug\n");
    }
    //printf("debug\n");
    shm_unlink(name);
    /* write to the shared memory object */
    return 0;
}