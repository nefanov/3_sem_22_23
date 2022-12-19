#ifndef ONCE
#define ONE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/stat.h>

#define BUF_MAX_SIZE 4096

typedef struct pPipe Pipe;
typedef struct op_table Ops;

typedef struct op_table  {
    size_t (*receive)(Pipe *self); 
    size_t (*send)(Pipe *self); 
} Ops;

typedef struct pPipe {
    char* data_direct;
    //char* data_back; 
    int fd_direct[2]; 
    int fd_back[2]; 
    size_t len;
    Ops actions;
}Pipe;

size_t send(Pipe* main_pipe);

size_t receive(Pipe* main_pipe);

Pipe* construct_pipe(size_t len);

int destruct_pipe(Pipe* main_pipe);

int swap_descriptors(Pipe* main_pipe);

int swap_buffs(Pipe* main_pipe);

size_t get_filesize (FILE* file_from);

int duplex_realize(Pipe* main_pipe, char* inputfile, char* outputfile);

#endif