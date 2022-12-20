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

    FILE* output_file = fopen("../test_output", "wb");

    size_t readed = 0;
    ssize_t ms_size = 0;

    for(readed = 0; readed < FILE_SIZE; readed += MASSAGE_SIZE)
    {
        if((ms_size = msgrcv(msqid, &buffer, MASSAGE_SIZE, TYPE, 0)) == -1)
        {
            printf("recive_error\n");
            exit(1);
        }
        fwrite(buffer.mtext, sizeof(char), ms_size, output_file);
    }

    fclose(output_file);

    return 0;
    
}