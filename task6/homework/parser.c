#include "daemon.h"

enum Mods getCMDargs(int argc, char* argv[], pid_t* pid)
{
    int mod = BASH;

    switch (argc)
    {
    case 3:
        if (!strcmp(argv[1], "-b"))
        {
            mod = BASH;
        }
        else if(!strcmp(argv[1], "-d"))
        {
            mod = DAEMON;
        }
        else
        {
            fprintf(stderr, "Wrong Mod:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }

        *pid = atoi(argv[2]);
        if (*pid == 0)
        {
            fprintf(stderr, "Wrong PID:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }
        return mod;
        break;

    case 2:
        *pid = atoi(argv[1]);
        if (*pid == 0)
        {
            fprintf(stderr, "Wrong PID:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }
        break;
    
    default:
        fprintf(stderr, "Wrong arguments:\n\n");
        showExample();
        exit(1);
        break;
    }
}