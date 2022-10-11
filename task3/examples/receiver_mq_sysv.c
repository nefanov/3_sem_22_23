#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#define MSG_SIZE     1024 // < /proc/sys/fs/mqueue/msg_max 


typedef struct msgbuf {
    long    mtype;
    char    mtext[MSG_SIZE];
} message_buf;


int main()
{
    int msqid;
    key_t key;
    message_buf  rbuf;

    key = 10;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    
    if (msgrcv(msqid, &rbuf, MSG_SIZE, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("%s\n", rbuf.mtext);
    return 0;
}
