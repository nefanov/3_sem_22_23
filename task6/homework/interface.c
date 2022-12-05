#include "daemon.h"

void signalHnd()
{
    fprintf(stdout, "Can't sent signal to proccess pid:%d\n", getpid());
    LOG("Trying send signal\n");
}

void registerSignals()
{
    signal(SIGINT,  signalHnd);
    signal(SIGQUIT, signalHnd);
    signal(SIGABRT, signalHnd);
    signal(SIGTRAP, signalHnd);
    signal(SIGTERM, signalHnd);
    signal(SIGCONT, signalHnd);
    signal(SIGTSTP, signalHnd);
}

FILE* prepareInterface()
{
    registerSignals();
    
    FILE* cmdFile = fopen(DAEMON_CMD, "w");

    if (!cmdFile)
    {
        LOG("ERROR CMD FILE:");
        return NULL;
    }

    mode_t old_umask = umask(0);
    
    createDir(DAEMON_DIR);
    createDir(DIFF_DIR);
    createDir(SAMPLE_DATA_DIR);

    return cmdFile;
}


int fifo_init()
{
    createDir("tmp");
    mkfifo(FIFO_DIR, 0666);

    int fifo_fd = -1;
    fprintf(stdout, "Write something in FIFO to start DAEMON\n");
    
    fifo_fd = open(FIFO_DIR, O_RDONLY); 
    fprintf(stdout, "fifo_fd: %d\n", fifo_fd);
    
    return fifo_fd;
}

int getInterface(int fifo_fd, int* arg)
{
    char cmdBuffer[CMD_SIZE] = {};
    size_t readRet = 0;

    readRet = read(fifo_fd, cmdBuffer, CMD_SIZE);
    
    if (readRet == 0 || (readRet == -1 && errno == EAGAIN))
        return NO_CMD;

    int cmd = getCmdFromBuff(cmdBuffer, arg);

    return cmd;
}

int getCmdFromBuff(const char* cmd_buffer, int* arg)
{   
    char cmdStr[CMD_SIZE] = {};
    int ret_val = sscanf(cmd_buffer, "%s %d", cmdStr, arg);

    switch (ret_val)
    {
        case 2:
            if (!strcmp(cmdStr, "setTime"))
            {
                return PERIOD;
            }
            if (!strcmp(cmdStr, "printLog"))
            {
                return PRINTLOGS;
            }
        return ERROR;
        break;

        case 1:
            if (!strcmp(cmdStr, "kill"))
                return KILL_DAEMON;
            else
                return ERROR;
            break;

        default:
            LOG("CMD GETTING FROM BUFF ERROR");
            return NO_CMD;
    }

    return ERROR;
}