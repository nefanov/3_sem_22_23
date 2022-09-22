#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define TIMEOUT 5

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
