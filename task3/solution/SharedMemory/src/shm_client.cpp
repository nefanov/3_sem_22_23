#include "shm.hpp"

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

    int shm_id = shmget(key, f_size, 0);
    ASSERTED(shmget, shm_id, -1, -1);

    void *shm_p = shmat(shm_id, NULL, 0);
    ASSERTED(shmat, shm_p, (void *) -1, -1);

    D(printf("fSize = %ld\n", f_size));
    D(printf("Fc %c\n", ((char *) shm_p)[0]));

    if (BUF_SIZE > f_size) {
        fwrite(shm_p, sizeof(char), f_size, output_f);
    } else {
        for (size_t i = 0; i < f_size / BUF_SIZE; i++) {
            fwrite(shm_p, sizeof(char), BUF_SIZE, output_f);
        }

        if (f_size % BUF_SIZE != 0)
            fwrite(shm_p, sizeof(char), f_size % BUF_SIZE, output_f);
    }

    int result = shmdt(shm_p);
    ASSERTED(shmdt, result, -1, -1);

    result = shmctl(shm_id, IPC_RMID, NULL);
    ASSERTED(shmctl, result, -1, -1);

    fclose(output_f);

    return 0;
}