#include <MaDuplex.h>

/* [Description]

The target of this project is to implement duplex pipe using
ordinary pipe. 

Ctor: 
[*] allocate memory for pipeDuplex instance
[*] put pointers to receive/send functions
[*] create 2 pipes for parent-->child and vice versa 
directions
[*] data, len?

Ideas 
1. for parent-->child:

    parent puts message in write fd of 'direct' pipe
    child reads from read fd of 'direct' pipe

2. for child-->parent:

    child puts message in write fd of 'back' pipe
    parent reads from read fd of 'back' pipe

Task:

    Read file with parent, transfer it contetnt to child, from child retransfer it
    to parent, write to file.

i.e:

    [1] read portion from file
    [2] transfer it to child
    [3] retransfer it from child
    [4] write to file

Version roadmap:

0.0:      transfer string from parent to child
0.5:    retransfer string to parent
1.0:      read from file and write to file (mb stdout)

*/

int main(int argc, const void* argv[])
{
    duplexPipe* dupPipe = NULL;

    switch(argc)
    {
        case 1:
            dupPipe = duplexPipeCtor(STD_INPUT_NAME);
            break;
        case 2:
            dupPipe = duplexPipeCtor(argv[1]);
            break;
        default:
            PRINT_ERROR("Please, enter 0 or 1 cmd argument, like:\n"
                        "$ ./MaDuplex std_input.txt\n");
            return 1;
    } 

    int pid = fork();
    int packet_id = 0;
    long long transfered_bytes = 0;

    while(1)
    {
        LOGC("[Packet id: %d]", packet_id);
        LOGP("[Packet id: %d]", packet_id++);

        switch(pid)
        {
            case ERROR:
            {
                PRINT_ERROR("fork negative pid");
            }
            case CHILD:
            {
                LOGC("Enter CHILD");

                dupPipe->acts.direct_interact(dupPipe, pid);
                dupPipe->acts.back_interact(dupPipe, pid);

                LOGC("Exit CHILD");
            }
            default: // PARENT
            {
                LOGP("Enter PARENT");

                // Write to buff
                dupPipe->acts.direct_interact(dupPipe, pid);
                int write_ret = dupPipe->acts.back_interact(dupPipe, pid);
                transfered_bytes += write_ret;
                printf("Transfered %lld | %ld kilo bytes\r", transfered_bytes / 1024, dupPipe->input_size);
                fflush(stdout);
                LOGP("Exit PARENT");
            }
        }
    }
    puts("");
    duplexPipeDtor(dupPipe, pid);
}