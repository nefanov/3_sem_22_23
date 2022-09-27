#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct pPipe Pipe;

typedef struct op_table optbl;

typedef struct op_table {
    size_t (*rcv)(Pipe *self);
    size_t (*snd)(Pipe *self);
} optbl;

typedef struct pPipe {
    char * data;
    int fd_tx[2];
    int fd_rx[2];
    size_t len;
    optbl *actions;
} Pipe;

