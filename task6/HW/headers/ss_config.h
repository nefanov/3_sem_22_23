#pragma once

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "ss_log.h"

#define PERMS_SIZE 3
#define PERMS_NUM 3
#define MAX_PERMS 0777

typedef struct Config {
  u_char options;         // 0000 000D; D -- daemon mode
//  char* path;           // Path to the config file
  pid_t pid;              // Tracking pid
} Config;

Config parse_console_args(int argc, const char* argv[]);
char* oct_to_rwx(unsigned short n);
int preparing();
void configure_service(Config* Conf);