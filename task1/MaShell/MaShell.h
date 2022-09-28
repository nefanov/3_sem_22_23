#pragma once

#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE // execvpe
#include <unistd.h> // fork
#include <sys/types.h> // pid_t
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <limits.h> // MAX_ARG for cmd
#include <ctype.h>
#include <stddef.h>

#define PRINT_ERROR(msg) printf("[ERROR:%d] %s\n", __LINE__, msg);
#define CHILD 0
#define ERROR -1
#define PERROR_STD "error message"

#define ARG_MAX 2097152

enum
{
    READ_FD  = 0,
    WRITE_FD = 1
};

enum
{
    END,   // end of cmd
    NOTH,  // no redir
    LEFT,  // redir from left
    RIGHT, // redir from right
    BOTH   // redir from left & right
};

typedef struct
{
    const char* cmd;
    const char** argv;
    int status;
} token;

token* parse_cmd(char* cmd_buffer, size_t* cmd_num);
void print_commands(token* cmd_array, size_t len);