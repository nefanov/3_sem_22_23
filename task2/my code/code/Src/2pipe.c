#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

static const size_t str_start_size =   1000000;

static const ssize_t my_massege_size = 20000;

int min(size_t a, size_t b) {return (a < b)? a : b;}

typedef struct pPipe Pipe;

typedef struct op_table Ops;

typedef struct op_table  
{
    size_t (*rcv)(Pipe *self); 
    size_t (*snd)(Pipe *self); 
} Ops;

typedef struct str_t
{
    char* str;
    size_t sz;
    size_t len;
} str;

typedef struct pPipe 
{
        str r_data; // intermediate buffer for read
        str w_data; // intermediate buffer for write
        int fd_direct[2]; // array of r/w descriptors for "pipe()" call (for parent-->child direction)
        int fd_back[2]; // array of r/w descriptors for "pipe()" call (for child-->parent direction)
        Ops actions;
        int status;
} Pipe;

void clean_str(str* string);

void realloc_up_str (str* string)
{
    
    char* buf = NULL;
    
    buf = realloc (string->str, ((string->sz * 2) + 1) * sizeof(char));

    if(buf == NULL)
    {
        printf("press f mem\n");
        return;
    }

    string->str = buf;
    string->sz *= 2;
    
} 

void realloc_down_str (str* string)
{
    
    char* buf = NULL;
    
    if(string->sz > str_start_size)
    {
        
        buf = realloc (string->str, ((string->sz / 2) + 1) * sizeof(char));

        if(buf == NULL)
        {
            printf("press f mem\n");
            return;
        }

        string->str = buf;
        string->sz /= 2;

    }
}

void init_str(str* string)
{
    
    char* buf = calloc(str_start_size, sizeof(char));

    if(buf == NULL)
    {
        printf("press f mem\n");
        return;
    }

    string->str = buf;
    string->len = 0;
    string->sz = str_start_size;

}

void str_dest(str* string)
{
    free(string->str);
    string->str = NULL;
}

void realloc_str (str* string)
{
    
    while(1)
    {
        if ((string->len * 2 <= string->sz) && (string->len >= str_start_size))
        {
            realloc_down_str(string);
        }
        else if ((string->len >= string->sz) && (string->len >= str_start_size))
        {
            realloc_up_str(string);
        }
        else
        {
            return;
        }
    }

}

void str_add_cstr (str* string, char* cstr)
{
    char* buf = NULL;
    size_t itter = 0;

    for(buf = cstr, itter = 0; *buf != '\0'; buf ++, itter ++)
    {
        *(string->str + itter) = *buf;
        string->len ++;
        realloc_str(string);
    }
}

void str_cpy_str (str* string, str* src_str)
{
    
    string->len = src_str->len;
    realloc_str(string);

    memcpy(string->str, src_str->str, src_str->len);
    
}

void str_cpy_nstr (str* string, str* src_str, size_t n)
{
    
    string->len = n;
    realloc_str(string);

    memcpy(string->str, src_str->str, n);

}

void str_cpy_ncstr (str* string, char* src_str, size_t n)
{
    
    string->len = n;
    realloc_str(string);

    memcpy(string->str, src_str, n);

}
size_t read_str (str* string, int fd)
{
    size_t old_len = string->len;
    
    while(1)
    {
        
        ssize_t reading = read(fd, string->str + string->len, string->sz - string->len);

        string->len += reading;
        
        if(string->len < string->sz)
        {
            return (string->len - old_len);
        }


        //printf("\rRead data    [%.ld]", string->len/old_len);
        //fflush(stdout);

        realloc_str(string);

    }     
}

ssize_t write_str (str* string, int fd)
{   
    ssize_t old_len = string->len;
    size_t it = 0;

    while (1)
    {

        ssize_t writed = write(fd, string->str + it, string->len - it);
        
        if(writed == -1) return writed;
        
        if(string->len == it)
        {
            clean_str(string);
            return old_len;
        }

        it += writed;    
    }
}

