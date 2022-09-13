/* Небольшое улучшение примера 10, которое позволяет держать не более 2 пайпов открытыми
Пайп пересоздается n-1 раз, но старые пайпы закрываются за ненадобностью. На одном пайпе я не вижу смысла делать, так как ввод и вывод перемешаются 
Для того, чтобы убедиться, посмотрите вывод используемых дескрипторов (строка 22-23).
У меня это:
New used pipe descriptors: 3 4
Input descriptor for current child process: 0
New used pipe descriptors: 4 5
Input descriptor for current child process: 3
New used pipe descriptors: 3 5
Input descriptor for current child process: 4
New used pipe descriptors: 4 5
Input descriptor for current child process: 3
*/



#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

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

int main()
{
  char *ls[] = {"/bin/ls","-ltr",".", NULL};
  char *grep1[] = {"grep","rw",NULL};
  char *grep2[] = {"grep","1", NULL};
  char *grep3[] = {"grep", "89", NULL};
  char **cmd[] = {ls, grep1, grep2, grep3, NULL};
  seq_pipe(cmd);
}
