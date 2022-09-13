/* this example shows how to implement the simplest solution of homework 1. */
#include <unistd.h>
#include <stdio.h>
/* put your headers here */

static void run_cmd(const char *cmd)
{
  const pid_t = fork();
  if (pid < 0) {
    printf("fork failed!\n");
  }
  if (pid) {
    waitpid(pid, NULL,0);
    /* add status check here (use examples) */
    return;
  }
  
  char **args=parse_cmd(cmd); // implement this function by yourself
  execvp(args[0], args);
  printf("exec* failed\n");
}

int main()
{
  while(1) {
    char *cmd;
    // read cmd here
    run_cmd(cmd);
  }
  return 0;
 }
