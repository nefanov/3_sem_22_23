// C program for Consumer process illustrating
// POSIX shared-memory API.
// monitor it using ipcs
// lsof -p 16894
// fuser some-huge-file.txt
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

const char* name = "HESOYAM";

#define SIZE 4096

int main()
{
    int shm_fd = -1;
 
    void* ptr = NULL;
 
    shm_fd = shm_open(name, O_RDONLY, 0666);
 
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
 
    printf("%s", (char*)ptr);
 
    shm_unlink(name);
    return 0;
}