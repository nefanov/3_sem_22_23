#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#include "../test.h"

typedef struct msgbuf {
    long    mtype;
    char    mtext[SIZE];
} message_buf;


int main()
{

    FILE *out = fopen("../output.txt", "w");
    
    if (!out) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }

    int msqid;
    key_t key;
    message_buf  rbuf;

    key = DEF_KEY;
    rbuf.mtype = DEF_T;
    for(int i = 0; i < sizeof(size_t); i++){

        rbuf.mtext[i] = 0;

    }
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (msgrcv(msqid, &rbuf, SIZE, DEF_T, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    

    size_t file_size = 0;
    memcpy(&file_size, rbuf.mtext, sizeof(size_t));
    int size = 0;
    printf("%lu\n", file_size); 

    while( file_size ){
 
        if ( (size = msgrcv(msqid, &rbuf, SIZE, DEF_T, 0)) < 0) {
            perror("msgrcv");
            exit(1);
        }

        file_size -= (size_t)size;
        fwrite(rbuf.mtext, sizeof(char), size, out);
    }
    fclose(out);
    msgctl(msqid, IPC_RMID, (struct msqid_ds*)&rbuf); 

    return 0; 
}
