#include <stdio.h>
#include <MaShell.h>

int main (int argc, char* argv[], char* envp[])
{
    char* path_env = findPathEnvVar(envp);
    if (!path_env) 
    {
        puts("[WARNING] Path variable is not set");
        return 1;
    }

    puts(path_env);

    return 0;    
}