#include "main.h"


command_line* create_command_line(char *string)
{
    int count;
    char **progs;
    command_line *line;
    line = (command_line *)malloc(sizeof(command_line));
    assert(line);

    count = count_programms(string);
    line->count_programms = count;

    if (count != 1)
    {
        progs = parse(string);
    }
    else
    {
        progs = (char **)malloc(sizeof(char *));
        assert(progs);

        progs[0] = string;
    }

    line->progs = (programma **)malloc(count * sizeof(programma*));
    assert(line->progs);

    for (int num_prog = 0; num_prog < count; num_prog++)
    {
        (line->progs)[num_prog] = (programma*)malloc(sizeof(programma));
        assert((line->progs)[num_prog]);

        (line->progs)[num_prog]->args_array = parse(progs[num_prog]);
        (line->progs)[num_prog]->name = ((line->progs)[num_prog]->args_array)[0];
    }

    return line;
}

int is_it_again(char *string)
{
    int elem;
    elem = 0;

    if (string[0] == '\n')
    {
        return 1;
    }
    
    while(string[elem] == ' ')
    {
        elem++;
        if (string[elem] == '\n')
        {
            return 1;
        }
    }

    return 0;
}


int call_progs(command_line *line)
{
    assert(line);

    pid_t pid;
    int fd[2], in, child_status;

    for (int num_prog = 0; num_prog < line->count_programms; num_prog++)
    {
        if (((line->progs)[num_prog]->name)[0] == 'q')
        {
            return 1;
        }

        if (((line->progs)[num_prog]->name)[0] == '\n')
        {
            return 1;
        }

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
            if (num_prog != 0)
            {
                dup2(in, 0); 
            }

            if (num_prog != line->count_programms - 1)
            {
                dup2(fd[1], 1); 
            }

            close(fd[0]);

            if (execvp((line->progs)[num_prog]->name, (line->progs)[num_prog]->args_array) < 0)
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

            if (num_prog != 0)
            {
                close(in); 
            }
            
            in = fd[0];
        }
    }

    return 0;
}


void destruct_command_line(command_line *line)
{
    for (int num_prog = 0; num_prog < line->count_programms; num_prog++)
    {
        free((line->progs)[num_prog]->args_array);
        free((line->progs)[num_prog]);
    }
    free(line->progs);
}