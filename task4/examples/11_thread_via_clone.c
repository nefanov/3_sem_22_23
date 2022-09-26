#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define TIMEOUT 5

/* *********** used flags description from MANual pages (man 2 clone) *************
If CLONE_VM is set, the calling process and the child
              process run in the same memory space.  In particular,
              memory writes performed by the calling process or by the
              child process are also visible in the other process.
              Moreover, any memory mapping or unmapping performed with
              mmap(2) or munmap(2) by the child or calling process also
              affects the other process.
  CLONE_SIGHAND (since Linux 2.0)
              If CLONE_SIGHAND is set, the calling process and the child
              process share the same table of signal handlers.  If the
              calling process or child process calls sigaction(2) to
              change the behavior associated with a signal, the behavior
              is changed in the other process as well.  However, the
              calling process and child processes still have distinct
              signal masks and sets of pending signals.  So, one of them
              may block or unblock signals using sigprocmask(2) without
              affecting the other process.
  CLONE_THREAD (since Linux 2.4.0)
              If CLONE_THREAD is set, the child is placed in the same
              thread group as the calling process.  To make the
              remainder of the discussion of CLONE_THREAD more readable,
              the term "thread" is used to refer to the processes within
              a thread group.
*/

static int child_func(void *arg) {
    printf("Child thread PID=%d TID=%d\n", getpid(),gettid());
    sleep(TIMEOUT);
    return 0;
}

int main(int argc, char **argv) {
    // Allocate stack for child task, then call clone.
    printf("Parent thread PID=%d TID=%d\n", getpid(), gettid());
    const int STACK_SIZE = 65536;
    char *stack = malloc(STACK_SIZE);
    int status;
    if (!stack) {
        perror("malloc");
        exit(1);
    }
    
    if (clone(child_func, stack + STACK_SIZE, CLONE_THREAD|CLONE_SIGHAND|CLONE_VM, NULL) == -1) {
        perror("clone");
        exit(1);
    }

    sleep(TIMEOUT);
    return 0;
}
