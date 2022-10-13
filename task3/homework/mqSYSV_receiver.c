#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>


#define DEFAULT_KEY 1337
#define MAX_MSG_SIZE 4096
#define DEFAULT_TYPE 1488

typedef struct msgbuf {
    long    mtype;
    char    mtext[MAX_MSG_SIZE];
} message_buf;


void dump_data(char* buf, int size)
{
    FILE* out = fopen("output.txt", "wb");
    if (!out)
    {
        fprintf(stderr,"File can't be created\n");
        return;
    }

    fwrite(buf, sizeof(char), size, out);
    fclose(out);
}


int main()
{
    int msqid = -1;
    key_t key = DEFAULT_KEY;
    message_buf  rbuf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (msgrcv(msqid, &rbuf, MAX_MSG_SIZE, DEFAULT_TYPE, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("MSG: \'%s\'\n", rbuf.mtext);

}