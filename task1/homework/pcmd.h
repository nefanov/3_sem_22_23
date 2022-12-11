#ifndef PCMD_H
#define PCMD_H

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "tokenizator.h"

void run(struct CmdTokens* s_commads);



#endif