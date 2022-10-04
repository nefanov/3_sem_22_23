#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SZ 20000

typedef struct pPipe Pipe;
typedef struct op_table Ops;
 
typedef struct op_table
{
    size_t (*rcv)(Pipe *self, int result, int get); 
    size_t (*snd)(Pipe *self, int result); 
} Ops;
 
typedef struct pPipe
{
    char data_read[BUF_SZ];
    char data_write[BUF_SZ];
    int fd_direct[2];
    int fd_back[2]; 
    size_t len; 
    Ops actions;
} Pipe;


Pipe *constructPipe();
void create_pipe(Pipe *pip);
size_t receive(Pipe *pip, int result, int get);
size_t send(Pipe *pip, int result);
size_t parent_send(Pipe *pip);
size_t child_receive(Pipe *pip);
size_t child_send(Pipe *pip);
size_t parent_receive(Pipe *pip, int get);
void destructPipe(Pipe *pip);


