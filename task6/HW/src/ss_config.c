#include "../headers/ss_config.h"

Config parse_console_args(int argc, const char* argv[]) 
{
    Config Conf;
    Conf.options = 0;   // Interactive mode as default
  
      // if without arguments
    if (argc == 1) 
    {
        printf("Mode: interactive\n");
        return Conf;
    }
  
    int flag;
    opterr = 0;
  
    const char optstring[] = "dh";
    const struct option longopts[] = {{"daemon", 0, NULL, 'd'}, {"help", 0, NULL, 'h'}};

    while ((flag = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) 
    {
        switch (flag) 
        {
            case 'd':
                printf("Mode: daemon\n");
                Conf.options |= 1;
                break;
            case 'h':
                print_instruction(argv[0]);
                exit(0);
            default:
                printf("ERROR: Undefined flag -%c.\n", flag);
                exit(1);
        }
    }
    return Conf;
}

char* oct_to_rwx(unsigned short n) 
{
    int arr_sz = PERMS_SIZE * PERMS_NUM;
    char* perms = calloc(arr_sz, sizeof(char));
    strcpy(perms, "---------");
    
    char flags[PERMS_SIZE] = {'x', 'w', 'r'};
    
    for (int i = 0; i < PERMS_NUM; i++) 
    {
        for (int j = 0; j < PERMS_SIZE; j++) 
        {
            int shift = (i * PERMS_NUM) + j;
            int pos = (arr_sz - 1) - shift;
      
            if ((n >> shift) & 1) 
            {
                perms[pos] = flags[j];
            }
    }
    }
    return perms;
}

int preparing() 
{     
    start_log(); 
    create_log("Configuration has begun.");
  
     // ----- Start preparing -----
    create_log("Preparation has begun.");
  
    // ----- Closing fds -----
    create_log("Closing file descriptors...");
    if (close(fileno(stdin)) == -1 || close(fileno(stdout)) == -1 || close(fileno(stderr)) == -1) 
    {
        // Closing file descriptors fail
        create_log("FAIL: %s", strerror(errno));
        return -1;
    } else {
        // Closing file descriptors succeed
        create_log("SUCCEED");
    }
  
    // ----- Create new session -----
    create_log("Creating new session...");
    pid_t sid = setsid();
    if (sid == -1) 
    {
        // Create new session fail
        create_log("FAIL: %s", strerror(errno));
        return -1;
    } else {
    // Create new session succeed
    create_log("SUCCEED: Session id is %ld", sid);
    }
  
    // ----- Set the file creation permissions -----
    create_log("Setting file creation permissions...");
    umask(0);
    //  char* perms = oct_to_rwx(MAX_PERMS);
    create_log("SUCCEED: Permissions are %04o", MAX_PERMS);
    //  free(perms);
    
    // ----- Change current directory -----
    create_log("Changing current directory...");
    if (chdir("/") == -1) 
    {
        // Changing current directory fail
        create_log("FAIL: %s", strerror(errno));
        return -1;
    } else {
        create_log("SUCCEED: Current directory is \"/\"");
    }
  
    create_log("Starting block all signals...");
  
    sigset_t mask;
    create_log("Creating mask...");
    if (sigfillset(&mask) == -1) {
    create_log("FAIL: %s", strerror(errno));
    return -1;
    } else {
    create_log("SUCCEED");
    }
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) 
    {
        create_log("FAIL: %s", strerror(errno));
        return -1;
    } else {
        create_log("SUCCEED: All signals are blocked.");
    }
  
  // ----- COMPLETED -----
    create_log("Preparation completed successfully.");
    return 0;
}

void configure_service(Config* Conf) 
{
    log_fd = fileno(stdout);
  
     // TODO: parsing Conf->path file and get configuration of the service
  
      // TODO: use daemon mode, if it turned on
    switch(Conf->options & 1) 
    {
        case 0:
            create_log("Configuration has begun.");
            break;
        case 1:
            if (preparing() == -1) 
            {
                open(stdout, O_WRONLY);
                printf("Preparation failed\n");
  
               exit(1);
            }
            break;
    }
  
    create_log("Configuration completed successfully.\n");
}