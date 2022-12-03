#include "daemon.h"
#include "log.h"


int main(int argc, char* argv[])
{
    startLogging();

    pid_t pid  = 0;

    int mod = getCMDargs(argc, argv, &pid);

    
}