void init_status_pipe (Pipe* pipe, pid_t pid)
{
    if(pid == 0)
    {
        pipe->status = 0;
    }
    else
    {
        pipe->status = 1;
    }
}

ssize_t read_pipe (Pipe* pipe)
{
    
    if(pipe->status == 0)
    {
        return read_str(&(pipe->r_data), pipe->fd_direct[0]);
    }
    else if (pipe->status == 1)
    {
        return read_str(&(pipe->r_data), pipe->fd_back[0]);
    }

}

ssize_t write_pipe (Pipe* pipe)
{
    
    if(pipe->status == 1)
    {
        return write_str(&(pipe->w_data), pipe->fd_direct[1]);
    }
    else if (pipe->status == 0)
    {
        return write_str(&(pipe->w_data), pipe->fd_back[1]);
    }

}

void read_from_file_str(FILE* file, str* string)
{


    struct stat stats = {};
    
    fstat(fileno(file), &stats);

    string->sz = (size_t) stats.st_size;

    string->str= calloc(string->sz + 1, sizeof(char));

    string->len = fread(string->str, sizeof(char), string->sz, file);
    
}

void write_to_file_str(FILE* file, str* string)
{
    fwrite(string->str, sizeof(char), string->len, file);
}

void Pipe_inint (Pipe *struct_pipe)
{
    
    pipe(struct_pipe->fd_back);
    pipe(struct_pipe->fd_direct);

    init_str(&(struct_pipe->r_data));
    init_str(&(struct_pipe->w_data));

    struct_pipe->actions.rcv = read_pipe;
    struct_pipe->actions.snd = write_pipe;

}

void Pipe_destructor(Pipe* pipe)
{
    str_dest(&(pipe->r_data));
    str_dest(&(pipe->w_data));

    if(pipe->status == 0)
    {
        close(pipe->fd_direct[0]);
        close(pipe->fd_direct[1]);
    }
    else
    {
        close(pipe->fd_back[0]);
        close(pipe->fd_back[1]);
    }
}

void clean_str(str* string)
{
    string->len = 0;
}

int main ()
{
    Pipe pipe ={};
    Pipe_inint(&pipe);

    pid_t pid = fork();

    init_status_pipe(&pipe, pid);

    if(pid)
    {

        FILE* file_in = fopen("file.txt", "rb");
        
        if(file_in == NULL)
        {
            printf("Reading data [File not open]\n");
            return 0;
        }

        str str_in = {};
        init_str(&str_in);

        printf("Reading data [...]");
        fflush(stdout);
        read_from_file_str(file_in, &str_in);
        printf("\rReading data [OK] \n");
        fflush(stdout);


        fclose(file_in); 

        FILE* file_out = fopen("out.txt", "wb");
        size_t itter = 0;

        while(itter < str_in.len)
        {
            printf("\rSend data    [%3ld\%]", /*itter/my_massege_size*/ (itter*100)/str_in.len);
            fflush(stdout);

            size_t cpy_len = min(my_massege_size, str_in.len - itter);
            str_cpy_ncstr(&(pipe.w_data), str_in.str + itter, cpy_len);

            size_t sended = pipe.actions.snd(&pipe);

            pipe.actions.rcv(&pipe);

            write_to_file_str(file_out, &(pipe.r_data));

            clean_str(&(pipe.r_data));

            itter += sended;
        }
        Pipe_destructor(&pipe);
        printf("\rSend data    [%3ld\%]\n", /*itter/my_massege_size*/ (itter*100)/str_in.len);
            fflush(stdout);

        close(file_out);

        exit(0);
    }
    else
    {

        size_t readed = my_massege_size;

        while (readed == my_massege_size )
        {
            readed = pipe.actions.rcv(&pipe);

            str_cpy_str(&(pipe.w_data), &(pipe.r_data));
            
            clean_str(&(pipe.r_data));

            pipe.actions.snd(&pipe);
        }
        
        Pipe_destructor(&pipe);

        exit(0);

    }
}