#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../test.h"

typedef struct msgbuf {
    long    mtype;
    char    mtext[SIZE];
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


    if (argc < 2) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR, FILE WASN'T INPUTED", ESC);
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR WHILE OPENING FILE", ESC);
        return -1;
    }

    size_t size = 0;

    struct stat code_stat = { };

    if ( fstat ( fileno(input), &code_stat) == -1 ) {
        fprintf(stderr, "%s%s%s\n", RED, "ERROR GETTING FILE SIZE", ESC);
        return -1;
    }

    size_t file_size = (size_t)code_stat.st_size;
    memcpy(snd_buf.mtext, &file_size, sizeof(size_t));
    snd_buf.mtext[sizeof(size_t)] = '\0';
    //printf("%lu\n", file_size);

    if (msgsnd(msqid, &snd_buf, sizeof(size_t)/*strlen(snd_buf.mtext)*/, 0) < 0){  

        perror("msgsnd");
        return -1;

    } 

    printf("%lu\n", file_size);

    while( (size = fread(snd_buf.mtext, sizeof(char), SIZE - 1, input)) > 0) {
        
        snd_buf.mtext[SIZE - 1] = '\0';

        if (msgsnd(msqid, &snd_buf, size, 0) < 0) {
    
        //printf ("%d, %ld, %s, %ld\n", msqid, snd_buf.mtype, snd_buf.mtext, size); 
        perror("msgsnd");
        return -1;
        }

    }

    fclose(input);
      
    return 0;
}