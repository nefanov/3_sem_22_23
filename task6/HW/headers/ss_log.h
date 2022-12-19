#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ss_start.h"

#define PATH_LEN 256
#define TIME_MARK_LEN 24
#define MSG_LEN (PATH_LEN - TIME_MARK_LEN)

struct MapsLine;
typedef struct MapsLine MapsLine;

static int log_fd;

void print_instruction(const char* cmd);
int start_log();
char* get_time();
void create_log(const char* format, ...);
void print_parsed_maps_line(MapsLine* PML);
void print_diff_PML(MapsLine* Cur, MapsLine* Next);