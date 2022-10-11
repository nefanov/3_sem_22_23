#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

typedef enum {
    PARENT,
    CHILD,
} proc_st;
typedef struct pOpTable OpTable;

typedef struct pPipe {
    proc_st status;
    FILE* read_d, *write_d;
     //data_tx for parent; data_rx for child
    char *data_tx, *data_rx;
    // parent: fd_tx[0] --> fd_rx[0]
    //child: fd_rx[1] <-- fd_tx[1]
    int fd_rx[2];
    int fd_tx[2];
    size_t buf_sz, cur_sent;
    OpTable* actions;
} Pipe;
//--------------------------------------------- methods
size_t recieve(Pipe* self) {
    size_t res;
    if (self->status == PARENT) {
        res = read(self->fd_rx[1], self->data_tx, self->cur_sent);
    } else if (self->status == CHILD) {
        res = read(self->fd_rx[0], self->data_rx, self->cur_sent);
    } else {
        fprintf(stderr, "Unkown struct status");
        exit(1);
    }
    self->cur_sent = res;
    return res;
}

size_t send(Pipe* self) {
    size_t res = 0;
    if (self->status == PARENT) {
        res = write(self->fd_tx[0], self->data_tx, self->cur_sent);
    } else if (self->status == CHILD) {
        res = write(self->fd_tx[1], self->data_rx, self->cur_sent);
    } else {
        fprintf(stderr, "Unkown struct status\n");
        exit(1);
    }
    self->cur_sent = res;
    return res;
}

size_t write_f(Pipe* self) {
    size_t res;
    assert(self->write_d != NULL);
    res = fwrite(self->data_tx, sizeof(char), self->cur_sent, self->write_d);
    return res;
}

size_t read_f(Pipe* self) {
    size_t res;
    assert(self->read_d != NULL);
    res = fread(self->data_tx, sizeof(char), self->buf_sz, self->read_d);
    self->cur_sent = res;
    return res;
}

void open_f(Pipe* self, const char* fname_r, const char* fname_w) {
    self->read_d = fopen(fname_r, "r");
    if (self->read_d == NULL) {
        perror("File for read opening error");
        exit(1);
    }
    
    self->write_d = fopen(fname_w, "w");
    if (self->write_d == NULL) {
        perror("File for write opening error");
        exit(1);
    }
    

}

void close_f(Pipe* self) {
    if (self->write_d != NULL) fclose(self->write_d);
    if (self->read_d != NULL) fclose(self->read_d);
}

//---------------------------------------------------------------
typedef struct pOpTable {
    size_t (*recieve) (Pipe* self);
    size_t (*send) (Pipe* self);
    size_t (*write_f) (Pipe* self);
    size_t (*read_f) (Pipe* self);
    void (*open_f) (Pipe* self, const char* fname_r, const char* fname_w);
    void (*close_f) (Pipe* self);
} OpTable;

OpTable* OpTableConstructor(size_t (*recieve) (Pipe*), size_t (*send) (Pipe*), 
                            size_t (*write_f) (Pipe*), size_t (*read_f) (Pipe*),
                            void (*open_f) (Pipe*, const char*, const char*), void (*close_f) (Pipe*)) {
    
    OpTable* res = (OpTable*)malloc(sizeof(OpTable));
    assert(res != NULL);
    res->recieve = recieve;
    res->send = send;
    res->write_f = write_f;
    res->read_f = read_f;
    res->open_f = open_f;
    res->close_f = close_f;
    return res;
}


Pipe* PipeConstructor(size_t buf_size, OpTable* operations) {
    int p_res_1 = 0, p_res_2 = 0;
    Pipe* res = (Pipe*) malloc(sizeof(Pipe));
    
    res->buf_sz = buf_size;
    res->cur_sent = buf_size;
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
    assert(operations != NULL);
    res->actions = (void*) operations;

    res->write_d = NULL;
    res->read_d = NULL;
    return res;

}

void PipeDestructor(Pipe* pipe) {
    free(pipe->data_tx); free(pipe->data_rx);
    pipe->actions->close_f(pipe);
    free(pipe->actions);
    free(pipe);

}

int main(int argc, char *argv[]) {
    char* read_name = "data.txt";
    char* write_name = "out.txt";
    printf("%d\n", argc);
    switch (argc) {
        case 1:
            printf("You didn`t typed fnames, set default\n");
            break;
        case 3:
            read_name = argv[1];
            write_name = argv[2];
            break;
        default:
            printf("Unkown type of arguments\n");
            exit(1);
    }
    OpTable* operations = OpTableConstructor(recieve, send, write_f, read_f, open_f, close_f);
    Pipe* pipe = PipeConstructor(2048, operations);
    pipe->actions->open_f(pipe, read_name, write_name);
    PipeDestructor(pipe);
    return 0;
}
