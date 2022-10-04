#include <limits.h>
#include <sys/stat.h>

#include "DuplexPipe/DuplexPipe.hpp"
#include "CommonFunctions/CommonFunctions.hpp"
#include "IO/IO.hpp"

#define SIZE_BUF 32768

// const size_t SIZE_BUF = 1048576;

int main() {
    DuplexPipe commCh = {};

    DuplexPipeCtor(&commCh);

    pid_t pid = fork();
    ASSERTED(fork, pid, -1, FORK_FAILED);

    FILE *srcFP = fopen("/mnt/c/Users/olegb/Desktop/data and result/file.txt", "r");
    ASSERTED(fopen, srcFP, NULL, FOPEN_FAILED);

    size_t fSize = GetFSize(srcFP);

    size_t cBlocks = fSize / SIZE_BUF + 1;

    FILE *dstFP = fopen("/mnt/c/Users/olegb/Desktop/data and result/result.txt", "w");
    ASSERTED(fopen, dstFP, NULL, FOPEN_FAILED);

    if (pid == 0) {
        // Child process
        DuplexPipeParentChannelDtor(&commCh);

        int buf[SIZE_BUF] = {0};

        for (size_t i = 0; i < cBlocks; i++) {
        
        DuplexPipeReceive(&commCh, buf, SIZE_BUF, DIRECT);
        // printf("Received from parent\n");

        DuplexPipeSend(&commCh, buf, SIZE_BUF, BACK);
        // printf("Send to parent\n");
        }

        DuplexPipeChildChannelDtor(&commCh);
    } else {
        // Parent process
        DuplexPipeChildChannelDtor(&commCh);

        char buf[SIZE_BUF] = {0};

        for (size_t i = 0; i < cBlocks; i++) {
            Input(srcFP, buf, SIZE_BUF);

            DuplexPipeSend(&commCh, buf, SIZE_BUF, DIRECT);
            
            // printf("Send to child\n");

            DuplexPipeReceive(&commCh, buf, SIZE_BUF, BACK);
            // printf("Received from child\n");
        
            Output(dstFP, buf, SIZE_BUF);
        }

        DuplexPipeParentChannelDtor(&commCh);

        fclose(srcFP);
        fclose(dstFP);

        wait(NULL);            
    }
}