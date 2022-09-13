#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FIFO "fifo_example"

int main()
{
  mknod(FIFO, S_IFIFO | 0666, 0);
  printf("Waiting for a writer\n");
  int fd = open(FIFO, O_RDONLY);
  printf("A writer is connected\n");

  int size;
  char buf[4096];

  while (( size = read(fd, buf, sizeof(buf)-1)) > 0) {
    buf[size] = '\0';
    printf("Send: %s", buf);
//    write(1, buf, size);
  }

  return 0;
}
