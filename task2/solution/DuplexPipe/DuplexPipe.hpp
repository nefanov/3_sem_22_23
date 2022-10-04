#ifndef _DUPLEX_PIPE_H_
#define _DUPLEX_PIPE_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../CommonFunctions/CommonFunctions.hpp"

enum ERROR_LIST {
    PIPE_FAILED = 1,
    FORK_FAILED,
    READ_FAILED,
    WRITE_FAILED,
    DuplexPipeParentChannelDtor_FAILED
};

enum DIRECTION {
    DIRECT,
    BACK
};

struct DuplexPipe {
    int directFD[2] = {};
    int backFD[2] = {};
};

int DuplexPipeCtor(DuplexPipe *commCh);

int DuplexPipeParentChannelDtor(DuplexPipe *commCh);

int DuplexPipeChildChannelDtor(DuplexPipe *commCh);

int DuplexPipeReceive(DuplexPipe *commCh, void *dstBuf, size_t size, int direction);

int DuplexPipeSend(DuplexPipe *commCh, void *srcBuf, size_t size, int direction);

#endif