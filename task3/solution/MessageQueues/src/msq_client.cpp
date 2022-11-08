#include "msq.h"

// #define DEBUG

#ifdef DEBUG
    #define D(...) __VA_ARGS__
#else
    #define D(...)
#endif

int main(int argc, char **argv) {
    size_t BUF_SIZE = 0;

    if (argc != 4) {
        printf("Wrong arguments\n");
        return -1;
    } else if (strcmp(argv[1], "SMALL") == 0)
        BUF_SIZE = SMALL;
    else if (strcmp(argv[1], "MEDIUM") == 0)
        BUF_SIZE = MEDIUM;
    else if (strcmp(argv[1], "BIG") == 0)
        BUF_SIZE = BIG;
    else {
        printf("Wrong arguments\n");
        return -1;
    }

    D(printf("Buf size: %ld\n", BUF_SIZE));

    FILE *input_f = fopen(argv[2], "r");
    ASSERTED(fopen, input_f, NULL, -1);

    struct stat stat_buf;
    size_t f_size = 0;

    if (fstat(fileno(input_f), &stat_buf) == 0)
        f_size = stat_buf.st_size;
    else {
        perror("fstat failed");
        return -1;
    }

    FILE *output_f = fopen(argv[3], "w");
    ASSERTED(fopen, output_f, NULL, -1);

    key_t key = ftok(FILENAME, 0);
    ASSERTED(ftok, key, -1, -1);

    int msq_id = msgget(key, IPC_CREAT | 0666);
    ASSERTED(msgget, msq_id, -1, -1);

    D(printf("fSize = %ld\n", f_size));

    struct my_msg {
        long mtype = 1;
        char msg[BIG] = {};
    };

    my_msg msg_buf_c;

    D(printf("mtype: %ld\n", msg_buf_c.mtype));

    int result;
    if (BUF_SIZE > f_size) {
        result = msgrcv(msq_id, &msg_buf_c, f_size, 1, MSG_NOERROR);

        D(ASSERTED(msgrcv, result, -1, -1));
        D(printf("Fc %c\n", msg_buf_c.msg[1]));
        D(printf("msgid: %d\n", msq_id));

        fwrite(msg_buf_c.msg, sizeof(char), f_size, output_f);
    } else {
        for (size_t i = 0; i < f_size / BUF_SIZE; i++) {
            result = msgrcv(msq_id, &msg_buf_c, BUF_SIZE, 1, 0);
            D(ASSERTED(msgrcv, result, -1, -1));
            D(printf("working client\n"));

            fwrite(msg_buf_c.msg, sizeof(char), BUF_SIZE, output_f);
        }

        D(printf("остаток: %d\n", f_size % BUF_SIZE));

        if (f_size % BUF_SIZE != 0) {
            result = msgrcv(msq_id, &msg_buf_c, f_size % BUF_SIZE, 1, MSG_NOERROR);

            D(ASSERTED(msgrcv, result, -1, -1));

            fwrite(msg_buf_c.msg, sizeof(char), f_size % BUF_SIZE, output_f);
        }
    }

   

    result = msgctl(msq_id, IPC_RMID, NULL);
    ASSERTED(msgctl, result, -1, -1);

    D(printf("check\n"));

    fclose(input_f);
    fclose(output_f);

    return 0;
}