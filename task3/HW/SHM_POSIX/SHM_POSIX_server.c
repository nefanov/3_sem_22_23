#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<unistd.h>

#include "../sizes.h"
const char* word = "Awes0me";


int main()
{
    sleep(0.0001); //for starting after sendting from client in onetime running
    
    int shm_fd = 123;
    void* p = NULL;
 
    shm_fd = shm_open(word, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, DEF_SIZE);
    
    p = mmap(0, DEF_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
    
    if(p == MAP_FAILED)
    {
        printf("ERROR with MMAP!\n");
        exit(1);
    }
    else printf("Access got!\n");

    // for printing in somewhere...
    // FILE* output = open("../output.txt", "w");
    // if (!output)
    // {
    //     printf("File not found\n");
    //     return 1;
    // }

    
    shm_unlink(word);
    return 0;
}