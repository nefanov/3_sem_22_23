#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


#include "pipe_manager.h"
#include "parser.h"


int main(void)
{
    
    comand com = {0};

    while(1)
    {

        printf("EBUSH\\\\> ");

        comand_rp(&com);

        pipe_manager(&com);

        cmd_struct_destructor(&com);

    }
}
