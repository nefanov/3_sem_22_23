
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pipe_manager.h"
#include "parser.h"

extern int WEXITSTATUS;

void pipe_manager(comand* com)
{
    
    int pi[2] = {-1, -1};
    pid_t pid = 0;
    int pi_in = -1;
    int status = 0;
    u_int8_t ex_code = 0;
    
    size_t i = 0;
    for(i = 0; i < com->cmass_len; i++)
    {
        pipe(pi);

        if ((pid = fork()) == -1) 
        {
            exit(1);
        }
        else if (pid == 0) 
        {
            if(i > 0)
            {
                dup2(pi_in, 0);
            }

            if(i < com->cmass_len - 1)
            {
                dup2(pi[1], 1);
            }
            close(pi[0]);
            execvp((com->call_m)[i].argv[0], (com->call_m)[i].argv);

            exit(127);
        }
        else
        {
            wait(&status);
            
            ex_code = status >> 8;
            
            if((ex_code != 0) && (ex_code != 127))
            {
                printf("Program:\"%s\" exited with exit code: %d\n", com->call_m[i].argv[0], ex_code);
                return;
            }
            else if(ex_code == 127)
            {
                printf("Can't start \"%s\"\n", com->call_m[i].argv[0]);
                return;
            }


            close(pi[1]);
                    
            if(i > 0)
            {
                close(pi_in);
            }
            pi_in = pi[0];
        }
    }

    return;

}