#include "main.h"


int main()
{
    command_line *line;
    char *string;
    size_t BF_SIZE = 0;
    
 	while(1)
    {
        printf("$ ");
        
 		if (!getline(&string, &BF_SIZE, stdin))
        {
            perror("fgets");
        };

        if (string[0] == 'q')
        {
            return 0;
        }

        line = create_command_line(string);

        call_progs(line);

    }

 	return 0;
 
}
