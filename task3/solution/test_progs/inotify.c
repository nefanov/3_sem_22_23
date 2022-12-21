#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  char* path_to_dir = argv[1];
  
  int fd = inotify_init();
  int watch_d;
  fcntl(fd, F_SETFL, O_NONBLOCK);
  watch_d = inotify_add_watch(fd, path_to_dir, IN_CREATE | IN_DELETE | IN_MODIFY);
  if (watch_d ==-1) {
    fprintf(stderr, "watch init error\n");
  }
  while (1) {
    int i=0, sz;
    char buf[4096];
    sz = read(fd, buf, 4096);
    while(i < sz) {
      struct inotify_event *event = (struct intoify_event*) &(buf[i]);
      if (event->len) {
        if (event->mask & IN_CREATE) {
          printf("%s: was created\n", event->name);
        } else if (event->mask & IN_DELETE) {
          printf("%s: was deleted\n", event->name);
        }
      }
      i += sizeof(struct inotify_event) + event->len;
    }
  }
  return 0;
}