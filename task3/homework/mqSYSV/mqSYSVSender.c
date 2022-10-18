#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define DEFAULT_KEY  1337
#define DEFAULT_TYPE 1488
#define MAX_MSG_SIZE 4096


#ifdef SMALL_SIZE
#define SIZE 4096
#endif

#ifdef MEDIUM_SIZE
#define SIZE 104857600
#endif

#ifdef LARGE_SIZE
#define SIZE 2147483648
#endif

typedef struct msgbuf 
{
    long    mtype;
    char    mtext[MAX_MSG_SIZE];
} message_buf;


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
    int msqid = -1;
    int msgflg = IPC_CREAT | 0666;
    key_t key = DEFAULT_KEY;
    message_buf sbuf;
    sbuf.mtype = DEFAULT_TYPE;


    if ((msqid = msgget(key, msgflg)) < 0) 
    {
        perror("msgget");
        exit(1);
    }
    else
        printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    

    FILE* in = fopen("./input.txt", "r");
    if (!in)
    {
        fprintf(stderr,"FILE NOT FOUND\n");
        return 1;
    }


    for (size_t bytes = SIZE; bytes > 0;)
    {
        fread(sbuf.mtext, sizeof(char), MAX_MSG_SIZE, in);
        sleep(0,01);
        if (msgsnd(msqid, &sbuf, MAX_MSG_SIZE, IPC_NOWAIT) < 0)
        {
            perror("msg:");
            exit(1);
        }
        bytes-=MAX_MSG_SIZE;
    }
    
    fclose(in);

    return 0;
}
