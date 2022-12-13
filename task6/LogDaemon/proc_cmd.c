#include <LogDaemon.h>

static void show_help()
{
    printf("Wrong cmd args format!\n"
        "Hint:\n"
        "$ ./LogDaemon <mode> <pid>\n"
        "where <mode>:\n"
        "\t-i: interactive\n"
        "\t-d: daemon \n");
}

enum MODE proc_cmd(int argc, const char* argv[], pid_t* pid_ptr)
{
    int mode = INTERACT;

    switch (argc)
    {
        case 3:

            if (strcmp(argv[1], "-i") == EQUAL)
            {
                mode = INTERACT;
            }
            else if (strcmp(argv[1], "-d") == EQUAL)
            {
                mode = DAEMON;
            }
            else
            {
                show_help();
                exit(EXIT_FAILURE);
            }

            errno = 0;
            *pid_ptr = atoi(argv[2]);
            if (errno != 0)
                {
                    printf("[ERROR] Bad pid %s\n", argv[2]);
                }
            return mode;
        
        case 2:
            errno = 0;
            *pid_ptr = atoi(argv[1]);
            if (errno != 0)
                {
                    printf("[ERROR] Bad pid %s\n", argv[1]);
                    show_help();
                }
            return mode;

        case 1:
            printf("[ERROR] Please specify pid and mode\n");
            show_help();
            exit(EXIT_FAILURE);
    }
}
