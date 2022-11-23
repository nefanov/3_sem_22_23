
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <LogDaemon.h>

void register_signals();

void skeleton_daemon()
{
    pid_t pid;
    /* Fork off the parent process */
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
     /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
   register_signals();
    /* Fork off for the second time*/
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
}

void register_signals()
{
    /* Ignore all signals */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGABRT, SIG_IGN);
    signal(SIGTRAP, SIG_IGN);
    signal(SIGKILL, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGCONT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}
