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


int main (void) {

/////////////////////////////////////////////////
    key_t key = ftok ("/home/ropho/plus/nef/HA_3sem/task3/code/makefile", 0);
    if (key < 0) {
        perror ("KEY GEN ERROR");
        exit (-1);
    }


    int shmid = shmget (key, 4, IPC_CREAT | 0666);
    // printf ("%d\n\n\n", shmid);
    if (shmid < 0) {
        perror ("MEMMORY GET ERROR");
        exit (-1);
    }
    int *num = (int*) shmat (shmid, NULL, 0);
    int filesize = *num;
    // return 0;




    shmid = shmget (key, filesize + sizeof (int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror ("MEMMORY GET ERROR");
        exit (-1);
    }
    // printf ("%d\n\n\n", shmid);

/////////////////////////////////////////////////INPUT
    char *arr = (char *) shmat (shmid, NULL, 0);
    if ((int *) arr == (int *) -1) {
        perror ("ATTACH ERROR");
        exit (-1);
    }

    // if (MAX_SIZE >= filesize) {

    //     if (fread (arr, sizeof (char), filesize, in) != filesize) {
    //         perror ("READ ERROR");
    //         exit (-1);
    //     }   
    // }
    // else {

    //     int cnt = 0;

    //     while (cnt != filesize) {

    //         cnt += fread (arr + cnt, sizeof (char), MAX_SIZE, in);

    //         if (cnt > filesize) {
    //             perror ("READ ERROR");
    //             exit (-1);
    //         }
    //     }

    // }


    // for (int i = 0; i < filesize + sizeof (int); ++i) {
    //     printf ("%c", arr[i]);
    // }

    if (shmdt (arr) < 0) {
        perror ("CANT CLEAR");
        exit (-1);
    }

// return 0;

    if (shmctl (shmid, IPC_RMID, NULL) < 0) {
        perror ("CANT DELETE");
        exit (-1);
    }
}
