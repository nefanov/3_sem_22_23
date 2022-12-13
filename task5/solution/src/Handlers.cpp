#include "Handlers.hpp"

int WRITE_FLAG = 1;
int READ_FLAG = 0;
int END_FLAG = 0;

void ParentHandler(int signo, siginfo_t *siginf, void *context) {
    WRITE_FLAG = 1;
}

void ChildHandler(int signo, siginfo_t *siginf, void *context) {
    READ_FLAG = 1;

    FILE *log = fopen("/home/oleg/tmp/log.txt", "a");

    int nPage = (siginf->si_value).sival_int;

    fprintf(log, "Была прочитана запись со страницы %d со смещением 0\n", nPage);

    fclose(log);
}

void EndHandler(int signo) {
    END_FLAG = 1;
}