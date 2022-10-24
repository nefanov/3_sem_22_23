#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#include "../test.h"

#define MSG_SIZE 6//24


typedef struct msgbuf {
    long    mtype;
    char    mtext[MSG_SIZE];
} message_buf;

int main(int argc, char* argv[]) {

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key = DEF_KEY;
    message_buf snd_buf;

    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        return -1;
    }
    else 
     printf("msgget: msgget succeeded: msqid = %d\n", msqid);

    snd_buf.mtype = DEF_T;
    
    printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    
    //else  printf("msgget: msgget succeeded: msqid = %d\n", msqid);

    if (argc < 2) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR, FILE WASN'T INPUTED", ESC);
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }


    snd_buf.mtype = DEF_T;
    size_t size = 0;

    while( (size = fread(snd_buf.mtext, sizeof(char), MSG_SIZE, input)) > 0) {
        
        if (msgsnd(msqid, &snd_buf, size, IPC_NOWAIT) < 0) {
        printf ("%d, %ld, %s, %ld\n", msqid, snd_buf.mtype, snd_buf.mtext, size);
        perror("msgsnd");
        return -1;
        }else 
       printf("Message: \"%s\" Sent\n", snd_buf.mtext);

    }

    fclose(input);
    
    /*buf_length = strlen(sbuf.mtext) + 1 ; // данные сообщения по прежнему считаем '\0'-terminated - строкой.
    
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }

   else 
      printf("Message: \"%s\" Sent\n", sbuf.mtext);*/
      
    return 0;
}