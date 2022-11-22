#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>


#define BUF_SIZE 4096 

#ifdef M_BUF_SIZE 
#define BUF_SIZE 
#endif

#ifdef L_BUF_SIZE 
#define BUF_SIZE 
#endif

#ifdef S_SIZE
#define SIZE 4096
#endif

#ifdef M_SIZE 
#define SIZE 104857600
#endif


#define SIZE 2147483648


size_t get_file_size (FILE* file);

size_t get_file_size (FILE* file)
{
    size_t size = 0;

    fseek (file, 0, SEEK_END);
    size = (size_t)ftell (file); 
    fseek (file, 0, SEEK_SET);       

    return size;
}
