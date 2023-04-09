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
    int   pipe_fd[2] = {-1, -1};
    pid_t pid   ;
    int   fd_in = 0;
    int   i = 0;

    
  

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
