#ifndef _PARSING_H_
#define _PARSING_H_

#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct process {
    char *name = NULL;
    char **args = NULL;
};

struct command_line {
    struct process *prss = NULL;
};

char **get_tokens(char *str);

struct process get_process_info(char *token);

size_t get_n_words(const char *token);

size_t get_n_processes(const char *str);

struct command_line parse(char *str);

#endif
