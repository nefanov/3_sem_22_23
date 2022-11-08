#include "fifo.hpp"

int main(int argc, char **argv) {
    int BUF_SIZE = GetBufSize(argc, argv);
    ASSERTED(GetBufSize, BUF_SIZE, -1, -1);

    int result = mkfifo(FIFO_NAME, O_RDWR);
    ASSERTED(mkfifo, result, -1, -1);

    int fd = open(FIFO_NAME, O_WRONLY);
    ASSERTED(open, fd, -1, -1);

    FILE *f_input = fopen(argv[2], "r");
    ASSERTED(fopen, f_input, NULL, -1);

    int f_size = GetFSize(f_input);
    ASSERTED(GetFSize, f_size, -1, -1);

    void *tmp_buf = calloc(BUF_SIZE, 1);
    ASSERTED(calloc, tmp_buf, NULL, -1);

    if (f_size > BUF_SIZE) {
        fread(tmp_buf, 1, f_size, f_input);
        write(fd, tmp_buf, f_size);
    } else {
        int chunk_number = f_size / BUF_SIZE;
    
        for (int i = 0; i < chunk_number; i++) {
            fread(tmp_buf, 1, BUF_SIZE, f_input);
            write(fd, tmp_buf, BUF_SIZE);
        }

        if (int mod = f_size % BUF_SIZE != 0) {
            fread(tmp_buf, 1, mod, f_input);
            write(fd, tmp_buf, mod);
        }
    }

    close(fd);
    fclose(f_input);

    return 0;
}