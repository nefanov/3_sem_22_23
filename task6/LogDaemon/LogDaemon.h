#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> // readlink
#include <string.h> // atoi
#include <sys/inotify.h>
#include <limits.h>
#include <fcntl.h>

#include <logs.h>

#define INFO_DIR "./info"
#define DAEMON_TERMINAL INFO_DIR "/LogDaemon_terminal.txt"
#define FIFO_NAME "/tmp/LogDaemon.fifo"
#define DAEMON_DIR "./daemon_closet"
#define DIFF_DIR "./daemon_diff"
#define SAMPLE_DIR "./daemon_sample"
#define CWD "cwd"
#define PROC_DIR_PATH(pid) "/proc/" #pid

int cp(const char *to, const char *from);
int save_dir_content (const char *dirpath);

#define TIMESTAMP_LEN 255
void get_timestamp (char* timestamp);

// Buffer for 10 inotify events
#define BUF_SIZE (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define CMD_LEN 255

// Sample funcs
int poll_sample(int inotify_fd, const char* work_dir);

// Directory funcs
int create_dir(const char* dir_name);
int create_closets();
int get_work_dir(pid_t pid, char* work_dir);

// Modes

enum MODE 
{
    INTERACT,
    DAEMON
};

enum MODE proc_cmd(int argc, const char* argv[], pid_t* pid_ptr);
void skeleton_daemon();

// for str comparison
#define EQUAL 0
#define STR_EQUAL(str1, str2) (strcmp(str1, str2) == EQUAL)

enum API
{
    SET_T, // "set_t T #"
    SET_PID, // "set_pid pid"
    STOP, // "exit"
    PRINT_K, // "print k"
    RESTORE_K, // "restore k"
    WRONG_CMD,
    NO_CMD
};
#define API_CMD_SIZE 50

// Work with FIFO
int create_fifo();
enum API read_fifo (int fifo_fd, int* arg);