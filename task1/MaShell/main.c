#include "MaShell.h"

int main(int argc, char* argv[], char* envp[])
{
    while(1)
    {
        char cmd_buffer[ARG_MAX] = {};

        printf("$");
        fgets(cmd_buffer, ARG_MAX, stdin);

        if (cmd_buffer[0] == 'q' && cmd_buffer[1] == '\n')
        {
            printf("[INFO] Program has successfully finished\n");
            return 0;
        }

        size_t cmd_num = 0;
        token* cmd_array = NULL;
        cmd_array = parse_cmd(cmd_buffer, &cmd_num);

        if (cmd_array == ERROR)
        {
            printf("\n[INFO] Please, carefully reenter command line!\n");
            continue;
        }

        print_commands(cmd_array, cmd_num);

        token *begunok = cmd_array;
        int pipefd[2] = {};
        int tube_in = 0; // is used to transfer data in pipeline

        while (begunok->cmd)
        {
            pipe(pipefd);

            pid_t pid = fork();

            if (pid < 0)
            {
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }
            if (pid == CHILD)
            {
                switch (begunok->status)
                {   
                    case LEFT:
                        if (dup2(tube_in, 0) < 0) PRINT_ERROR(strerror(errno)); // redir input
                                                                                // catch input from previous pipeline element
                        close(tube_in);
                        close(pipefd[READ_FD]);
                        close(pipefd[WRITE_FD]);
                        break;

                    case RIGHT:
                        close(pipefd[READ_FD]);
                        if (dup2(pipefd[WRITE_FD], 1) < 0) PRINT_ERROR(strerror(errno)); // redir output
                        close(pipefd[WRITE_FD]);
                        break;

                    case BOTH:
                        if (dup2(tube_in, 0) < 0) PRINT_ERROR(strerror(errno)); // redir input
                                                                                // catch input from previous pipeline element
                        close(tube_in);

                        close(pipefd[READ_FD]);
                        if (dup2(pipefd[WRITE_FD], 1) < 0) PRINT_ERROR(strerror(errno)); // redir output
                        close(pipefd[WRITE_FD]);
                        break;

                    case NOTH:
                        close(pipefd[READ_FD]);
                        close(pipefd[WRITE_FD]);
                        break;
                
                    default:
                        PRINT_ERROR("Unknown cmd mode");
                }

                if (execvpe(begunok->cmd, NULL, envp) == ERROR)
                {
                    // puts(begunok->cmd);
                    PRINT_ERROR(strerror(errno));
                    return ERROR;
                }
            }
            else // parent
            {
                int wstatus = 0;
                if (waitpid(pid, &wstatus, 0) == ERROR)
                {
                    PRINT_ERROR(strerror(errno));
                    return ERROR;
                }

                switch (begunok->status)
                {   
                    case LEFT:
                        close(tube_in);
                        close(pipefd[READ_FD]);
                        close(pipefd[WRITE_FD]);
                        break;

                    case RIGHT:
                        close(pipefd[WRITE_FD]); // wanna read from process, wanna avoid blocking
                        tube_in = pipefd[READ_FD];
                        break;

                    case BOTH:
                        close(pipefd[WRITE_FD]);
                        tube_in = pipefd[READ_FD];
                        break;

                     case NOTH:
                        close(pipefd[READ_FD]);
                        close(pipefd[WRITE_FD]);
                        break;

                    default:
                        PRINT_ERROR("Unknown cmd mode");
                }

                begunok++;

                printf("[INFO] Child %u exited with code %d\n", pid, WEXITSTATUS(wstatus));
            }
        } 
    }
}

int main1 (int argc, char* argv[], char* envp[])
{
    const char  *test_m = "/home/matmuh/test_mashell/meow.exe",
                *test_n = "/home/matmuh/test_mashell/neighbour.exe",
                *test_g = "/home/matmuh/test_mashell/gav.exe",
                *test_s = "/home/matmuh/test_mashell/say.exe";

    token cmd[] = {{test_m, RIGHT}, {test_n, BOTH}, {test_n, BOTH}, {test_n, LEFT}, {NULL, END}}; // first cmd variant

    token *begunok = cmd;
    int pipefd[2] = {};
    int tube_in = 0; // is used to transfer data in pipeline

    while (begunok->cmd)
    {
        pipe(pipefd);

        pid_t pid = fork();

        if (pid < 0)
        {
            PRINT_ERROR(strerror(errno));
            return ERROR;
        }
        if (pid == CHILD)
        {
            switch (begunok->status)
            {   
                case LEFT:
                    if (dup2(tube_in, 0) < 0) PRINT_ERROR(strerror(errno)); // redir input
                                                                            // catch input from previous pipeline element
                    close(tube_in);
                    close(pipefd[READ_FD]);
                    close(pipefd[WRITE_FD]);
                    break;

                case RIGHT:
                    close(pipefd[READ_FD]);
                    if (dup2(pipefd[WRITE_FD], 1) < 0) PRINT_ERROR(strerror(errno)); // redir output
                    close(pipefd[WRITE_FD]);
                    break;

                case BOTH:
                    if (dup2(tube_in, 0) < 0) PRINT_ERROR(strerror(errno)); // redir input
                                                                            // catch input from previous pipeline element
                    close(tube_in);

                    close(pipefd[READ_FD]);
                    if (dup2(pipefd[WRITE_FD], 1) < 0) PRINT_ERROR(strerror(errno)); // redir output
                    close(pipefd[WRITE_FD]);
                    break;

                default:
                    PRINT_ERROR("Unknown cmd mode");
            }

            if (execvpe(begunok->cmd, NULL, envp) == ERROR)
            {
                // puts(begunok->cmd);
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }
        }
        else // parent
        {
            int wstatus = 0;
            if (waitpid(pid, &wstatus, 0) == ERROR)
            {
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }

            switch (begunok->status)
            {   
                case LEFT:
                    close(tube_in);
                    close(pipefd[READ_FD]);
                    close(pipefd[WRITE_FD]);
                    break;

                case RIGHT:
                    close(pipefd[WRITE_FD]); // wanna read from process, wanna avoid blocking
                    tube_in = pipefd[READ_FD];
                    break;

                case BOTH:
                    close(pipefd[WRITE_FD]);
                    tube_in = pipefd[READ_FD];
                    break;

                default:
                    PRINT_ERROR("Unknown cmd mode");
            }

            begunok++;

            printf("[INFO] Child %u exited with code %d\n", pid, WEXITSTATUS(wstatus));
        }
    }

    return 0;    
}