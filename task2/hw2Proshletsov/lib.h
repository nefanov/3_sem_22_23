#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

#define BUF_SIZE 4096

typedef struct P_pipe Pipe;
typedef struct Op_table Ops;

typedef enum Parent_child
{
    PARENT,
    CHILD
} Parent_and_child;

/*typedef enum Errors
{
    CALLOC_ERR,

} Errs;

typedef struct Error_hndler
{
    Errs error;
    int line;
} Err_hndl;*/

typedef struct Op_table
{
    size_t (*recieve) (Pipe *self, size_t max_size);
    size_t    (*send) (Pipe *self, size_t amount_to_send, pid_t pid);
} Ops;

typedef struct P_pipe
{
    char *buffer;
    size_t len;

    int sender[2];
    int reciever[2];

    Ops operations;
} Pipe;

Pipe *pipe_construct (size_t size);

size_t recieve_op (Pipe *self, size_t max_size);
size_t    send_op (Pipe *self, size_t amount_to_send, pid_t pid);

void tune_pipes_for_child  (Pipe *pipe);
void tune_pipes_for_parent (Pipe *pipe);

size_t send_file_size (FILE* file, Pipe *pipe);
size_t recieve_file_size (Pipe *pipe);
