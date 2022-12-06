#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static char child_stack[1048576];

static int child_fn() {
  printf("PID: %ld\n", (long)getpid());
  pid_t pid = fork();
  if (pid==0) {
	printf("PID: %ld, sleeping 5 sec\n", (long)getpid());
	pid = fork();
	if (pid == 0) {
		printf("PID: %ld, PPID: %ld,  sleeping 6 sec\n", (long)getpid(), (long)getppid());
       		sleep(6);
		printf("PID: %ld, PPID: %ld, sleeping another 6 sec\n", (long)getpid(), (long)getppid());
                sleep(6);
	}
	sleep(5);
  } else {
	  sleep(15);
  }
  return 0;
}

int main() {
  pid_t child_pid = clone(child_fn, child_stack+1048576, CLONE_NEWPID | SIGCHLD, NULL);
  printf("clone() = %ld\n", (long)child_pid);

  waitpid(child_pid, NULL, 0);
  return 0;
}

