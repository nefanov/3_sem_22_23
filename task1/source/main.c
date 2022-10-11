#include "shell-wrapper_func.h"

int main()
{
    char* cmd = (char*)calloc(ARG_MAX, sizeof(char));
    cmd_line *Bash = (cmd_line*)calloc(1, sizeof(cmd_line));
    cmd_line_constructor(Bash);
    while(1)
    {   
        fgets(cmd, ARG_MAX, stdin);
        if(strcmp(cmd, "\n") != 0)
        { 
            seq_pipe(cmd, Bash);
            cmd_line_destructor(Bash);
        }
        else printf("ERROR with input\n");
        
    }
    free(cmd);
    free(Bash);
    return 0;
 }
