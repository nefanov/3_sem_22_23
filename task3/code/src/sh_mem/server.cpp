#include "sys/shm.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "sys/stat.h"


// #define FIRST
// #define SECOND
#define THIRD

#if defined FIRST
    const int MAX_SIZE = 128;
#elif defined SECOND
    const int MAX_SIZE = 1024;
#elif defined THIRD
    const int MAX_SIZE = 10000;
#endif


size_t get_filesize (FILE *in);

int main (void) {
    // fprintf (stderr, "%d\n", MAX_SIZE);

    #if defined SMALL
        FILE *in = fopen ("files/small",  "rb");
    #elif defined MEDIUM
        FILE *in = fopen ("files/medium", "rb");
    #elif defined BIG
        FILE *in = fopen ("files/big",    "rb");
    #endif

    if (!in) {
        perror ("FILE OPEN ERROR");
        exit (-1);
    }

    size_t filesize = get_filesize (in);
    printf ("%ld\n", filesize);
/////////////////////////////////////////////////
    key_t key = ftok ("/home/ropho/plus/nef/HA_3sem/task3/code/makefile", 0);
    if (key < 0) {
        perror ("KEY GEN ERROR");
        exit (-1);
    }
    
    filesize += sizeof (int);
    int shmid = shmget (key, filesize, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror ("MEMMORY GET ERROR");
        exit (-1);
    }
    filesize -= sizeof (int);


/////////////////////////////////////////////////INPUT
    char *arr = (char *) shmat (shmid, NULL, 0);
    if ((int *) arr == (int *) -1) {
        perror ("ATTACH ERROR");
        exit (-1);
    }

    int cnt = sizeof (int);
    *((int *)arr) = filesize;

    if (MAX_SIZE >= filesize) {

        if (fread (arr + cnt, sizeof (char), filesize, in) != filesize) {
            perror ("READ ERROR");
            exit (-1);
        }   
    }
    else {

        while (cnt < filesize + sizeof (int)) {
            cnt += fread (arr + cnt, sizeof (char), MAX_SIZE, in);
        }

    }


    // for (int i = 0; i < filesize + sizeof (int); ++i) {
    //     printf ("%c", arr[i]);
    // }


    if (shmdt (arr) < 0) {
        perror ("CANT CLEAR");
        exit (-1);
    }

    fclose (in);
}




size_t get_filesize (FILE *in) {

    assert (in);

    struct stat buff = {};

    fstat (fileno (in), &buff);

    return buff.st_size;
}