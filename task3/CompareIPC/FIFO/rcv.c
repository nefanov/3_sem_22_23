#include <FIFO.h>

int main()
{

    int output_fd = open(OUTPUT_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0600); // perr
    logr("Open output");

    char buffer[BUFFER_SIZE] = {};
    int fifo_fd = open(FIFO_NAME, O_RDONLY); // perr
    logr("Open fifo");
    // while file is not read completely

    size_t read_ret = 0;
    do
    {
        read_ret = read(fifo_fd, buffer, BUFFER_SIZE); // perr
        logr("Read %d bytes", read_ret);

        write(output_fd, buffer, read_ret); // perr
        logr("Write");
    }
    while (read_ret);

    close(fifo_fd);
    close(output_fd);
    logr("Close descriptors");

    return 0;

    // open fifo for reading

    // while not completely read

        // read from fifo
}