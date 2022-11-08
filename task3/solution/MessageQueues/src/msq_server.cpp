#include "msq.h"

// #define DEBUG

#ifdef DEBUG
    #define D(...) __VA_ARGS__
#else
    #define D(...)
#endif

int main(int argc, char **argv) {
    size_t BUF_SIZE = 0;

    if (argc != 3) {
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

    // FILE *output_f = fopen(argv[3], "w");
    // ASSERTED(fopen, output_f, NULL, -1);

    key_t key = ftok(FILENAME, 0);
    ASSERTED(ftok, key, -1, -1);

    int msq_id = msgget(key, IPC_CREAT | 0666);
    ASSERTED(msgget, msq_id, -1, -1);

    D(printf("fSize = %ld\n", f_size));

    struct my_msg {
        long mtype = 1;
        char msg[BIG] = {};
    };

    my_msg msg_buf;

    if (BUF_SIZE > f_size) {
        
        fread(msg_buf.msg, sizeof(char), f_size, input_f);
        
        D(printf("Fc %c\n", msg_buf.msg[1]));
        D(printf("msgid: %d\n", msq_id));

        int result = msgsnd(msq_id, &msg_buf, BUF_SIZE, 0);
        
        D(ASSERTED(msgsnd, result, -1, -1));
    } else {
        // extern msg_buf;

        for (size_t i = 0; i < f_size / BUF_SIZE; i++) {
            fread(msg_buf.msg, sizeof(char), BUF_SIZE, input_f);
            msgsnd(msq_id, &msg_buf, BUF_SIZE, 0);

            D(printf("working server\n"));
        }

        if (f_size % BUF_SIZE != 0)
            fread(msg_buf.msg, sizeof(char), f_size % BUF_SIZE, input_f);
            msgsnd(msq_id, &msg_buf, f_size % BUF_SIZE, 0);
    }

    // int result = shmdt(shm_p);
    // ASSERTED(shmdt, result, -1, -1);

    // result = shmctl(shm_id, IPC_RMID, NULL);
    // ASSERTED(shmctl, result, -1, -1);

    fclose(input_f);

    return 0;
}