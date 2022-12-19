#include "../headers/ss_main.h"

int main(int argc, const char* argv[]) 
{
    Config Conf = parse_console_args(argc, argv);

    printf("Enter PID, which you want to track: ");
    scanf("%d", &Conf.pid);
    printf("\nPID %d received.\n", Conf.pid);
  
    pid_t pid;
    switch(Conf.options & 1) 
    {
        case 0:
            configure_service(&Conf);
            if (start_service(Conf.pid) == -1) 
            {
                create_log("Starting service failed.");
                return 1;
            }
            break;
        case 1:
            pid = fork();
            switch (pid) 
            {
                case -1:
                    perror("fork failed");
                    return 1;
                case 0:
                    configure_service(&Conf);
                    start_service(Conf.pid);
                    break;
                default:
                    printf("Daemon was created.\n");
                    break;
            }
            break;
        default:
            printf("Bad config.\n");
            return 1;
  }
  return 0;
}