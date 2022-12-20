#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <assert.h>


const long LAST_MESSAGE     = 255;
const long DEFAULT_MESSAGE  = 228;

// #define FIRST
#define SECOND
// #define THIRD

#if defined FIRST
    const int MAX_SIZE = 128;
#elif defined SECOND
    const int MAX_SIZE = 1024;
#elif defined THIRD
    const int MAX_SIZE = 10000;
#endif

size_t get_filesize (FILE *in);

typedef struct Msgbuf  {
    long    type            = DEFAULT_MESSAGE;
    char    text[MAX_SIZE]  = {};
} Msgbuf;


int main (void) {

    key_t key = ftok ("/home/ropho/plus/nef/HA_3sem/task3/code/makefile", 0);
    if (key < 0) {
        perror ("KEY GEN ERROR");
        exit (-1);
    }


    int mq_id = msgget (key, IPC_CREAT | 0666);
    if (mq_id < 0) {
        perror ("MQ GET ERROR");
        exit (-1);
    }



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

/////////////////////////////////////////////////
    int filesize = get_filesize (in);
    printf ("%d\n", filesize);

    int cnt = 0;
    Msgbuf buf {};


    if (MAX_SIZE >= filesize) {
        
        buf.type = DEFAULT_MESSAGE;
        if (fread (buf.text + cnt, sizeof (char), filesize, in) != filesize) {
            perror ("READ ERROR");
            exit (-1);
        }

        if (msgsnd (mq_id, (struct msgbuf *) &buf, filesize, 0) < 0) {
                perror ("SEND ERROR");
                exit (-1);
            }

    }
    else {
        while (cnt < filesize) {
            
            int tmp = fread (buf.text, sizeof (char), MAX_SIZE, in);
            cnt += tmp;
            buf.type = DEFAULT_MESSAGE;

            if (msgsnd (mq_id, (struct msgbuf *) &buf, tmp, 0) < 0) {
                perror ("SEND ERROR 2");
                exit (-1);
            }

        }
    }

    buf.type = LAST_MESSAGE;
    if (msgsnd (mq_id, (struct msgbuf *) &buf, 0, 0) < 0) {
        perror ("SEND ERROR 3");
        exit (-1);
    }

}



size_t get_filesize (FILE *in) {

    assert (in);

    struct stat buff = {};

    fstat (fileno (in), &buff);

    return buff.st_size;
}