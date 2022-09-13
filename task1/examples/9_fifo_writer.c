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
  printf("Waiting for a reader\n");
  int fd = open(FIFO, O_WRONLY);
  printf("A reader is connected\n");

  int size;
  char buf[4096];

  while (( size = read(0, buf, sizeof(buf)-1)) > 0) {
    buf[size] = '\0';
    printf("Send: %s", buf);
    write(fd, buf, size);
  }

  return 0;
}
