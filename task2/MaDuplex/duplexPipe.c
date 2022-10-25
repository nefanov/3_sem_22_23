#include <MaDuplex.h>

duplexPipe* duplexPipeCtor(const char* user_input_name)
{
    LOG("Start CTOR");
    const char* input_name = user_input_name;
    const char* output_name = STD_OUTPUT_NAME;

    duplexPipe* dupPipe = (duplexPipe*) calloc(1, sizeof(duplexPipe));
    if (!dupPipe) PRINT_ERROR("calloc null ptr");

    memset(dupPipe->data, 0, BUFFER_SIZE); // Buffer is statically alloced inside object
    dupPipe->len = 0;

    // Create pipes
    if (pipe(dupPipe->fd_direct) == ERROR) PRINT_ERROR(strerror(errno));
    LOG("FD direct: read - %d, write - %d", dupPipe->fd_direct[READ_FD], dupPipe->fd_direct[WRITE_FD]);

    if (pipe(dupPipe->fd_back) == ERROR) PRINT_ERROR(strerror(errno));
    LOG("FD  back: read - %d, write - %d", dupPipe->fd_back[READ_FD], dupPipe->fd_back[WRITE_FD]);

    // Open input-output files
    int input_fd = open(input_name, O_RDONLY);
        if (input_fd == ERROR) PRINT_ERROR(strerror(errno));
    dupPipe->input_fd = input_fd;
        LOG("Input fd %d", input_fd);

    struct stat buf;
    fstat(input_fd, &buf);
    off_t size = buf.st_size;
    dupPipe->input_size = size / 1024; // Kilo bytes

    int output_fd = open(output_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);  // We should write the Satana number
        if (output_fd == ERROR) PRINT_ERROR(strerror(errno));               // to open files in POSIX correctly
        dupPipe->output_fd = output_fd;
        LOG("Output fd %d", output_fd);

    // Init ops table
    dupPipe->acts.direct_interact = direct_interact;
    dupPipe->acts.back_interact = back_interact;

    return dupPipe;
}

void duplexPipeDtor(duplexPipe* dupPipe, int pid)
{
    LOG("Start DTOR");

    close(dupPipe->fd_direct[READ_FD]);
    close(dupPipe->fd_direct[WRITE_FD]);
    
    close(dupPipe->fd_back[READ_FD]);
    close(dupPipe->fd_back[WRITE_FD]);

    close(dupPipe->input_fd);
    close(dupPipe->output_fd);

    if (pid != CHILD)
    {
        LOG("Dtor PARENT");
        memset(dupPipe, 0, sizeof(duplexPipe));
        free(dupPipe);
    }
    else
    {
        LOG("Dtor CHILD");
    }
}

size_t direct_interact(duplexPipe *self, int pid)
{   
    switch(pid)
    {
        case ERROR:
            PRINT_ERROR("Fork error");
            break;
        case CHILD:
            {
                LOGC("Direct interact");
                int read_ret = read(self->fd_direct[READ_FD], self->data, BUFFER_SIZE);
                self->len = read_ret;
                break;
            }
        default: // PARENT
            {
                LOGP("Direct interact");
                int read_ret = read(self->input_fd, self->data, BUFFER_SIZE);

                if (!read_ret) 
                    {
                        LOGP("Reached end of file");  
                        exit(0);
                    }

                write(self->fd_direct[WRITE_FD], self->data, read_ret);
                self->len = read_ret;
            }
    }
}

size_t back_interact(duplexPipe *self, int pid)
{    
    switch(pid)
    {
        case ERROR:
            PRINT_ERROR("Fork error");
            break;
        case CHILD:
            {
                LOGC("Back interact");
                write(self->fd_back[WRITE_FD], self->data, self->len);
                break;
            }
        default: // PARENT
            {
                LOGP("Back interact");
                int read_ret = read(self->fd_back[READ_FD], self->data, self->len);
                write(self->output_fd, self->data, read_ret);
                return read_ret;
            }
    }
}