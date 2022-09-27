#ifndef TRANSFER_H
#define TRANSFER_H

#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>


typedef struct pPipe Pipe;

typedef struct op_table Ops;

typedef struct op_table  
{
    ssize_t (*rcv) (Pipe *self, int curFd, size_t PartOfBuffer); 
    ssize_t (*send)(Pipe *self, int curFd, size_t PartOfBuffer); 
} Ops;

typedef struct pPipe 
{
    char* dataIn;
    char* dataOut;

    int fdChild[2];
    int fdParent[2]; 

    size_t len;
    size_t lenParts;

    Ops actions;
} Pipe;

void constructPipe(Pipe* self);

ssize_t readDuplex(Pipe* self, int curFd, size_t PartOfBuffer);

ssize_t writeDuplex(Pipe* self, int curFd, size_t PartOfBuffer);

size_t findFileSize(FILE* f);

void run(Pipe* self);

#endif