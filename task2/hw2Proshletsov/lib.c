#include "lib.h"


/* Pipe constructor. */
Pipe *pipe_construct (size_t size)
{
    Pipe *pipe_elem = calloc (1, sizeof (Pipe));
    
    pipe_elem->buffer = calloc (size, sizeof(char));
    pipe_elem->len = size;

    int ret_check = 0;
    ret_check = pipe (pipe_elem->reciever);
    if (ret_check < 0)
    {
        perror ("Receiver pipe creation error: ");
        return NULL;
    }

    ret_check = pipe (pipe_elem->sender);
    if (ret_check < 0)
    {
        perror ("Sender pipe creation error: ");
        return NULL;
    }

    pipe_elem->operations.send = send_op;
    pipe_elem->operations.recieve = recieve_op;    

    return pipe_elem;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Recieve and send functions for Pipe operation functions. */
size_t recieve_op (Pipe *self, size_t max_size)
{
    static size_t recieved_from_the_beginning = 0;
    ssize_t curr_read = 0;
    size_t total_read = 0;

    while (total_read < BUF_SIZE && max_size > recieved_from_the_beginning)
    {
        if (recieved_from_the_beginning + BUF_SIZE - total_read < max_size)
        {  
            curr_read = read (self->reciever[0], self->buffer + total_read, BUF_SIZE - total_read);
        }
        else
        {
            curr_read = read (self->reciever[0], self->buffer + total_read, max_size - recieved_from_the_beginning);
            total_read += curr_read;
            recieved_from_the_beginning += curr_read;

            break;
        }
      
        if (curr_read < 0)
        {
            perror("Read from reciever error: ");
            return 0;
        }

        total_read += curr_read;
        recieved_from_the_beginning += curr_read;
    }

    return total_read;
}

size_t    send_op (Pipe *self, size_t amount_to_send, pid_t pid)
{
    ssize_t curr_written = 0;
    size_t total_written = 0;

    while (total_written < amount_to_send)
    {
        curr_written = write (self->sender[1], self->buffer, amount_to_send);
        if (curr_written < 0)
        {
            printf ("error\n");
            perror ("Write to sender error");
            return 0;
        }

        total_written += curr_written;
    }

    return total_written;        
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Pipe tuning fot child and parent. Makes reciever and sender match their names in each process. */
void tune_pipes_for_child (Pipe *self)
{
    int tmp0 = self->reciever[0];
    int tmp1 = self->reciever[1];
    self->reciever[0] = self->sender[0];
    self->reciever[1] = self->sender[1];
    self->sender[0] = tmp0;
    self->sender[1] = tmp1;

    close (self->sender[0]);
    close (self->reciever[1]);
}

void tune_pipes_for_parent (Pipe *self)
{
    close (self->sender[0]);
    close (self->reciever[1]);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Send file size from parent to child.*/

size_t send_file_size (FILE* file, Pipe *pipe)
{
    int check = fseek (file, 0, SEEK_END);
    if (check < 0)
        perror ("fseek error");

    size_t size = ftell (file);

    fseek (file, 0, SEEK_SET);


    size_t size_save = size;
    int num_len = 0;
    for (size_t i = 1; size/(i*10) != 0; i*=10, num_len++);

    for (; num_len >= 0; num_len--)
    {
        pipe->buffer[num_len] = '0' + size % 10;
        size /= 10;
    }

    ssize_t num = write (pipe->sender[1], pipe->buffer, BUF_SIZE);
    return size_save;
}

size_t recieve_file_size (Pipe *pipe)
{
    read (pipe->reciever[0], pipe->buffer, BUF_SIZE);

    char* pointer = pipe->buffer;

    size_t size = 0;
    for (; *pointer != '\0'; pointer++);    

    pointer--;
    for (size_t i = 1; pointer != (pipe->buffer - 1); pointer--, i *=10)
    {
        size += (*pointer - '0') * i;    
    }

    return size;
}
