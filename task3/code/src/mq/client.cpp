#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/shm.h>

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



typedef struct Msgbuf  {
    long    type            = DEFAULT_MESSAGE;
    char    text[MAX_SIZE]  = {};
} Msgbuf;


int main (void) {

    FILE *out = fopen ("files/out", "wb");


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

    Msgbuf buf {};

    int cnt = 0;

    while (1) {

        int recv = msgrcv (mq_id, (struct msgbuf *) &buf, MAX_SIZE, 0, 0);
        if (recv < 0 ) {

            perror ("RECEIVER ERROR");
            exit (-1);
        }
        
        cnt += recv;

        if (buf.type == LAST_MESSAGE) {
            break;
        }

        // fprintf (stderr, "%s\n", buf.text);
            // cnt += fread (buf.text + cnt, sizeof (char), MAX_SIZE, in);

        fwrite (buf.text, sizeof (char), recv, out);
    }


    printf ("%d\n", cnt);

    if (msgctl (mq_id, IPC_RMID, (struct msqid_ds *) NULL) < 0) {
        perror ("ERROR IN DELETE MQ");
        exit (-1);
    }


    fclose (out);
}
