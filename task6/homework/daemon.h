#ifndef DAEMON_H
#define DAEMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include "log.h"
#include <unistd.h>

#define _GNU_SOURCE


enum Mods getCMDargs(int argc, char* argv[], pid_t* pid);
char* getCurDir(pid_t pid, char* curDir);
int   saveDirData(const char* curDir);
int cp(const char *to, const char *from);
int createDir(const char* dir_name);

void createDaemon();

void registerSignals();
FILE* prepareInterface();
int fifo_init();
int getCmdFromBuff(const char* cmd_buffer, int* arg);
int getInterface(int fifo_fd, int* arg);

int pollSamples(int inotify_fd, const char* work_dir);
void create_diff (const char* observed_file_path, const char* file_name);
int is_text_file(const char* file);
void create_sample();
void getTimestamp(char* timestamp);



#define PATH_MAX_SIZE 255
#define CMD_SIZE      4096
#define NOTIFY_SIZE   (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define STAMP_SIZE    255


#define DAEMON_DIR       "./DaemonData"
#define DAEMON_CMD       "DaemonCmd.txt"
#define DIFF_DIR         "./DaemonDiff"
#define FIFO_DIR         "./tmp/LogDaemon.fifo"
#define SAMPLE_DATA_DIR  "./SampleData"

enum Mods
{
    BASH        = 0,
    DAEMON      = 1,
};

enum Interface
{
    PRINTLOGS   = 1336,
    KILL_DAEMON = 1337,
    NO_CMD      = 1338,
    PERIOD      = 1339,
    ERROR       = 1400,
};

#endif