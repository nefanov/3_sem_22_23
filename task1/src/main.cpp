#include "header.hpp"


#define CHECK_ERR(NO_ERR) do {                                                                      \
                        if (error != NO_ERR) {fprintf (stderr, "ERR: %u LINE: %u\n", error, __LINE__); \
                                     _exit (error);}                                        \
                    }while (0)


int main (void) {

    BUF buf = {};

    CMD_ARR arr = {};

    int p[2] = {};

    pid_t pid = 0;
    
    int fd_in = 0;

    while (1) {

        input_cmd (&arr, &buf);

        //     printf ("CMD NUM %d\n", arr.cmd_num);

        // for (int i = 0; i <= arr.cmd_num; ++i) {
        //     for (int j = 0; j < arr.cmd[i].opt_num; ++j)
        //         puts (arr.cmd[i].opt[j]);
        // }


        for (size_t i = 0; i <= arr.cmd_num; ++i) {

            if (pipe(p) == -1) {
                perror ("PIPE ERROR");
                return PIPE_ERROR;
            }


            if ((pid = fork()) < 0) {
                perror ("FORK ERROR");
                return FORK_ERROR;
            }

            else if (pid == 0) {
                
                int error = 0;

                if (i > 0) {
                    error = dup2 (fd_in, STDIN);
                    CHECK_ERR(STDIN);
                }

                if (i != arr.cmd_num) {
                    error = dup2 (p[1], STDOUT);
                    // fprintf (stderr, "%d\n\n", error);
                    CHECK_ERR(STDOUT);
                }

                error = close(p[0]);
                CHECK_ERR(NO_ERROR);

                error = exec_cmd (arr.cmd[i]);
                CHECK_ERR(NO_ERROR);
                
                _exit (NO_ERROR);
            }
            else {

                int status = 0;

                waitpid (pid, &status, 0);

                if (status != 0) {
                    perror ("ERROR IN CHILD");
                    return status;
                }

                close(p[1]);

                if (i > 0)
                    close(fd_in);

                fd_in = p[0];
            }
        }

        puts ("END OF CMD\n\n\n");

        clear_cmd (&arr);
    }
}
