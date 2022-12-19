#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define DEF_KEY  2222
#define DEF_TYPE 333
#include "../sizes.h"

typedef struct msgbuf 
{
    long    mtype;
    char    mtext[DEF_SIZE];
} msgbuf;

int main()
{
    key_t key = DEF_KEY;
    
    msgbuf read_buf;
    read_buf.mtype = DEF_TYPE;

    int msqid = -2;
    if((msqid = msgget(key, 0666)) < 0)
    {
        printf("Error! msgget\n");
        exit(1);
    }

    FILE* output = fopen("../output.txt", "w");

    for (size_t bytes = SIZE; bytes > 0; bytes-=DEF_SIZE)
    {
        if (msgrcv(msqid, &read_buf, DEF_SIZE, DEF_TYPE, 0) < 0)
        {
            perror("Message in receiver:");
            exit(1);
        }
        //mb printf stdout
    }
    printf("Received\n");
    return 0;
}   