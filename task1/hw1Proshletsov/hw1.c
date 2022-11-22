

#define RESET "\x1B[0m\033[0m"
#define BOLD "\x1B[1m"
#define RED "\033[0;32m"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

#define $$$ printf("line : %d\n", __LINE__);

int get_amount_of_commands (ssize_t line_size, char *line, char ***cmds_array);
int* parse_seq (char ****cmds, char *line_buf, ssize_t line_size);
char* skip_spaces (char* line);
int parse_single_cmd (char ***cmds, char *line);
int is_line_finished (char *line);

void seq_pipe(char ***cmd)
{
    int   p[2];
    pid_t pid;
    int   fd_in = 0;
    int   i = 0;

    while (cmd[i] != NULL) 
    {
        pipe(p);

        if ((pid = fork()) == -1) 
        {
            exit(1);
        }

        else if (pid == 0) 
        {
            if (i > 0)
                dup2(fd_in, 0); 
            if (cmd[i+1] != NULL)
                dup2(p[1], 1);

            close(p[0]);
            execvp((cmd)[i][0], cmd[i]);
            exit(2);
        } 
        else 
        {
            wait(NULL);
            close(p[1]);
            if (i>0)
                close(fd_in);
    
            fd_in = p[0];
            i++;
        }
    }
    return;
}

int main()
{
    char ****cmd = (char****) calloc (1, sizeof (char***));

    ssize_t line_size = 0;

    int quit = 0;

    while (quit != 1)
    {        
        printf ("%s", RED);
        printf ("%shw1.pseudobash%s$ ", BOLD, RESET);

        char *line = calloc (4096, sizeof (char));
        fgets (line, 4096, stdin);

        int * units_info = parse_seq (cmd, line, 4096);
        seq_pipe(*cmd);                    
        
        for (int i = 0; i < units_info[0] - 1; i++)
            free (*(*cmd + i));
        free (units_info);

        free (*cmd);
        if (strcmp (line, "quit") == 0)
            quit = 1;

        free (line);
    }

    free (cmd);

    return 0;
}

int * parse_seq (char ****cmds, char *line_buf, ssize_t line_size)
{
    const char cmd_delim[] = {'|'};

    ssize_t offset = 0;
    char*** cmd_starts_arr = (char***) calloc (1, sizeof (char**));

    int amount_of_cmds = get_amount_of_commands (line_size, line_buf, cmd_starts_arr);

    *cmds = (char***)calloc (amount_of_cmds + 1, sizeof (char**));
    *(*cmds + amount_of_cmds) = NULL;
    int * parse_units_amount = (int*) calloc (amount_of_cmds + 2, sizeof (int)); // array of cmd sizes, the first elemen is amount of cmds
    *parse_units_amount = amount_of_cmds + 1;
    parse_units_amount[amount_of_cmds + 1] = 0;

    for (ssize_t i = 0; i < amount_of_cmds; i++)
    {
        *(parse_units_amount + i + 1) = parse_single_cmd((*cmds + i), (*cmd_starts_arr)[i]);
    }  

    
    free (*cmd_starts_arr);
    free (cmd_starts_arr); 

    return parse_units_amount;
}

int get_amount_of_commands (ssize_t line_size, char *line, char ***cmds_array)
{
    int amount = 1;
    *cmds_array = calloc (amount, sizeof (char*));

    line = skip_spaces(line);
    **cmds_array = line;

    for (ssize_t i = 0; i < line_size; i++)
        if (line[i] == '|') 
        {
            line[i] = '\0';
            amount++;

            *cmds_array = (char**)realloc (*cmds_array, amount*sizeof (char*));

            (*cmds_array) [amount - 1] = skip_spaces(line + i + 1);
        }

    return amount;
}

char* skip_spaces (char* line)
{
    int i = 0;
    for (; line[i] == ' ' || line[i] == '\t'; i++);

    return line + i;
}

int parse_single_cmd (char ***cmds, char *line)
{
    char *start_line = line;
    size_t cmd_sz = strlen (line);
    line = skip_spaces(line);
    int i = 1;

    *cmds = calloc (1, sizeof (char*));
    **cmds = line;

    for (; line < cmd_sz + start_line; line++)
    {
        if (*line == ' ' || *line == '\t')
        {
            *line = '\0';
            line = skip_spaces (line + 1);
            if (!is_line_finished(line))
            {
                i++;
                *cmds = realloc (*cmds, i * (sizeof (char*)));
                *(*(cmds) + i - 1) = line;
            }
        }
        
        if (*line == '\n')
            *line = '\0';
    }

    *cmds = realloc (*cmds, (i+1) * sizeof (char*));
    (*cmds)[i] = NULL;

    return i+1;
}

int is_line_finished (char *line)
{
    for (int i = 0; i < strlen(line) + 1; i++)
    {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\0' && line[i] != '\n')
            return 0;
    }

    return 1;
}
