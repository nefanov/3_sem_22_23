#include "duplex-pipe.h"

int main(int argc, char* argv[])
{
    if(argc == 3)
    {
        char* inputfile = argv[1];
        char* outputfile = argv[2];

        Pipe* main_pipe;
        if((main_pipe = construct_pipe(BUF_MAX_SIZE)) == NULL)
        {
            printf("EROOR: creating main_pipe\n");
            return -1;
        }

        if(duplex_realize(main_pipe, inputfile, outputfile) == -1)
        {
            printf("Duplex realize ERROR\n");
            return -1;
        }

        if(destruct_pipe(main_pipe) == 1)  
            return 0;    

        else
        { 
            printf("ERROR: bad destruct!\n");
            return -1;
        }

    }
    else
    {
        printf("ERROR: bad input files, retry\n");
        return -1;
    }
}
