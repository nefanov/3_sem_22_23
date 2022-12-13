#include "DuplexPipe.hpp"

int DuplexPipeCtor(DuplexPipe *commCh) {
    int result = pipe(commCh->directFD);
    ASSERTED(pipe, result, -1, PIPE_FAILED);

    result = pipe(commCh->backFD);
    ASSERTED(pipe, result, -1, PIPE_FAILED);

    return 0;
}

int DuplexPipeParentChannelDtor(DuplexPipe *commCh) {
    close(commCh->directFD[1]);
    close(commCh->backFD[0]);

    return 0;
}

int DuplexPipeChildChannelDtor(DuplexPipe *commCh) {
    close(commCh->directFD[0]);
    close(commCh->backFD[1]);

    return 0;
}

int DuplexPipeReceive(DuplexPipe *commCh, void *dstBuf, size_t size, int direction) {
    if (direction == DIRECT) {
        int result = read(commCh->directFD[0], dstBuf, size);
        ASSERTED(read, result, -1, READ_FAILED);
    } else {
        int result = read(commCh->backFD[0], dstBuf, size);
        ASSERTED(read, result, -1, READ_FAILED);
    }    

    return 0;
}

int DuplexPipeSend(DuplexPipe *commCh, void *srcBuf, size_t size, int direction) {
    if (direction == DIRECT) {
        int result = write(commCh->directFD[1], srcBuf, size);
        ASSERTED(write, result, -1, WRITE_FAILED);
    } else {
        int result = write(commCh->backFD[1], srcBuf, size);
        ASSERTED(write, result, -1, WRITE_FAILED);
    }

    return 0;
}

