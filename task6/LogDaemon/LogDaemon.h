#pragma once

#include <logs.h>

#define DAEMON_DIR "./daemon_closet"
#define CWD "cwd"
#define PROC_DIR_PATH(pid) "/proc/" #pid

int cp(const char *to, const char *from);
int save_dir_content (const char *dirpath);