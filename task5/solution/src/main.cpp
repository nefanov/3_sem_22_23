#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "DebugFunctions.hpp"
#include "Handlers.hpp"

#define N_PAGE 100
#define PAGE_SIZE 1

#define SIZE N_PAGE * PAGE_SIZE

extern int WRITE_FLAG;
extern int READ_FLAG;
extern int END_FLAG;

int main() {
    int res = 0;

    FILE *log = fopen("/home/oleg/tmp/log.txt", "w");
    ASSERTED(fopen, log, NULL, -1);

    FILE *src = fopen("/home/oleg/tmp/1.txt", "r");
    ASSERTED(fopen, src, NULL, -1);

    FILE *dst = fopen("/home/oleg/tmp/dst.txt", "w");
    ASSERTED(fopen, dst, NULL, -1);

    key_t key = ftok("/home/oleg/tmp/log.txt", 0);
    ASSERTED(ftok, key, -1, -1);

    int shmId = shmget(key, SIZE, IPC_CREAT | 0666);
    ASSERTED(shmget, shmId, -1, -1);

    int pPid = getpid();

    int cPid = fork();
    ASSERTED(fork, cPid, -1, -1);

    if (cPid == 0) {
        // Child process

        void *shmP = shmat(shmId, NULL, 0);
        ASSERTED(shmat, shmP, NULL, -1);

        struct sigaction act;
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction  = &ChildHandler;

        struct sigaction act_end;
        act_end.sa_handler  = &EndHandler;

        sigaction(SIGUSR1, &act, NULL);
        sigaction(SIGUSR2, &act_end, NULL);

        while (1) {
            if (READ_FLAG == 1 && END_FLAG == 0) {
                READ_FLAG = 0;

                union sigval value = {0};

                res = fwrite(shmP, 1, PAGE_SIZE, dst);
                ASSERTED(fwrite, res, 0, -1);
                sigqueue(pPid, SIGUSR1, value);
            } else if (END_FLAG == 1) {
                FILE *log = fopen("/home/oleg/tmp/log.txt", "a");
                ASSERTED(fopen, log, NULL, -1);

                fprintf(log, "Файл был полностью прочитан\n");   

                fclose(log);   
                
                shmdt(shmP);
                shmctl(shmId, IPC_RMID, NULL);
                
                fclose(dst);

                break;
            } 
        }

        kill(pPid, SIGCHLD);
    } else {
        // Parent process

        int nPage = 1;
        int offset = 0;

        void *shmP = shmat(shmId, NULL, 0);
        ASSERTED(shmat, shmP, NULL, -1);

        struct sigaction act;
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction  = &ParentHandler;

        sigaction(SIGUSR1, &act, NULL);

        union sigval value = {0};

        while (1) {
            if (WRITE_FLAG == 1 && END_FLAG == 0) {
                WRITE_FLAG = 0;

                value.sival_int = nPage;

                res = fread(shmP, 1, PAGE_SIZE, src);

                if (res == 0) {
                    FILE *log = fopen("/home/oleg/tmp/log.txt", "a");
                    ASSERTED(fopen, log, NULL, -1);

                    fprintf(log, "Файл был полностью записан\n");

                    fclose(log);

                    shmdt(shmP);
                    
                    fclose(src);

                    break;
                } else {
                    FILE *log = fopen("/home/oleg/tmp/log.txt", "a");
                    ASSERTED(fopen, log, NULL, -1);

                    fprintf(log, "Была совершена запись на страницу %d со смещением %d\n", nPage, offset);

                    fclose(log);

                    nPage++;

                    sigqueue(cPid, SIGUSR1, value);
                } 
            } 
        }
        sigqueue(cPid, SIGUSR2, value);

        wait(NULL);
    }
    
}