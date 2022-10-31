#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char buf[4096] = "QWE";
  int size;
  int fd[2];
  
  for (int i = 0; i < 4096; ++i)
    buf[i] = 1;
    
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

    while(1) {
      // buf[size - 1] = '49'; // the text string data is expected
      // printf ("WRITTEN: %d\n", write(1, buf, 9999));

      printf ("WRITTEN: %d\n", write(fd[1], buf, 999));
      
      // printf ("%d Send: %s\n", buf[size - 1], buf);
    }
  }

  else {
    close(fd[1]);
  
    while((size = read(fd[0], buf, sizeof(buf)-1)) > 0) {
      // printf ("READ: %d\n", size);
      buf[size] = '\0'; // the text string data is expected
      // printf("Received from parent: %s\n", buf);
      puts (buf);
    }
  }
  
  return 0;
}
