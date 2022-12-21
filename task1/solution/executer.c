#include "executer.h"

void execute(request_t** reqs) {
    int   pipes_d[2];
    pid_t pid;
    int   descr_in = 0, descr_out = 1;
    int   i = 0;

    while (reqs[i] != NULL) {
        pipe(pipes_d);
        
        if ((pid = fork()) == -1) {
            perror("fork error: ");
            exit(1);
        } else if (pid == 0) {
            close(pipes_d[0]);
            
            if (i > 0) dup2(descr_in, 0);
            if (reqs[i+1] != NULL) dup2(pipes_d[1], descr_out);
            
            
            if (execvp(get_command(reqs[i]), get_args(reqs[i])) == -1) {
                perror("exec error: ");
                exit(1);
            }
            
        } else {
            int status;
            wait(&status);
            close(pipes_d[1]);
            if (reqs[i+1] == NULL) {
                if (WIFEXITED(status)) {
                    printf("The process ended with exit code: %d \n", WEXITSTATUS(status));
                }
            }

            if (WIFSIGNALED(status)) {
                printf("The process ended with kill code: %d \n", WTERMSIG(status));
            }
            if (i>0) close(descr_in); 
            descr_in = pipes_d[0];
            i++;
        }
    }
}