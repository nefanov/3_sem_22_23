#ifndef _SOLVE_H
#define _SOLVE_H

#include "parsing.hpp"
#include <unistd.h>
#include <sys/wait.h> 

void exec_pipes(struct command_line);

void solve(char *str);

#endif