#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "ss_log.h"
#include "ss_errors.h"

#define SLEEP_TIME 10
#define MAPSLINE_FIELD_CNT 6

typedef struct MapsLine {
  char* address;
  char* perms;
  char* offset;
  char* device;
  char* inode;
  char* path;
} MapsLine;

static FILE* maps_file;

MapsLine parse_maps_line(char* line);
int parse_maps(MapsLine** PML);
int PML_diff(MapsLine** Cur, int cnt_cur, MapsLine** Next, int cnt_next);
int free_PML(MapsLine** pPML, int num_lines);
int PML_swap(MapsLine** Cur, int* cnt_cur, MapsLine** Next, int* cnt_next);
int open_maps_file(pid_t tr_pid);
int start_service(pid_t tr_pid);