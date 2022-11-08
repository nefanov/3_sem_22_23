#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include "../common.h"

const long KEY = 0xABCD;
const long TYPE = 0xDCBA;

typedef struct msgbuf
{
    long type;
    char* data [BUF_SIZE];
} msgbuf;

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Wrong arguments\n");
        return 1;
    }

    key_t key = KEY;

    msgbuf send_buf;
    send_buf.type = TYPE;

    int queue_id = -1;

    if ((queue_id = msgget (key, IPC_CREAT | 0666)) < 0)
    {
        perror ("msgget err");
        return 1;
    }
    else printf ("Connected to queue sucessfully\n");

    FILE* input = fopen (argv[1], "r");
    if (input == NULL)
    {
        fprintf (stderr, "file open error\n");
        return 1;
    }

    for (size_t bytes_to_send = SIZE; bytes_to_send > 0; bytes_to_send -= BUF_SIZE)
    {        
        fread(send_buf.data, sizeof(char), BUF_SIZE, input);
        
        if (msgsnd (queue_id, &send_buf, BUF_SIZE, 0) < 0)
        {
            perror("msg in seder: \n");
            msgctl (queue_id, IPC_RMID, NULL);
            exit(1);
        }
    }
    
    fclose(input);

    return 0;
}