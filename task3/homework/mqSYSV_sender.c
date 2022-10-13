#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define DEFAULT_KEY 1337
#define DEFAULT_TYPE 1488
#define MAX_MSG_SIZE 4096

enum errors
{
    OVERFLOW_MSG_ERR = -2,
    FILE_ERR = -1,
    OK = 0
};

typedef struct msgbuf {
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

int initMsgBuf(struct msgbuf* sbuf, size_t* buf_length)
{
    sbuf->mtype = DEFAULT_TYPE;

    FILE* in = NULL;
    if (in = fopen("input.txt", "rb"))
    {
        *buf_length = findFileSize(in);

        if (*buf_length >= MAX_MSG_SIZE) return OVERFLOW_MSG_ERR;

        fread(sbuf->mtext, sizeof(char), *buf_length, in);
        sbuf->mtext[*buf_length] = '\0';
        
        fclose(in);

        return OK;
    }
    else
    {
        fprintf(stderr, "File not found\n");
        return FILE_ERR;
    }
}


int main()
{
    int msqid = -1;
    int msgflg = IPC_CREAT | 0666;
    key_t key = DEFAULT_KEY;
    message_buf sbuf;
    size_t buf_length = 0;

    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    }
    else  
        printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    

    int err = initMsgBuf(&sbuf, &buf_length);

    if (err == OVERFLOW_MSG_ERR)
    {
        fprintf(stderr, "MESSAGE TOO BIG\n");
        return OVERFLOW_MSG_ERR;
    }
    if (err == FILE_ERR)
    {
        return FILE_ERR;
    }

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
        printf ("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }

    printf("MSG %s \n", sbuf.mtext);
        
    return 0;
}
