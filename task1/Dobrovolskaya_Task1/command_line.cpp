#include "main.h"


command_line* create_command_line(char *string)
{
    char **progs;
    command_line *line;
    line = (command_line *)malloc(sizeof(command_line));

    int count = count_programms(string);
    line->count_programms = count;

    if (count != 1)
    {
        progs = parse(string);
    }
    else
    {
        progs = (char **)malloc(sizeof(char *));
        progs[0] = string;
    }
    line->progs = (programma **)malloc(count * sizeof(programma*));

    for (int i = 0; i < count; i++)
    {
        (line->progs)[i] = (programma*)malloc(sizeof(programma));
        (line->progs)[i]->args_array = parse(progs[i]);
        (line->progs)[i]->name = ((line->progs)[i]->args_array)[0];
    }

    return line;
}


void call_progs(command_line *line)
{
    pid_t pid;
    int fd[2], in, child_status;

    for (int i = 0; i < line->count_programms; i++)
    {
        if (pipe(fd) < 0)
        {
            perror("pipe");
            _exit(-1);
        }

        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            if (i != 0)
            {
                dup2(in, 0); 
            }

            if (i != line->count_programms - 1)
            {
                dup2(fd[1], 1); 
            }

            close(fd[0]);

            if (execvp((line->progs)[i]->name, (line->progs)[i]->args_array) < 0)
            {
                perror("execvp");
            }

            exit(1);
        }
        else
        {
            wait(&child_status);

            if (WIFEXITED(child_status))
            {
                printf("\nExit code %d\n", WEXITSTATUS(child_status));
            }
            else
            {
                printf("\nChild process exited abnormally\n");
            }

            close(fd[1]);

            if (i != 0)
            {
                close(in); 
            }
            
            in = fd[0];
        }
    }

}