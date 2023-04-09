#include "daemon.h"
#include "log.h"


int main(int argc, char* argv[])
{
    system("make clean");
    startLogging();
    pid_t pid  = 0;

    FILE* cmdFile = prepareInterface();
    int mod = getCMDargs(argc, argv, &pid);

    fprintf(stdout, "DaemonLog pid: %d\n", getpid());

    if (mod == DAEMON)
    {
        createDaemon();
    }
    
    char workDir[PATH_MAX_SIZE] = {};

    if (!getCurDir(pid, workDir))
    {
        LOG_FUNC(getCurDir);
        return -1;
    }

    saveDirData(workDir);

    int inotify_fd = 0;
    inotify_fd = inotify_init();
    
    fcntl(inotify_fd, F_SETFL, fcntl (inotify_fd, F_GETFL) | O_NONBLOCK);
    
    int fifo_fd = fifo_init();
    if (fifo_fd == -1) return -1;
   
    fcntl(fifo_fd, F_SETFL, fcntl (inotify_fd, F_GETFL) | O_NONBLOCK);

    inotify_add_watch(inotify_fd, workDir, IN_MODIFY);
    
    int T = 3;
    char cmd_buffer[CMD_SIZE] = {};
    int DAEMON_HP = 1;
    
    for (int i = 0; DAEMON_HP; i++, sleep(T)) 
    {
        LOG("START SAMPLING");
        int arg = -1;
        int cmd = getInterface(fifo_fd, &arg);
        
        switch (cmd)
        {
            case KILL_DAEMON:
                    LOG("KILL DAEMON");
                    DAEMON_HP = 0;
                break;
            
            case PERIOD:
                    logVar("Set period:from %d to %d\n", T, arg);
                    T = arg;
                   break;

            case PRINTLOGS:
                    logVar("Count of last logs: %d\n", arg);
                    sprintf(cmd_buffer, "bash bashDiff.sh %d %s", arg, DAEMON_CMD);
                    system(cmd_buffer);
                break;

            case ERROR:
                    LOG("ERROR");
                break;

            case NO_CMD:
                    LOG("NO CMD\n");
                break;
        }

        pollSamples(inotify_fd, workDir);
        system("bash saveSamples.sh -k1");
    }

    fclose(cmdFile);
    close(fifo_fd);

    return 0;
}