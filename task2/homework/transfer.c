#include "transfer.h"

#define $$ fprintf(stdout, "HERE %d\n", __LINE__);


const size_t MAXBUFSIZE = 65536;

size_t findFileSize(FILE* in)
{
    assert(in);

    size_t fileSize = 0;

    fseek (in, 0, SEEK_END);

    fileSize = ftell (in);

    fseek (in, 0, SEEK_SET);

    return (fileSize);
}

void constructPipe(Pipe* self)
{
    assert(self);

    FILE* parent = fopen("file.txt", "r");

    self->len      = findFileSize(parent);
    self->lenParts = self->len / MAXBUFSIZE;

    self->dataIn = calloc(MAXBUFSIZE, sizeof(char));

    if (self->dataIn == NULL) fprintf(stderr, "ERROR: can't calloc memory for data on %d LINE", __LINE__);

    self->actions.rcv   = readDuplex;
    self->actions.send  = writeDuplex; 

    fclose(parent);
}


ssize_t readDuplex(Pipe* self, int curFd, size_t PartOfBuffer)
{
    ssize_t tmp = read(curFd, self->dataIn, MAXBUFSIZE);

    return (tmp);
}


ssize_t writeDuplex(Pipe* self, int curFd, size_t PartOfBuffer)
{
    ssize_t tmp = write(curFd, self->dataIn, MAXBUFSIZE);

    return (tmp);
}

void run(Pipe* self)
{
    pipe(self->fdParent);
    pipe(self->fdChild);
    
    size_t tmpSize = self->len;

    pid_t pid = -1;

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, "ERROR: Can't create new proccess\n");
    }
    else if (pid == 0)
    {
        close(self->fdChild[1]);
        close(self->fdParent[0]);
        //----------------------------------------------------------------

        for (size_t i = 0; i < self->lenParts; i++)
            self->actions.rcv(self, self->fdChild[0], i);

        if ((tmpSize - MAXBUFSIZE * self->lenParts) != 0)
        {
            self->actions.rcv(self, self->fdChild[0], self->lenParts);
        }
        //----------------------------------------------------------------

        for (size_t i = 0; i < self->lenParts; i++)
        {
            self->actions.send(self, self->fdParent[1], i);
            fprintf(stdout, "%lu\n", i);
        }
        if ((tmpSize - MAXBUFSIZE * self->lenParts) != 0)
        {
            self->actions.send(self, self->fdParent[1], self->lenParts);
        }

        //----------------------------------------------------------------
    }
    else
    {
        FILE* parent = fopen("parent.txt", "r");
        if (parent == NULL) fprintf(stderr, "ERROR: can't open file with data on %d LINE", __LINE__);
        //--------------------------------------------------------------------------------------------------------------

        for (size_t i = 0; i < self->lenParts; i++)
        {
            fread(self->dataIn, sizeof(char), MAXBUFSIZE, parent);
            self->actions.send(self, self->fdChild[1], i);
            fprintf(stdout, "%lu\n", i);
        }
        if ((self->len - MAXBUFSIZE * self->lenParts) != 0) 
        {
            fread(self->dataIn, sizeof(char), MAXBUFSIZE, parent);
            self->actions.send(self, self->fdChild[1], self->lenParts);
        }
        //--------------------------------------------------------------------------------------------------------------
        fclose(parent);
        
        FILE* child = fopen("child.txt", "w");
        if (child == NULL) fprintf(stderr, "ERROR: can't open file with data on %d LINE", __LINE__);

        for (size_t i = 0; i < self->lenParts; i++)
        {
            self->actions.rcv(self, self->fdParent[0], i);
            fwrite(self->dataIn, sizeof(char), MAXBUFSIZE, child);
        }
        if ((self->len - MAXBUFSIZE * self->lenParts) != 0) 
        {
            self->actions.rcv(self, self->fdParent[0], self->lenParts);
            fwrite(self->dataIn, sizeof(char), MAXBUFSIZE, child);
        }
        //--------------------------------------------------------------------------------------------------------------
        
        fclose(child);
    }
}