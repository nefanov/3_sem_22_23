#pragma once

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