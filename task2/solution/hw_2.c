#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

typedef struct pOpTable {
    size_t (*rcv) (Pipe* self);
    size_t (*snd) (Pipe* self);
} OpTable;

typedef struct pPipe {
    char* data_tx, data_rx;
    int fd_rx[2];
    int fd_tx[2];
    size_t length;
    void* actions;
} Pipe;

Pipe* PipeConstructor(size_t buf_size) {
    int p_res_1 = 0, p_res_2 = 0;
    Pipe* res = (Pipe*) malloc(sizeof(Pipe));
    
    res->length = buf_size;
    res->data_tx = (char*) calloc(buf_size, sizeof(char));
    res->data_rx = (char*) calloc(buf_size, sizeof(char));
    int pipefd[2];
    
    p_res_1 = pipe(pipefd);
    res->fd_rx[0] = pipefd[0];
    res->fd_tx[0] = pipefd[1];
    
    p_res_2 = pipe(pipefd);
    res->fd_rx[1] = pipefd[0];
    res->fd_tx[1] = pipefd[1];
    
    if (p_res_1 == -1 || p_res_2 == -1) {
        perror("pipe failure");
        exit(1);
    }

}

size_t recieve(Pipe* self) {
    
}
