#include "MaShell.h"

int main(int argc, char* argv[], char* envp[])
{
    token* cmd_array = NULL;

    while(1)
    {
        // Create buffer for user's cmd
        char cmd_buffer[ARG_MAX] = {};
        printf("$$");
        
        if (!fgets(cmd_buffer, ARG_MAX, stdin)) 
            {
            if (feof(stdin)) 
            {
                printf("[INFO] Eof detected\n");
                return 0;
            }

            PRINT_ERROR("fgets cant read anymore");
            return 1;
            }
        putchar('\n');

        // Delete '\n'
        char* end_of_buffer = strchr(cmd_buffer, '\n');

        if (end_of_buffer)
        {
            *end_of_buffer = '\0';
            // printf("Incoreect inpur");
            // exit(1);
        }

        if (cmd_buffer[0] == 'q' && cmd_buffer[1] == '\0')
        {
            printf("[INFO] Program has successfully finished\n");
            return 0;
        }

        size_t cmd_num = 0;
        cmd_array = parse_cmd(cmd_buffer, &cmd_num);

        if (cmd_array == (token*) ERROR)
        {
            printf("\n[INFO] Please, carefully reenter command line!\n");
            continue;
        }

        token *cmd_begunok = cmd_array;
        int pipefd[2] = {};
        int tube_in = 0; // is used to transfer data in pipeline

        while (cmd_begunok->cmd)
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
                printf("executing: %s ", cmd_begunok->cmd);
                print_argv(cmd_begunok->argv + 1);
                putchar('\n');
                putchar('\n');
                
                switch (cmd_begunok->status)
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

                if (execvpe(cmd_begunok->cmd, cmd_begunok->argv, envp) == ERROR)
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

                switch (cmd_begunok->status)
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

                cmd_begunok++;

                printf("\n[INFO] Child %u exited with code %d\n\n", pid, WEXITSTATUS(wstatus));
            }
        } 

    // Free
    token* free_cmd_array_begunok = cmd_array;
    while (free_cmd_array_begunok->cmd) 
        {
        free(free_cmd_array_begunok->argv);
        free_cmd_array_begunok++;
        }

    free(cmd_array);
    }
}
