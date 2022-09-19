#include <stdio.h>
#include <unistd.h> // fork
#include <sys/types.h> // pid_t
#include <sys/wait.h> // wstatus
#include <MaShell.h>

#define PRINT_ERROR(msg) printf("[ERROR:%d] %s\n", __LINE__, msg);
#define CHILD 0

enum
{
    READ_FD  = 0,
    WRITE_FD = 1
};

int main (int argc, char* argv[], char* envp[])
{
    const char* test_cmd = "/home/matmuh/test_mashell/meow.exe";

    pid_t pid = fork();

    if (pid < 0)
    {
        PRINT_ERROR("Can't fork");
        return -1;
    }
    if (pid == CHILD)
    {
        execve(test_cmd, NULL, NULL);
    }
    else // parent
    {
        int wstatus = 0;
        waitpid(pid, &wstatus, 0);

        printf("[INFO] Child %u exited with code %d\n", pid, WEXITSTATUS(wstatus));
    }


    return 0;    
}