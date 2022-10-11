#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // pipe

#include <MaDuplex.h>

/* [Description]

The target of this project is to implement duplex pipe using
ordinary pipe. 

Ctor: 
[*] allocate memory for pipeDuplex instance
[*] put pointers to receive/send functions
[*] create 2 pipes for parent-->child and vice versa 
directions
[*] data, len?

Ideas 
1. for parent-->child:

    parent puts message in write fd of 'direct' pipe
    child reads from read fd of 'direct' pipe

2. for child-->parent:

    child puts message in write fd of 'back' pipe
    parent reads from read fd of 'back' pipe

Task:

    Read file with parent, transfer it contetnt to child, from child retransfer it
    to parent, write to file.

i.e:

    [1] read portion from file
    [2] transfer it to child
    [3] retransfer it from child
    [4] write to file

Version roadmap:

0.0:      transfer string from parent to child
0.5:    retransfer string to parent
1.0:      read from file and write to file (mb stdout)

*/

size_t direct_interact(duplexPipe *self, int pid)
{
    ;
}

size_t back_interact(duplexPipe *self, int pid)
{
    if (pid == CHILD)
    {
        printf("Back interact: CHILD\n");
    }
    else
    {
        printf("Back interact: PARENT\n");
    }
}

duplexPipe* duplexPipeCtor()
{
    LOG("Start CTOR");

    duplexPipe* dupPipe = calloc(1, sizeof(duplexPipe));
    if (!dupPipe) PRINT_ERROR("calloc null ptr");

    memset(dupPipe->data, 0, BUFFER_SIZE); // Buffer is statically alloced inside object
    dupPipe->len = 0;

    // Create pipes
    pipe(&dupPipe->fd_direct);
    pipe(&dupPipe->fd_back);

    // Init ops table
    dupPipe->acts.direct_interact = direct_interact;
    dupPipe->acts.back_interact = back_interact;
}

void duplexPipeDtor(duplexPipe* dupPipe, int pid)
{
    LOG("Start DTOR");

    close(dupPipe->fd_direct[READ_FD]);
    close(dupPipe->fd_direct[WRITE_FD]);
    
    close(dupPipe->fd_back[READ_FD]);
    close(dupPipe->fd_back[WRITE_FD]);

    if (pid != CHILD)
    {
        LOG("Dtor PARENT");
        memset(dupPipe, 0, sizeof(duplexPipe));
        free(dupPipe);
    }
    else
    {
        LOG("Dtor CHILD");
    }
}

int main()
{
    duplexPipe* dupPipe = duplexPipeCtor();

    // fork logic
    int pid = fork();

    // while(1)
    // {
        if (pid < 0)
        {
            PRINT_ERROR("fork negative pid");
        }
        else if (pid == CHILD)
        {
            LOG("Enter CHILD");
            dupPipe->acts.direct_interact(dupPipe, pid);
            LOG("Exit CHILD");
        }
        else
        {
            LOG("Enter PARENT");

            int wstatus = 0;
            if (waitpid(pid, &wstatus, 0) == ERROR)
            {
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }

            dupPipe->acts.direct_interact(dupPipe, pid);
        
            LOG("Child %u exited with code %d", pid, WEXITSTATUS(wstatus));
            LOG("Exit PARENT");
        }
    //}

    duplexPipeDtor(dupPipe, pid);
}