#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>

const char* name = "HESOYAM";

#define SIZE 4096

int findFileSize(FILE* in)
{
    assert(in);

    int fileSize = 0;

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
 
    char inBuf[SIZE];

    FILE* in = fopen("input.txt", "r");
    if (!in)
    {
        fprintf(stderr, "File not found\n");
        return 1;
    }

    int file_size = findFileSize(in);

    if (file_size >= SIZE)
    {
        fprintf(stderr, "DATA TOO BIG\n");
        return 1;
    }
    
    fread(inBuf, sizeof(char), file_size, in);
    inBuf[file_size] = '\0';
    fclose(in);

    sprintf(ptr, "%s", inBuf);
 
    return 0;
}