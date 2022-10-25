#pragma once

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // pipe
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define DEBUG
#undef DEBUG

#ifdef DEBUG

    #define PRINT_ERROR(msg) printf("[ERROR] In %s function %s line %d:\n\t%s\n", __FILE__, __func__, __LINE__, msg);
    #define LOG(...) fprintf(stdout, "[INFO]" __VA_ARGS__); putc('\n', stdout)
    #define LOGP(...) logp(__VA_ARGS__)
    #define LOGC(...) logc(__VA_ARGS__)

#else

    #define PRINT_ERROR(msg)
    #define LOG(...)
    #define LOGP(...)
    #define LOGC(...)

#endif

#define BUFFER_SIZE 65536
#define STD_INPUT_NAME "input.txt"
#define STD_OUTPUT_NAME "output.txt"

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
    size_t (*direct_interact)(duplexPipe *self, int pid);
    size_t (*back_interact)(duplexPipe *self, int pid);
};

struct duplexPipe
{
    char data[BUFFER_SIZE]; // intermediate buffer
    int fd_direct[2]; // for send() call (parent-->child)
    int fd_back[2]; // for receive() call (child-->parent)
    size_t len; // data length in intermediate buffer
    
    int input_fd;
    size_t input_size;
    int output_fd;

    dupPipeOps acts;
};


duplexPipe* duplexPipeCtor(const char* output_name);
void duplexPipeDtor(duplexPipe* dupPipe, int pid);
size_t direct_interact(duplexPipe *self, int pid);
size_t back_interact(duplexPipe *self, int pid);

void logp (const char* format, ...);
void logc (const char* format, ...);