#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

typedef enum {
    PARENT,
    CHILD,
} proc_st;
typedef struct pOpTable OpTable;
//struct for pipe
typedef struct pPipe {
    proc_st status;
    FILE* read_d, *write_d;
     //data_tx for parent; data_rx for child
    char *buffer;
    // parent: fd_tx[0] --> fd_rx[0]
    //child: fd_rx[1] <-- fd_tx[1]
    int fd_rx[2];
    int fd_tx[2];
    size_t buf_sz, cur_sent;
    OpTable* actions;
} Pipe;

typedef struct pOpTable {
    size_t (*recieve) (Pipe* self);
    size_t (*send) (Pipe* self);
    size_t (*write_f) (Pipe* self);
    size_t (*read_f) (Pipe* self);
    void (*open_f) (Pipe* self, const char* fname_r, const char* fname_w);
    void (*close_f) (Pipe* self);
    void (*set_status) (Pipe* self, proc_st status);
} OpTable;

//---------methods-------------
void dump_buf(Pipe* pipe, const char* name);
void set_status(Pipe* self, proc_st status);
Pipe* PipeConstructor(size_t buf_size, OpTable* operations);
void PipeDestructor(Pipe* pipe);
OpTable* OpTableConstructor(size_t (*recieve) (Pipe*), size_t (*send) (Pipe*), 
                            size_t (*write_f) (Pipe*), size_t (*read_f) (Pipe*),
                            void (*open_f) (Pipe*, const char*, const char*), void (*close_f) (Pipe*), 
                            void (*set_status) (Pipe*, proc_st)) ;

size_t recieve(Pipe* self);
size_t send(Pipe* self);
size_t write_f(Pipe* self);
size_t read_f(Pipe* self);
void open_f(Pipe* self, const char* fname_r, const char* fname_w);
void close_f(Pipe* self);
//-----------------------------