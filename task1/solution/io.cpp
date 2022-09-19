#include "io.hpp"

void input(char **str) {
    assert(str);

    *str = (char *) calloc(N_TTY_BUF_SIZE, sizeof(char));
    assert(*str);

    fgets(*str, N_TTY_BUF_SIZE, stdin);
    assert(*str);

    void *tmp_p = realloc(*str, strlen(*str) + 1);
    if (tmp_p) {
        *str = (char *) tmp_p;
    } else {
        perror("realloc() failed");
        return;
    }
}   