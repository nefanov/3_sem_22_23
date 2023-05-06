#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "../Inc/test_size.h"


typedef struct msgbuf 
{
    long    mtype;
    char    mtext[MASSAGE_SIZE];

} msgbuf;


int main (void)
{

    
    key_t key = KEY;

    msgbuf  buffer = {0};
    buffer.mtype = TYPE;


    int msqid = -2;
    msqid = msgget(key,  IPC_CREAT | 0666);

    FILE* input_file = fopen(FILE_NAME, "rb");

    size_t writed = 0;
    ssize_t ms_size = 0;

    for(writed = 0; writed < FILE_SIZE; writed += MASSAGE_SIZE)
    {
        ms_size = fread(buffer.mtext, sizeof(char), MASSAGE_SIZE, input_file);
        if(msgsnd(msqid, &buffer, ms_size, 0) == -1)
        {
            printf("send error\n");
            exit(1);
        }
    }
    
    fclose(input_file);

    return 0;
}