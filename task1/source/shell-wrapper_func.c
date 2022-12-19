#include "shell-wrapper_func.h"

void cmd_line_constructor(cmd_line* Bash)
{
    Bash->cmd = NULL;
    Bash-> cmd_counter = 0;
    Bash->cmd_numbers = NULL;
    return;
}

void cmd_line_destructor(cmd_line* Bash)
{
    assert(Bash);

    for(int i = 0; i < Bash->cmd_counter; i++)
    {
        for(int j = 0; j < Bash->cmd_numbers[i]; j++)
            free((Bash->cmd)[i][j]);
        free((Bash->cmd)[i]); 
    }
    
    free(Bash->cmd[Bash->cmd_counter]);
    free(Bash->cmd);
    free(Bash->cmd_numbers);
    return;
}

void print_parced_cmd_line(cmd_line* Bash)
{
    assert(Bash);

    for(int i = 0; i < Bash->cmd_counter; i++)
    {
        printf("[");

        for(int j = 0; j <= Bash->cmd_numbers[i]; j++)
            printf("%s ", Bash->cmd[i][j]);
        
        printf("],");
    }
    printf("[%s]\n", Bash->cmd[Bash->cmd_counter]);
    return;
}

void count_cmd_line(cmd_line* Bash, char* cmd)
{
    assert(Bash);
    assert(cmd);

    int i = 0;
    int j = 0;
    while(1)
    {
        if(cmd[i] == ' ' && cmd[i + 1] != ' ')
            j++;
        else if(cmd[i] == '|')
        {
            Bash->cmd_numbers[Bash->cmd_counter] = j;
            Bash->cmd_counter++;
            j = -1;
        }
        else if(cmd[i] == '\n' || cmd[i] == '\0')
        {
            cmd[i] = '\0';
            Bash->cmd_numbers[Bash->cmd_counter] = j + 1;
            Bash->cmd_counter++;         
            return;
        }
        i++;
    }
    return;
}

void first_parce(cmd_line* Bash, char* cmd)
{
    Bash->cmd = (char***)calloc(Bash->cmd_counter+1, sizeof(char**));
    int i = 0;
    int j = 0;   
    for(i = 0; i < Bash->cmd_counter; i++)
        Bash->cmd[i] = (char**)calloc(Bash->cmd_numbers[i]+1, sizeof(char*)); 
    Bash->cmd[Bash->cmd_counter] = NULL;
    return;
}

void second_parce(cmd_line* Bash, char* cmd)
{ 
    char delim1[] = "|";
    char delim2[] = " ,;\n";
    char** pre_parced_cmd = (char**)calloc(Bash->cmd_counter+1, sizeof(char*));  
  
    int i = 0;
    int j = 0;
    for (char* p = strtok(cmd, delim1); p != NULL; p = strtok(NULL, delim1))
    {   
        pre_parced_cmd[i] = strdup(p);
        i++;
    }
    
    for(i = 0; i < Bash->cmd_counter; i++)
    {
        j = 0;
        for (char* q = strtok(pre_parced_cmd[i], delim2); q != NULL; q = strtok(NULL, delim2))
        {   
            (Bash->cmd)[i][j] = strdup(q);
            j++;
        }
        Bash->cmd[i][j] = NULL;
    }

    for(i = 0; i <= Bash->cmd_counter; i++)
        free(pre_parced_cmd[i]);
    free(pre_parced_cmd);
    return; 
}

void fill_cmd_line(cmd_line* Bash, char* cmd)
{
    assert(Bash);
    assert(cmd);

    first_parce(Bash, cmd);
    second_parce(Bash, cmd);
    return;
}

cmd_line* parce_cmd_line(char* cmd, cmd_line* Bash)
{   
    assert(cmd);
    assert(Bash);

    Bash->cmd_numbers = (int*)calloc(CMD_SIZE, sizeof(int));
    Bash->cmd_counter = 0;
    
    count_cmd_line(Bash, cmd);
    fill_cmd_line(Bash, cmd);
    print_parced_cmd_line(Bash);
    return Bash;
}

void seq_pipe(char* cmd, cmd_line* Bash)
{
    assert(cmd);
    assert(Bash);

    parce_cmd_line(cmd, Bash);
    int   p[2];
    pid_t pid;
    int   fd_in = 0;
    int   i = 0;

    while ((Bash->cmd)[i] != NULL) 
    {
        pipe(p);
        if ((pid = fork()) == -1) 
        {
            printf("fork failed!\n");
            exit(1);
        } 
        else if (pid == 0) 
        {
            if (i > 0)
                dup2(fd_in, 0);
            if ((Bash->cmd)[i+1] != NULL)
                dup2(p[1], 1); 
        close(p[0]);
        execvp((Bash->cmd)[i][0], (Bash->cmd)[i]);
        printf("exec* failed\n");
        exit(2);
        } 
        else 
        {   
            int status = 0;
	        waitpid(pid, &status, 0);
	        printf("Ret code: %d\n", WEXITSTATUS(status));
            close(p[1]);
            if (i>0)
                close(fd_in); 
            fd_in = p[0];
            i++;
        }
    }
    return;
}

