#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>


#define DEFAULT_KEY  1337
#define MAX_MSG_SIZE 4096
#define DEFAULT_TYPE 1488


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


int main()
{
    int msqid = -1;
    key_t key = DEFAULT_KEY;
    message_buf  rbuf;
    rbuf.mtype = DEFAULT_TYPE;

    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }


    for (size_t bytes = SIZE; bytes > 0;)
    {
        if (msgrcv(msqid, &rbuf, MAX_MSG_SIZE, DEFAULT_TYPE, 0) < 0)
        {
            perror("msg:");
            exit(1);
        }
        
        bytes-=MAX_MSG_SIZE;
    }

    return 0;
}