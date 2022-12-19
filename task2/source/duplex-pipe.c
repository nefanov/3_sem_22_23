#include "duplex-pipe.h"

size_t receive(Pipe* main_pipe)
{
    assert(main_pipe);
    assert(main_pipe->data_direct);

    return(read((main_pipe->fd_direct)[0], main_pipe->data_direct, main_pipe->len));
}

size_t send(Pipe* main_pipe)
{
    assert(main_pipe);
    assert(main_pipe->data_direct);

    return(write((main_pipe->fd_back)[1], main_pipe->data_direct, main_pipe->len));
}

Pipe* construct_pipe(size_t len)
{
    Pipe* main_pipe = (Pipe*)calloc(1, sizeof(Pipe));
    if(main_pipe == NULL)
    {
        printf("ERROR: Bad calloc pipe\n");
        return NULL;
    }

    main_pipe->data_direct = (char*)calloc(len, sizeof(char));
    //main_pipe->data_back = (char*)calloc(len, sizeof(char));
    
    if(main_pipe->data_direct == NULL) //|| main_pipe->data_back == NULL)
    {
        printf("ERROR: Bad calloc data buf\n");
        return NULL;
    }

    main_pipe->len = 0;

    if(pipe(main_pipe->fd_direct) == -1 || pipe(main_pipe->fd_back) == -1)
    {
        printf("ERROR: bad descriptors\n");
        return NULL;
    }

    main_pipe->actions.receive = receive;
    main_pipe->actions.send = send;

    return main_pipe;
}

int destruct_pipe(Pipe* main_pipe)
{
    //free(main_pipe->data_back);
    free(main_pipe->data_direct);
    free(main_pipe);  

    return 1;
}

int swap_descriptors(Pipe* main_pipe)
{
    assert(main_pipe);

    int tmp[2] = {0, 0}; 
    tmp[0] = main_pipe->fd_direct[0];
    main_pipe->fd_direct[0] = main_pipe->fd_back[0]; 
    main_pipe->fd_back[0] = tmp[0];
    tmp[1] = main_pipe->fd_direct[1];
    main_pipe->fd_direct[1] = main_pipe->fd_back[1];
    main_pipe->fd_back[1] = tmp[1];
    close(main_pipe->fd_direct[1]);
    close(main_pipe->fd_back[0]);

    return 1;
}

// int swap_buffs(Pipe* main_pipe)
// {   
//     char* temp = NULL;
//     temp = main_pipe->data_direct;
//     main_pipe->data_direct = main_pipe->data_back;
//     main_pipe->data_back = temp;

//     return 1;
// }

size_t get_filesize (FILE* file_from) 
{
    assert (file_from);
 
    struct stat buff = {};
    fstat (fileno (file_from), &buff);
 
    return buff.st_size;
}

int duplex_realize(Pipe* main_pipe, char* inputfile, char* outputfile)
{
    pid_t pid;
    FILE* file_from = fopen(inputfile, "r");
    size_t filesize = get_filesize(file_from);
    fclose(file_from);

    if ((pid = fork()) == -1) 
    {
        printf("ERROR: Fork failed!\n");
        return -1;
    }
    
    if (pid == 0)
    {
        if(swap_descriptors(main_pipe) != 1)// ||swap_buffs(main_pipe) != 1)
        {
            printf("ERROR: swapping buffs and descriptors\n");
            return -1;
        }
        
        size_t temp_size;  

        while(filesize > 0)
        {
            main_pipe->len = BUF_MAX_SIZE;
            temp_size = main_pipe->actions.receive(main_pipe);
            main_pipe->len = temp_size;
            filesize -= temp_size;
            main_pipe->actions.send(main_pipe);
        }

        exit(0);
    }

    else if(pid > 0)
    {
        FILE* file_from = fopen(inputfile, "r");
        FILE* file_to = fopen(outputfile, "w");

        close(main_pipe->fd_direct[1]);
        close(main_pipe->fd_back[0]);

        if(file_to == NULL || file_from == NULL)
        {
            printf("ERROR: failed opening files\n");
            return -1;
        }
        
        while(!feof(file_from))
        {
            size_t len = fread(main_pipe->data_direct, sizeof(char), BUF_MAX_SIZE, file_from);
            main_pipe->len = len;
            main_pipe->actions.send(main_pipe);
            main_pipe->actions.receive(main_pipe);
            fwrite(main_pipe->data_direct, sizeof(char), main_pipe->len, file_to);
        }

        int status = 0;
	    waitpid(pid, &status, 0);
	    printf("FINISHED: Ret code: %d\n", WEXITSTATUS(status)); 
        fclose(file_to);
        fclose(file_from);
    }

    return 0;
}

