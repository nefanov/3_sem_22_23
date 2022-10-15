#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>

const char* name = "HESOYAM";

#define DEFAULT_SIZE 4096

#define MEDIUM_SIZE

#ifdef SMALL_SIZE
#define SIZE 4096
#endif

#ifdef MEDIUM_SIZE
#define SIZE 65536
#endif

#ifdef LARGE_SIZE
#define SIZE 2147483648
#endif


size_t findFileSize(FILE* in)
{
    assert(in);

    size_t fileSize = 0;

    fseek (in, 0, SEEK_END);

    fileSize = ftell (in);

    fseek (in, 0, SEEK_SET);

    return (fileSize);
}


int main()
{ 
    int shm_fd = -1;
 
    void* ptr = NULL;
 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
 
    ftruncate(shm_fd, SIZE);
 
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    #ifdef LARGE_SIZE
    char inBuf[DEFAULT_SIZE];
    #endif

    FILE* in = fopen("input.txt", "r");
    if (!in)
    {
        fprintf(stderr, "File not found\n");
        return 1;
    }


    size_t file_size = findFileSize(in);

    if (file_size >= SIZE)
    {
        fprintf(stderr, "DATA TOO BIG\n");
        return 1;
    }

    #ifndef LARGE_SIZE

    fread((char*)ptr, sizeof(char), file_size, in);

    *(char*)(ptr + file_size) = '\0';

    fclose(in);
    #endif

    #ifdef LARGE_SIZE

    size_t rSize = 0;
    
    while(rSize = fread((char*)(ptr + rSize), sizeof(char), DEFAULT_SIZE, in) > 0)
    {
    }

    fclose(in);
    #endif

    return 0;
}