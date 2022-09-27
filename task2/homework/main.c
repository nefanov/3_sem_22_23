#include "transfer.h"


int main()
{
    Pipe duplex = {};

    constructPipe(&duplex);

    run(&duplex);

    free(duplex.dataIn);
}