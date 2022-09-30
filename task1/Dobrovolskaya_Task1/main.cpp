#include "main.h"


int main()
{
    command_line *line;
    char *string;
    size_t BF_SIZE;

    BF_SIZE = 0;
    
 	while(1)
    {
        printf("$ ");
        
 		if (!getline(&string, &BF_SIZE, stdin))
        {
            perror("getline");
        };

        if (is_it_again(string))
        {
            continue;
        }

        line = create_command_line(string);

        if (call_progs(line))
        {
            destruct_command_line(line);
            return 0;
        }

        destruct_command_line(line);

    }

 	return 0;
 
}
