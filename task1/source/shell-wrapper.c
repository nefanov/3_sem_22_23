#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define CMDLINESIZE 4096
#define CMDSIZE 128

typedef struct cmd_line
{
    char*** cmd;
    int cmd_count;
    int* cmds_num;
}cmdline;

void free_cmdline(cmdline* Arg)
{
    for(int i = 0; i < Arg->cmd_count; i++)
    {
        for(int j = 0; j < (Arg->cmds_num)[i]; i++)
        {
            free((Arg->cmd)[i][j]);
        }
        free((Arg->cmd)[i]);
    }
}

void print_cmd(char*** arg, int cmd_num, int* cmds_num)
{
    for(int i = 0; i <= cmd_num; i++)
    {
        printf("[");
        for(int j = 0; j <= cmds_num[i]; j++)
        {
            printf("%s ", arg[i][j]);
        }
        printf("] ");
    }
    printf("\n");
}

cmdline* parce_cmd(char* cmd)
{   
    assert(cmd);

    int i = 0;   
    int j = 0;
    int cmd_counter = 0;
    int* cmds_num = (int*)calloc(CMDSIZE, sizeof(int));

    while(1)
    {
        if(cmd[i] == ' ' && cmd[i + 1] != ' ')
            j++;

        else if(cmd[i] == '|')
        {
            cmds_num[cmd_counter] = j;
            cmd_counter++;
            j = -1;
        }
        else if(cmd[i] == '\n' || cmd[i] == '\0')
        {
            cmd[i] = '\0';
            cmds_num[cmd_counter] = j + 1;
            cmd_counter++;
            break;
        }
        i++;
    }

    cmdline *Arg = (cmdline*)calloc(1, sizeof(cmdline));
    char*** arg = (char***)calloc(cmd_counter+1, sizeof(char**));

    Arg->cmd = arg;
    Arg->cmds_num = cmds_num;
    Arg->cmd_count = cmd_counter;

    for(i = 0; i <= cmd_counter; i++)
        arg[i] = (char**)calloc(cmds_num[i]+1, sizeof(char*));
    
   
    char delim1[] = "|";
    char delim2[] = " ";

    i = 0;
    j = 0;
    char** pre_parced_cmd = (char**)calloc(cmd_counter+1, sizeof(char*));    
    for (char* p = strtok(cmd, delim1); p != NULL; p = strtok(NULL, delim1))
    {   
        pre_parced_cmd[i] = strdup(p);
        i++;
    }

    i = 0;
    int k = 0;
    for(i = 0; i < cmd_counter; i++)
    {
        k = 0;
        for (char* q = strtok(pre_parced_cmd[i], delim2); q != NULL; q = strtok(NULL, delim2))
        {   
            (arg)[i][k] = strdup(q);
            k++;
        }
        arg[i][k] = NULL;
    }
    print_cmd(arg, cmd_counter, cmds_num);
    free(pre_parced_cmd);
    arg[cmd_counter] = NULL;
    return Arg;
}

void seq_pipe(char* cmd)
{
    cmdline* Arg = parce_cmd(cmd);
    int   p[2];
    pid_t pid;
    int   fd_in = 0;
    int   i = 0;

    while ((Arg->cmd)[i] != NULL) 
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
            if ((Arg->cmd)[i+1] != NULL)
                dup2(p[1], 1); 
        close(p[0]);
        execvp((Arg->cmd)[i][0], (Arg->cmd)[i]);
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
    free_cmdline(Arg);
    free(Arg);
    return;
}

int main()
{
    char* cmd = (char*)calloc(CMDLINESIZE, sizeof(char));
    char a;
    while(1) //mb fork bomb 
    {   
        fgets(cmd, CMDLINESIZE, stdin);
        seq_pipe(cmd);
    }
    return 0;
 }
