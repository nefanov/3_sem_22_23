#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define NAME "sharedmemory"
#define SIZE 14
#define END_SIG SIGCHLD
#define WRITE_SIG SIGUSR1
#define READ_SIG SIGUSR2

#define OUT_NAME "out.txt"
#define LOG_NAME "log.txt"

// global variables for flags
int write_flag = 1;
int read_flag = 0;
int end_child_flag = 0;
int end_parent_flag = 0;

//------------------------------------------

void write_log(const char *process, const char *action, int n_dat) {
  FILE *fd = fopen(LOG_NAME, "a+");
  assert(fd != NULL);
  fprintf(fd, "[%s] %s: %d symbols\n", process, action, n_dat);
  fclose(fd);
}

void write_sig_handler(int signo, siginfo_t *info, void *context) {
  write_flag = 1;
}

void read_sig_handler(int signo, siginfo_t *info, void *context) {
  read_flag = 1;
  int writed = 0;
  FILE *fd = fopen(OUT_NAME, "a+");
  assert(fd != NULL);
  writed = fprintf(fd, "%s", (char *)info->si_value.sival_ptr);
  fclose(fd);
  write_log("child ", "read", writed);
}

void end_sig_handler(int signo, siginfo_t *info, void *context)
{
  end_parent_flag = 1;
}

void child_end_handler(int signo, siginfo_t *info, void *context) {
  end_child_flag = 1;
}

int main(int argc, char* argv[]) {
  int shm_fd = 0;
  assert(argc == 2);
  char* in_name = argv[1];
  //open shared memory
  void *ptr;
  shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, SIZE);
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  memset(ptr, 0, SIZE);
  if (ptr == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  // forking
  union sigval val;

  pid_t parent_pid = getpid();
  pid_t child_pid = fork();

  if (child_pid == -1) {
    perror("fork");
    exit(1);
  }
  else if (child_pid > 0) { 
    //parent process
    FILE* read_d = fopen(in_name, "r");
    assert(read_d != NULL);
    
    size_t size = 0;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, END_SIG);
    sigaddset(&mask, WRITE_SIG);
    struct sigaction sa_end, sa_write;
    sa_end.sa_sigaction = end_sig_handler;
    sa_end.sa_mask = mask;
    sa_end.sa_flags = SA_SIGINFO;
    sa_write.sa_sigaction = write_sig_handler;
    sa_write.sa_mask = mask;
    sa_write.sa_flags = SA_SIGINFO;
    sigaction(END_SIG, &sa_end, NULL);
    sigaction(WRITE_SIG, &sa_write, NULL);
    
    int writed = 1;
    while (writed > 0) {

      if (write_flag == 1) {

        if ((writed = fread((char*)(ptr + size), sizeof(char), SIZE - size - 1, read_d)) > 0) {
          
          val.sival_ptr = ptr + size;
          write_flag = 0;
          write_log("parent", "write", writed);
          size += writed;
          if (size >= SIZE - 1)
            size = 0;
          sigqueue(child_pid, READ_SIG, val);
        }
      }

      if (writed <= 0) {

        FILE* log = fopen(LOG_NAME, "a+");
        fprintf(log, "parent ended\n");
        fclose(log);
        end_child_flag = 1;
        end_parent_flag = 0;
        write_flag = 0;
        int status = 0;
        kill(child_pid, END_SIG);
        waitpid(child_pid, &status, 0);
        return 0;
      }
    }
    kill(child_pid, END_SIG);
  }
  else { 
    //child process
    //closing default descriptors
    close(0);
    close(1);
    close(2);
    
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, END_SIG);
    sigaddset(&mask, READ_SIG);
    struct sigaction sa_end, sa_read;
    sa_end.sa_sigaction = child_end_handler;
    sa_end.sa_mask = mask;
    sa_end.sa_flags = SA_SIGINFO;
    sa_read.sa_sigaction = read_sig_handler;
    sa_read.sa_mask = mask;
    sa_read.sa_flags = SA_SIGINFO;
    sigaction(END_SIG, &sa_end, NULL);
    sigaction(READ_SIG, &sa_read, NULL);

    while (1) {

      if (read_flag == 1) {

        read_flag = 0;
        memset(ptr, 0, SIZE);
        kill(parent_pid, WRITE_SIG);
      }

      if (end_child_flag == 1) {

        FILE* log = fopen(LOG_NAME, "a+");
        fprintf(log, "child ended\n");
        fclose(log);
        shm_unlink(NAME);
        munmap(ptr, SIZE);
        kill(parent_pid, SIGCHLD);
        return 0;
      }
    }
  }

  return 0;
}