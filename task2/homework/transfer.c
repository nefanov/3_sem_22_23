#include "transfer.h"


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

    FILE* parent = fopen("parent.txt", "r");

    self->len = findFileSize(parent);

    self->actions.rcv   = readDuplex;
    self->actions.send  = writeDuplex; 

    fclose(parent);
}

void destructPipe(Pipe* self)
{
    free(self->dataIn);
    free(self->dataOut);
}


size_t readDuplex(Pipe* self, int curFd)
{
    return (read(curFd, self->dataIn, self->len));
}


size_t writeDuplex(Pipe* self, int curFd)
{
    return (write(curFd, self->dataIn, self->len));
}

void run(Pipe* self)
{
    pipe(self->fdParent);
    pipe(self->fdChild);
    
    pid_t pid = -1;

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, "ERROR: Can't create new proccess\n");
    }
    else if (pid == 0)
    {
        sleep(1.5);
        self->actions.rcv(self, self->fdChild[0]);
        self->actions.send(self, self->fdParent[1]);
    }
    else
    {
        int status = 0;
        
        self->dataIn = calloc(self->len, sizeof(char));

        if (self->dataIn == NULL) fprintf(stderr, "ERROR: can't calloc memory for data on %d LINE", __LINE__);

        FILE* parent = fopen("parent.txt", "r");

        if (parent == NULL) fprintf(stderr, "ERROR: can't open file with data on %d LINE", __LINE__);

        fread(self->dataIn, sizeof(char), self->len, parent);

        fclose(parent);

        self->actions.send(self, self->fdChild[1]);

        waitpid(pid, &status, 0);


        
    }


}