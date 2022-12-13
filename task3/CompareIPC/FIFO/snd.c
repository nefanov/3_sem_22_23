#include <FIFO.h>

void SendData()
{

}

int main()
{
    int input_fd = open(INPUT_NAME, O_RDONLY); // perr
    logs("Open output");

    // open fifo
    mkfifo(FIFO_NAME, 0666); // perr
    logs("Create fifo");

    char buffer[BUFFER_SIZE] = {};
    int fifo_fd = open(FIFO_NAME, O_WRONLY); // perr
    logs("Open fifo");
    // while file is not read completely

    size_t read_ret = 0;
    do
    {
        read_ret = read(input_fd, buffer, BUFFER_SIZE); // perr
        logs("Read %d bytes", read_ret);

        write(fifo_fd, buffer, read_ret); // perr
        logs("Write");
    }
    while (read_ret);

    close(fifo_fd);
    close(input_fd);
    logs("Close descriptors");

    return 0;
}