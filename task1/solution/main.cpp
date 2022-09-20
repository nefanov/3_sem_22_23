#include "io.hpp"
#include "solve.hpp"
#include <unistd.h>
#include <sys/wait.h> 

void seq_pipe(char ***cmd)
{
  int   p[2];
  pid_t pid;
  int   fd_in = 0;
  int   i = 0;

  while (cmd[i] != NULL) {
    pipe(p);
    printf("New used pipe descriptors: %d %d\n",p[0],p[1]);
    printf("Input descriptor for current child process: %d\n", fd_in);
    if ((pid = fork()) == -1) {
          exit(1);
    } else if (pid == 0) {
        if (i > 0)
          dup2(fd_in, 0); //stdin <- read from fd_in (dup / dup2(int oldfd, int newfd) newfd <--copy( oldfd ) )
        if (cmd[i+1] != NULL)
          dup2(p[1], 1); //stdout -> write to pipe
        close(p[0]);
        execvp((cmd)[i][0], cmd[i]);
        exit(2);
    } else {
      wait(NULL);
      close(p[1]);
      if (i>0)
        close(fd_in); // old pipe from previous step is not used further, and can be destroyed
      fd_in = p[0]; //fd_in <--read from pipe
        i++;
    }
}
  return;
}

int main() {
    char *str = NULL;
    input(&str);    
    command_line cmd = parse(str);

    char ***prsses_p = (char ***) calloc(cmd.n_prsses + 1, sizeof(char **));

    for (size_t i = 0; i < cmd.n_prsses; i++) {
        prsses_p[i] = cmd.prss[i].args;
    }

    prsses_p[cmd.n_prsses] = NULL;

    seq_pipe(prsses_p);

    free(str);
    free(prsses_p);

    for (size_t i = 0; i < cmd.n_prsses; i++) {
        free(cmd.prss[i].args);
    }

    free(cmd.prss);

}