#include "fifo.hpp"

int main(int argc, char **argv) {
    printf("client: ");

    int BUF_SIZE = GetBufSize(argc, argv);
    ASSERTED(GetBufSize, BUF_SIZE, -1, -1);

    // int result = mkfifo(FIFO_NAME, O_RDWR);
    // ASSERTED(mkfifo, result, -1, -1);

    int fd = -1;
    while (fd == -1) {
        fd = open(FIFO_NAME, O_RDONLY);
    }


    FILE *f_input = fopen(argv[2], "r");
    ASSERTED(fopen, f_input, NULL, -1);

    FILE *f_output = fopen(argv[3], "w");
    ASSERTED(fopen, f_output, NULL, -1);

    int f_size = GetFSize(f_input);
    ASSERTED(GetFSize, f_size, -1, -1);

    void *tmp_buf = calloc(BUF_SIZE, 1);
    ASSERTED(calloc, tmp_buf, NULL, -1);

    if (BUF_SIZE > f_size) {
        read(fd, tmp_buf, f_size);
        fwrite(tmp_buf, 1, f_size, f_output);
    } else {
        int chunk_number = f_size / BUF_SIZE;
    
        for (int i = 0; i < chunk_number; i++) {
            int result = read(fd, tmp_buf, BUF_SIZE);
            fwrite(tmp_buf, 1, BUF_SIZE, f_output);

        }

        if (int mod = f_size % BUF_SIZE != 0) {
            read(fd, tmp_buf, f_size);
            fwrite(tmp_buf, 1, mod, f_output);
        }
    }

    close(fd);
    fclose(f_input);
    fclose(f_output);

    free(tmp_buf);

    unlink(FIFO_NAME);

    return 0;
}
