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
    
    msgbuf send_buf;
    send_buf.mtype = DEF_TYPE;

    int msqid = -2      ;
    if((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        printf("Error! msgget\n");
        exit(1);
    }    
    else printf("msgget success! id: %d \n", msqid);

    FILE* input = fopen("../input.txt", "r");
    if (!input)
    {
        fprintf(stderr,"FILE NOT FOUND\n");
        return 1;
    }

    for (size_t bytes = SIZE; bytes > 0; bytes-=DEF_SIZE)
    {        
        fread(send_buf.mtext, sizeof(char), DEF_SIZE, input);
        
        if (msgsnd(msqid, &send_buf, DEF_SIZE, 0) < 0)
        {
            perror("msg in seder: \n");
            msgctl(msqid, IPC_RMID, NULL);
            exit(1);
        }
    }
    
    fclose(input);

    return 0;
}