#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "../common.h"

#define BUF_SIZE 4096

const long KEY = 0xABCD;
const long TYPE = 0xDCBA;

typedef struct msgbuf
{
    long type;
    char data [BUF_SIZE];
} msgbuf;

int main ()
{

    key_t key = KEY;

    msgbuf read_buf;
    read_buf.type = TYPE;

    int  queue_id = -1;
    if ((queue_id = msgget (key, 0666)) < 0)
    {
        perror ("msgget error: ");
        return 1;
    }

    //FILE* output = fopen (argv[1], "w");

    for (size_t bytes_to_read = SIZE; bytes_to_read > 0; bytes_to_read -= BUF_SIZE)
    {
        if (msgrcv (queue_id, &read_buf, BUF_SIZE, TYPE, 0) < 0)
        {
            perror ("Message recieved: ");
            return 1;
        }
    }

    printf ("FINISHED \n");

    return 0;
}