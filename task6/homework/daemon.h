#ifndef DAEMON_H
#define DAEMON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "log.h"

enum Mods getCMDargs(int argc, char* argv[], pid_t* pid);


enum Mods
{
    BASH        = 0,
    DAEMON      = 1,
};

#endif