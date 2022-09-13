#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char buf[4096];
  int size;
  int fd[2];
  
  if (pipe(fd) < 0) {
    printf("Pipe creation is failed!\n");
    return -1;
  }
  
  const pid_t pid = fork();
  if (pid < 0) {
    printf("fork failed!\n");
    return -1;
  }
  
  if (pid) { //parent transmits the data to child
    close(fd[0]);
    while((size = read(0, buf, sizeof(buf)-1)) > 0) {
      buf[size] = 0; // the text string data is expected
      write(fd[1], buf, size);
      printf("Send: %s\n", buf);
    }
  }
  else {
    close(fd[1]);
    while((size = read(fd[0], buf, sizeof(buf)-1)) > 0) {
      buf[size] = '\0'; // the text string data is expected
      printf("Received from parent: %s\n", buf);
    }
  }
  
  return 0;
}
