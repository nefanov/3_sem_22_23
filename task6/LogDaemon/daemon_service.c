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
    {
        logs_dtor();
        exit(EXIT_SUCCESS);
    }

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Fork off for the second time*/
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
    {
        logs_dtor();
        exit(EXIT_SUCCESS);
    }
}

void log_signal(int signal)
{
    log("Caught signal %s (%d)", strsignal(signal), signal);
}

#define SAFE_SIG(...)                       \
    do                                      \
    {                                       \
        int ret = __VA_ARGS__;              \
        if (errno == EINVAL)                \
            LOG_ERROR("cant set signal");   \
    } while (0);

void register_signals()
{
    /* Log signals */
    SAFE_SIG(signal(SIGINT,  log_signal));
    SAFE_SIG(signal(SIGQUIT, log_signal));
    SAFE_SIG(signal(SIGABRT, log_signal));
    SAFE_SIG(signal(SIGTRAP, log_signal));
    SAFE_SIG(signal(SIGTERM, log_signal));
    SAFE_SIG(signal(SIGCONT, log_signal));
    SAFE_SIG(signal(SIGTSTP, log_signal));
}

#undef SAFE_SIG