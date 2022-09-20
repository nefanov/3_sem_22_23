#include <stdio.h>
#include <unistd.h> // fork
#include <sys/types.h> // pid_t
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <limits.h> // MAX_ARG for cmd
#include <MaShell.h>

#define PRINT_ERROR(msg) printf("[ERROR:%d] %s\n", __LINE__, msg);
#define CHILD 0
#define ERROR -1
#define PERROR_STD "error message"

enum
{
    READ_FD  = 0,
    WRITE_FD = 1
};

int main (int argc, char* argv[], char* envp[])
{
    const char  *test_m = "/home/matmuh/test_mashell/meow.exe",
                *test_n = "/home/matmuh/test_mashell/heighbour.exe",
                *test_g = "/home/matmuh/test_mashell/gav.exe",
                *test_s = "/home/matmuh/test_mashell/say.exe";

    const char* cmd[] = {test_m, test_s, NULL}; // first cmd variant


    const char** begunok = cmd;
    // int pipefd[2] = {};
    // if (pipe(pipefd))
    // {
    //     PRINT_ERROR(strerror(errno));
    //     return ERROR;
    // }

    while (*begunok)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            PRINT_ERROR(strerror(errno));
            return ERROR;
        }
        if (pid == CHILD)
        {
            // close(pipefd[READ_FD]); // Writer

            // char c = 'g';
            // while (c != 'q')
            // {
            //     write(pipefd[WRITE_FD], &c, 1);
            //     c = getchar();
            // }

            // puts("[INFO] Child left writing cycle");

            if (execve(*begunok, NULL, NULL) == ERROR)
            {
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }
        }
        else // parent
        {
        //     close(pipefd[WRITE_FD]); // Reader

        //     char c = 0;
        //     while (c != 'a')
        //     {
        //         read(pipefd[READ_FD], &c, 1);
        //     }
        //     puts("[INFO] Parent left reading cycle");

            int wstatus = 0;
            if (waitpid(pid, &wstatus, 0) == ERROR)
            {
                PRINT_ERROR(strerror(errno));
                return ERROR;
            }

            begunok++;
            printf("[INFO] Child %u exited with code %d\n", pid, WEXITSTATUS(wstatus));
        }
    }

    return 0;    
}