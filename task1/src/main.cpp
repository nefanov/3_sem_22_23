#include "header.hpp"


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

            pipe(p);
                
            if ((pid = fork()) < 0) {
                _exit(1);
            }
            else if (pid == 0) {
            
                if (i > 0)
                    dup2 (fd_in, 0);

                if (i != arr.cmd_num)
                    dup2(p[1], 1);

                close(p[0]);

                exec_cmd (arr.cmd[i]);

                _exit(2);

            }
            else {

                int status = 0;

                waitpid (pid, &status, 0);

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
