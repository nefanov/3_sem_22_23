#ifndef DEMON
#define DEMON

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BF_SIZE 4096

int fd, watch_d, T, time = 0;
pid_t my_pid, CHILD_PID;
char *patch_array[4096] = {};
char my_dir[256] = {};
char process_dir[256] = {};
char is_demon = 0;

FILE *fifo;
int log_file;

static void skeleton_daemon()
{
    pid_t pid;
    
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
   
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    
    chdir("/");
    printf("Мой пид: %d\n", getpid());
    
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
 
}

#endif



