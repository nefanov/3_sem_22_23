#include "main.h"





Pipe *constructPipe()
{
    Pipe *pip = (Pipe*)malloc(sizeof(Pipe));
    assert(pip);

    pip->len = BUF_SZ;
    pip->actions.rcv = receive;
    pip->actions.snd = send;

    return pip;

}


void create_pipe(Pipe *pip)
{
	if (pipe(pip->fd_direct) < 0 || pipe(pip->fd_back) < 0)
    {
        perror("pipe");
        _exit(-1);
    }
}


void clear_buffer(char* buffer, size_t buffer_size)
{
    for (size_t byte_id = 0; byte_id < buffer_size; byte_id++)
    {
        buffer[byte_id] = '\0';
    }
}


size_t send(Pipe *pip, int result)
{
    size_t size;

    if (result < 0)
    {
        perror("fork");
        _exit(-1);
    }
    else if (result > 0)//родитель читает из text.txt и отправляет ребенку через канал fd_direct
    {
    	size = parent_send(pip);
    }
    else//ребенок читает из канала fd_direct и сохраняет у себя в data_read
    {
		size = child_receive(pip);
    }

    return size;

}


size_t receive(Pipe *pip, int result)
{
    size_t size;

    if (result < 0)
    {
        perror("fork");
        _exit(-1);
    }
    else if (result == 0)//ребенок отправляет родителю через канал fd_back, то что прочитал ранее
    {
    	size = child_send(pip);
    }
    else//родитель читает из канала fd_back и выводит в text2.txt
    {
 		size = parent_receive(pip);
    }

    return size;

}


size_t parent_send(Pipe *pip)
{
	size_t size;
	static int fd = 0;

    if (fd == 0)
    {
    	close((pip->fd_direct)[0]);
        if ((fd = open("file.txt", O_RDONLY)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }
    
    size = read(fd, pip->data_read, pip->len);

    if (size != pip->len)
    {
    	for (size_t i = size; i < pip->len; i++)
    	{
    		(pip->data_read)[i] = '\0';
    	}
    }
  
    if (size < 0)
    {
        close(fd);
        perror("read");
        _exit(-1);
    }

    write((pip->fd_direct)[1], pip->data_read, pip->len);
    
    if (size < 0)
    {
        close(fd);
        perror("write");
        _exit(-1);
    }
    
    return size;

}


size_t child_receive(Pipe *pip)
{
	size_t size;
	static int first = 1;

	if (first == 1)
	{
		close((pip->fd_back)[1]);
		first = 0;
	}
   
    size = read((pip->fd_direct)[0], pip->data_read, pip->len);
    
    if (size < 0)
    {
        perror("read");
        _exit(-1);
    }  

    return size;  
}


size_t child_send(Pipe *pip)
{
	size_t size;
	static int first = 1;

	if (first == 1)
	{
		close((pip->fd_back)[0]);
		first = 0;
	}

    size = write((pip->fd_back)[1], pip->data_read, pip->len);

    if (size < 0)
    {
        perror("write");
        _exit(-1);
    }

    return size;

}


size_t parent_receive(Pipe *pip)
{
	size_t size;
	static int fd = 0;

	if (fd == 0)
    {
    	close((pip->fd_back)[1]);
        if ((fd = open("text.txt", O_WRONLY | O_CREAT, 0666)) < 0)
        {
            perror("open");
            _exit(-1);
        }
    }

    size = read((pip->fd_back)[0], pip->data_write, pip->len);

    if (size < 0)
    {
        close(fd);
        perror("read");
        _exit(-1);
    }

    size = write(fd, pip->data_write, pip->len);
    
    if (size < 0)
    {
        close(fd);
        perror("write");
        _exit(-1);
    } 

    return size; 

}


void destructPipe(Pipe *pip)
{
	free(pip);
}