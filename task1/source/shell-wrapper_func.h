#ifndef once
#define once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define ARG_MAX 209715 //from getconf ARG_MAX
#define CMD_SIZE 1024

typedef struct Cmd_line
{
    char*** cmd;
    int cmd_counter;
    int* cmd_numbers;
}cmd_line;

void cmd_line_constructor(cmd_line* Bash);
void cmd_line_destructor(cmd_line* Bash);
void print_parced_cmd_line(cmd_line* Bash);
void count_cmd_line(cmd_line* Bash, char* cmd);
void first_parce(cmd_line* Bash, char* cmd);
void second_parce(cmd_line* Bash, char* cmd);
void fill_cmd_line(cmd_line* Bash, char* cmd);
cmd_line* parce_cmd_line(char* cmd, cmd_line* Bash);
void seq_pipe(char* cmd, cmd_line* Bash);
#endif