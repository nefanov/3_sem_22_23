#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>


enum SIGS {

    END_SIG   = 2,
    WRITE_SIG = 45,
    READ_SIG  = 46,
};

enum BOOL_STATUS {

    WRITE_BOOL  = 1,
    READ_BOOL   = 2,
    END_WR_BOOL = 3,
    END_RD_BOOL = 4
};

short parent_flag     = 1;
short child_flag      = 0;
short parent_flag_end = 0;
short child_flag_end  = 0;

const char *name = "HESOYAM";

const int SIZE = 20480;


void parent_handler (int signo, siginfo_t *siginf, void *context) {
    parent_flag = WRITE_BOOL;
}

void parent_terminator (int signo, siginfo_t *siginf, void *context) {
    parent_flag_end = END_WR_BOOL;
}

void child_handler (int signo, siginfo_t *siginf, void *context) {

    child_flag = READ_BOOL;
    FILE *output = fopen ("out.txt", "a");

    fprintf (output, "%s\n", (char *)siginf->si_value.sival_ptr);
    fclose  (output);

    FILE *log = fopen ("log.txt", "a");
    fprintf (log, "MESSAGE READ %p\n", (char *)siginf->si_value.sival_ptr);
    fclose  (log);

    return;
}

void child_terminator (int signo, siginfo_t *siginf, void *context) {
    child_flag_end = END_RD_BOOL;   
}


int main (void) {
    //can truncate
    FILE *clear = fopen ("log.txt", "w");
    if (!clear) {
        perror ("OPEN LOG ERROR");
        exit (1);
    }
    if (fclose (clear) == EOF) {
        perror ("CLOSE LOG ERROR");
        exit (1);
    }

    clear = fopen ("out.txt", "w");
    if (!clear) {
        perror ("OPEN OUT ERROR");
        exit (1);
    }
    if (fclose (clear) == EOF) {
        perror ("CLOSE OUT ERROR");
        exit (1);
    }
/////////////////////////////////////////////////SHM INIT & MMAP
    int shm_fd = shm_open (name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror ("SHM CREAT ERROR");
        exit (1);
    }

    if (ftruncate (shm_fd, SIZE) < 0) {
        perror ("FTRUNCATE ERROR");
        exit (1);
    }

    void *ptr = mmap (0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == (void *)-1) {
        perror ("MMAP ERROR");
        exit (1);
    }

/////////////////////////////////////////////////START FORK
    pid_t ppid = getpid ();
    pid_t cpid = fork ();

    if (cpid < 0) {
        perror ("FORK ERROR");
        exit (1);
    }

/////////////////////////////////////////////////PARENT
    if (cpid > 0)  {
        size_t size = 0;
        
        sigset_t mask;

        sigemptyset (&mask);
        sigaddset   (&mask, WRITE_SIG);
        sigaddset   (&mask, END_SIG);

        struct sigaction sa_write, sa_end;

        sa_end.sa_sigaction   = parent_terminator;
        sa_end.sa_mask        = mask;
        sa_end.sa_flags       = SA_SIGINFO;
        
        sa_write.sa_sigaction = parent_handler;
        sa_write.sa_mask      = mask;
        sa_write.sa_flags     = SA_SIGINFO;
        
        union sigval val;

        sigaction (END_SIG,   &sa_end,   nullptr);
        sigaction (WRITE_SIG, &sa_write, nullptr);

        FILE *log = nullptr;

        while (1) {

            if (parent_flag == WRITE_BOOL) {
                
                
                if (scanf ("%[^\n]", (char *)(ptr + size)) == 1) {

                    while (getchar () != '\n');
                    val.sival_ptr = ptr + size;
                    parent_flag = 0;

                    log = fopen ("log.txt", "a");
                    fprintf (log, "MESSAGE WRITTEN: MSG:\"%s %p\"\n", (char*)(ptr + size), (char*)(ptr + size));
                    fclose (log);
                    
                    size += strlen ((char *)(ptr + size));
                    if (size >= SIZE) size = 0;

                    sigqueue (cpid, READ_SIG, val);
                }

            }

            if (parent_flag_end == END_WR_BOOL) {

                log = fopen ("log.txt", "a");
                fprintf (log, "parent dead\n");
                fclose  (log);

                munmap     (ptr, SIZE);
                shm_unlink (name);

                kill    (cpid, END_SIG);
                
                int status = 0;
                waitpid (cpid, &status, 0);
                if (status != 0) {
                    perror ("CHILD DEATH ERROR");
                    exit (1);
                }

                exit (0);
            }
        }
    }

/////////////////////////////////////////////////CHILD
    else {
        FILE* log = nullptr;

        sigset_t mask;
        sigemptyset (&mask);
        sigaddset   (&mask, END_SIG);
        sigaddset   (&mask, READ_SIG);

        struct sigaction sa_end, sa_read;
        
        sa_end.sa_sigaction = child_terminator;
        sa_end.sa_mask = mask;
        sa_end.sa_flags = SA_SIGINFO;

        sa_read.sa_sigaction = child_handler;
        sa_read.sa_mask = mask;
        sa_read.sa_flags = SA_SIGINFO;
        
        sigaction (END_SIG,  &sa_end,  nullptr);
        sigaction (READ_SIG, &sa_read, nullptr);

        while (1) {

            if (child_flag == READ_BOOL) {
                
                child_flag = 0;
                kill (ppid, WRITE_SIG);
            }

            if (child_flag_end == END_RD_BOOL) {

                log = fopen ("log.txt", "a" );
                fprintf (log, "child dead\n");
                fclose (log);

                munmap (ptr, SIZE);
                shm_unlink (name);

                // kill (ppid, SIGCHLD);
                exit (0);
            }
        }
    }

    return 0;
}