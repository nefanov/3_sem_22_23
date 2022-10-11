#pragma once

#include <stdlib.h>
#include <errno.h>

#define PRINT_ERROR(msg) printf("[ERROR] In %s function %s line %d:\n\t%s\n", __FILE__, __func__, __LINE__, msg);
#define LOG(...) fprintf(stdout, "[INFO]" __VA_ARGS__); putc('\n', stdout)

#define BUFFER_SIZE 4096

#define CHILD 0
#define ERROR -1
enum
{
    READ_FD  = 0,
    WRITE_FD = 1
};

typedef struct dupPipeOps dupPipeOps;
typedef struct duplexPipe duplexPipe;

struct dupPipeOps
{
    size_t (*direct_interact)(duplexPipe *self, int pid); // rename to direct_interact?
    size_t (*back_interact)(duplexPipe *self, int pid); // back interacts
};

struct duplexPipe
{
    char data[BUFFER_SIZE]; // intermediate buffer
    int fd_direct[2]; // for send() call (parent-->child)
    int fd_back[2]; // for receive() call (child-->parent)
    size_t len; // data length in intermediate buffer
    dupPipeOps acts;

};


duplexPipe* duplexPipeCtor();