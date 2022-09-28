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
#include <string.h>

char*** parser(char* cmd, FILE *logfile){

    char* parsed;
    char*** realparsed = (char***)calloc((size_t)(256), (size_t)(sizeof(char***)));
    for(int i = 0; i< 100; i++) realparsed[i] = (char**)calloc((size_t)(256), (size_t)(sizeof(char**)));

    parsed = strtok(cmd, "|");

    for(int i = 0; parsed != NULL; i++){
        fprintf(logfile, "LINE %d:  Ya tut, i = %d\n", __LINE__, i);
        realparsed[i][0] = parsed;
        i++;
        realparsed[i][0] = NULL;
        parsed = strtok(NULL, "|");
    }

    return realparsed;
}

char** second_parser(char* cmd, FILE* logfile){

    char* parsed;
    char** realparsed = (char**)calloc((size_t)(256), (size_t)(sizeof(char**)));
    for(int i = 0; i< 100; i++) realparsed[i] = (char*)calloc((size_t)(256), (size_t)(sizeof(char*)));

    parsed = strtok(cmd, " ");
    int i = 0;
    while(parsed != NULL){
        fprintf(logfile, "LINE %d: SECOND, i = %d, parsed = %s\n", __LINE__, i, parsed);
        realparsed[i] = parsed;
        fprintf(logfile, "LINE %d: realparsed[%d] = %s\n", __LINE__, i, realparsed[i]);
        i++;
        parsed = strtok(NULL, " ");
    }
    realparsed[i] = NULL;

    return realparsed;
}

void seq_pipe(char ***cmd, FILE* logfile)
{
  int   p[2];
  pid_t pid;
  int   fd_in = 0;
  int   i = 0;
  fprintf(logfile, "LINE %d:  **cmd = %s\n", __LINE__, **cmd);


  while (cmd[i] != NULL) {
    fprintf(logfile, "\nLINE %d: cmd[%d] = %s\n", __LINE__, i, cmd[i][0]);
    pipe(p);
    fprintf(logfile, "New used pipe descriptors: %d %d\n",p[0],p[1]);
    fprintf(logfile, "Input descriptor for current child process: %d\n", fd_in);
    if ((pid = fork()) == -1){
      exit(1);

    } else if(pid == 0) {
      fprintf(logfile, "LINE %d:  Child has started work, i = %d\n", __LINE__, i);

      if (i > 0)
      dup2(fd_in, 0); //stdin <- read from fd_in (dup / dup2(int oldfd, int newfd) newfd <--copy( oldfd ) )

      fprintf(logfile, "LINE %d: (*cmd)([i] == [%d]) = %s\n",__LINE__, i, (*cmd)[i]);
      
      if (cmd[i+1] != NULL){
        fprintf(logfile, "\nLINE %d: cmd[i+1] == cmd[%d+1] != NULL => dup(p[1], 1);\n", __LINE__, i);
        dup2(p[1], 1); //stdout -> write to pipe
      }

      close(p[0]);
      fprintf(logfile, "\nLINE %d:  execvp((cmd)[i][0], cmd[i]) = execvp(%s, %s)", __LINE__, (cmd)[i][0], cmd[i]);

      execvp((cmd)[i][0], cmd[i]);
      exit(2);
    } else {
      wait(NULL);
      close(p[1]);

      fprintf(logfile, "LINE %d: child finished work.\n", __LINE__);

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
  FILE *logfile = fopen("logi_moi_logi.txt", "w");
  int k = 0, size = 0;
  char c, buf[4096], str[256];
  char*** realparsed = NULL;
  char** commands[10];
  c = (char)getchar();

  for(; c != '\n'; c = (char)getchar()){
    str[k] = c;
    k++;
  }
  str[k] = '\0';
  fprintf(logfile, "\nS = %s\n", str);

  realparsed = parser(str, logfile);
  for(int i = 0; i < k; i++) fprintf(logfile, "realparsed[%d][0] = %s\n", i, realparsed[i][0]);
  for(int i = 0; i < 5; i++) {
    commands[i] = second_parser(realparsed[2*i][0], logfile);
    fprintf(logfile, "i = %d, commands[%d][0] = %s\n", i, i, commands[i][0]);
    fprintf(logfile, "i = %d, commands[%d][1] = %s\n", i, i, commands[i][1]);
    fprintf(logfile, "i = %d, commands[%d][2] = %s\n", i, i, commands[i][2]);
    }

  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 5; j++) {
      fprintf(logfile, "LINE %d:  commands[%d][%d] = %s\n", __LINE__, i, j, commands[i][j]);
      fprintf(logfile, "(commands[%d][0] == NULL) = %d\n", i, commands[i][0] == NULL);
      if(commands[i][0] == NULL) {
      commands[i] = NULL;
      fprintf(logfile, "\nLINE %d;\n", __LINE__);
      i = 6;
      j = 6;
      }
    }
  }
  seq_pipe(&commands, logfile);
  fclose(logfile);
  return 0;
}
