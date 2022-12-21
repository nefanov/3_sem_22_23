#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
загтовка для фонового проекта (daemon)
рекомендуется после первого форк сделатще раз, для безопасности перезвата терминала

*/

/*
Дз 6
1) -i - старт в интерактивном режиме -d в виде демона
2) программа наблюдает за директорией ( мейн)
3) смотрит без рекурсии, выбирает толькрные файлы
4) директории только с человеко-читаемыми, текст
5) наблюдать с помощью inotify, dentry 
6) в этой директории :
6.1) считываем события с регулярными фа
6.2) отлов событий изменения файла (IN_MODIFIED) считаем его diff
6.3) команды через fifo:
 get_current_diff() - изменения в файлах
 set_direct(path) - изменение директории

*/
void do_work(void* args) {
  while (1) {
    printf("Daemon func stub\n");
    sleep(1);
  }
}

int main(int argc, char* argv[]) {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  } else if (pid > 0) {
    //parent
    signal(SIGCHLD,SIG_IGN);
  } else {
    //child
    int sid = setsid();
    if (sid == -1) {
      perror("setsid");
      exit(1);
    }
    close(0);
    close(2);
    umask(0);
    chdir("/");
    do_work(NULL);
  }
  return 0;
}
