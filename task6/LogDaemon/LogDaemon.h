#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> // readlink
#include <string.h> // atoi
#include <sys/inotify.h>
#include <limits.h>
#include <fcntl.h>

#include <logs.h>

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

// Dir funcs
int create_closets();
int get_work_dir(pid_t pid, char* work_dir);

// Daemon
void skeleton_daemon();