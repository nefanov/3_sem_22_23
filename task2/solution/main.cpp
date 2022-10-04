#include "DuplexPipe/DuplexPipe.hpp"
#include "CommonFunctions/CommonFunctions.hpp"


int main() {
    DuplexPipe commCh = {};

    DuplexPipeCtor(&commCh);

    pid_t pid = fork();
    ASSERTED(fork, pid, -1, FORK_FAILED);

    if (pid == 0) {
        // Child process
        DuplexPipeParentChannelDtor(&commCh);

        int x = 0;
        
        DuplexPipeReceive(&commCh, &x, sizeof(x), DIRECT);
        printf("Received %d\n", x);

        x *= 4;

        DuplexPipeSend(&commCh, &x, sizeof(x), BACK);
        printf("Wrote %d\n", x);

        DuplexPipeChildChannelDtor(&commCh);
    } else {
        // Parent process
        DuplexPipeChildChannelDtor(&commCh);

        int y = 5;

        DuplexPipeSend(&commCh, &y, sizeof(y), DIRECT);
        printf("Wrote %d\n", y);

        DuplexPipeReceive(&commCh, &y, sizeof(y), BACK);
        printf("Result is %d\n", y);

        DuplexPipeParentChannelDtor(&commCh);

        wait(NULL);            
    }
}