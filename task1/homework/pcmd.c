#include "pcmd.h"


void run(struct CmdTokens* s_commands)
{
    // example /bin/ls -ltr . | grep rw
    
    int   pipeFd[2] = {-1, -1};
    pid_t pid       = -1;
    int   fd_in     = -1;

    for (size_t counter = 0; counter < s_commands->pipelineCapacity + 1; counter++)
    {
        pipe(pipeFd);

        if ((pid = fork()) == -1)
        {
            fprintf(stderr, "ERROR:Unable to create process. A system-imposed limit on the number of threads was encountered\n");
        }
        else if (pid == 0)
        {
            if (counter > 0)
                dup2(fd_in, 0); //stdin

            if (counter != s_commands->pipelineCapacity) dup2(pipeFd[1], 1); //stdout

            close(pipeFd[0]);

            if (execvp(s_commands->flags[counter][0], s_commands->flags[counter]) == - 1)
            {
                fprintf(stderr, "ERROR: execvp error, check errno; Command not found.\n");
            }

            exit(-1);
        }
        else
        {
            int status = 0;

            waitpid(pid, &status, WNOHANG);

            s_commands->exitStatus[counter] = WEXITSTATUS(status);

            close(pipeFd[1]);

            if (counter > 0) close(fd_in);

            fd_in = pipeFd[0];
        }   
    }    
    return;
}
