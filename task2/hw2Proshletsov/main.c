#include "lib.h"
#include <unistd.h>
#include <sys/types.h>
//#include <sys/fnctl.h>
#include <errno.h>


int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf (stderr, "Wrong amount of arguments. Should be 2 input fila and output file\n");
        return 1;
    }

    Pipe *pipe_hndl = pipe_construct (BUF_SIZE);

    pid_t this_pid = fork();
    if (this_pid == 0)
    {
        tune_pipes_for_child (pipe_hndl);
        int size = recieve_file_size (pipe_hndl);
        printf ("sizeinchild = %d\n", size);

        FILE *output = fopen (argv[2], "w");

        size_t total_read = 0;
        size_t curr_read = 0;
        while (total_read < size)
        {
            curr_read = pipe_hndl->operations.recieve (pipe_hndl, size);
            total_read += curr_read;
            fwrite (pipe_hndl->buffer, sizeof (char), curr_read, output);
        }
        fclose (output);
    }
    else
    {
        tune_pipes_for_parent (pipe_hndl);
        FILE *input = fopen (argv[1], "r");
        send_file_size (input, pipe_hndl);

        while (!feof(input))
        {
            fread (pipe_hndl->buffer, sizeof (char), BUF_SIZE, input);
            pipe_hndl->operations.send (pipe_hndl);
        }
        

       fclose (input);
    }

    return 0;
}