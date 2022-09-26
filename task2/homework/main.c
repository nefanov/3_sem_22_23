#include "transfer.h"


int main()
{
    Pipe duplex = {};

    constructPipe(&duplex);

    run(&duplex);

    destructPipe(&duplex);
}