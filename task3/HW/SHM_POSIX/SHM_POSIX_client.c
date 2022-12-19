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

size_t file_size(FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    size_t size = (size_t)ftell(file); 
    fseek(file, 0, SEEK_SET);

    return size; 
}

int main()
{

    int shm_fd = 123;
    void* p = NULL;
    
    shm_fd = shm_open(word, O_CREAT | O_RDWR, 0666);
    
    ftruncate(shm_fd, DEF_SIZE);
    p = mmap(0, DEF_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
    
    if(p == MAP_FAILED)
    {
        printf("ERROR WITH MMAP!\n");
        exit(1);
    }

    FILE* input = fopen("../input.txt", "r");
    if (!input)
    {
        printf("File not found\n");
        return 1;
    }

    size_t fsize = file_size(input);

    size_t read_size = 0;
    size_t counter = 0;

    while(read_size = fread((char*)(p), sizeof(char), DEF_SIZE, input) > 0)
    {      
        counter++;
    }

    printf("Sent %d bytes -> numPackets: %lu\n", DEF_SIZE, counter);

    fclose(input);
    return 0;
}
