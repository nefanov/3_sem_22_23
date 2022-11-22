#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define NAME "AWESOME"
#define SIZE 4096

static pid_t pid;
static int out_char = 0;
static int counter = 128;

// SIG_CHLD
void exit_child(int signo) 
{
  exit(EXIT_SUCCESS);
}

// SIG_ALRM
void exit_parent(int signo) 
{
  exit(EXIT_SUCCESS);
}

// nothing
void nothing(int signo){}

// SIG_USR1
void got_one(int signo) 
{
  out_char += counter;
  counter /= 2;
  kill(pid, SIGUSR1);
}

// SIG_USR2
void got_zero(int signo) 
{
  counter /= 2;
  kill(pid, SIGUSR1);
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Use: %s [destination]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int shm_fd = shm_open( NAME, O_CREAT | O_RDWR, 0666 );
    void* ptr;

    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if ( ptr == MAP_FAILED ) 
    {
        perror("mmap");
        return -1;
    } 

    // Получаем pid родителя
    pid_t parent_pid = getpid();
    sigset_t set;

    // SIG_CHLD -> exit
    struct sigaction act_exit;
    memset(&act_exit, 0, sizeof(act_exit));
    act_exit.sa_handler = exit_child;
    sigfillset(&act_exit.sa_mask);
    sigaction(SIGCHLD, &act_exit, NULL);

    // SIG_USR1 -> got_one()
    struct sigaction act_one;
    memset(&act_one, 0, sizeof(act_one));
    act_one.sa_handler = got_one;
    sigfillset(&act_one.sa_mask);
    sigaction(SIGUSR1, &act_one, NULL);

    // SIG_USR2 -> got_zero()
    struct sigaction act_zero;
    memset(&act_zero, 0, sizeof(act_zero));
    act_zero.sa_handler = got_zero;
    sigfillset(&act_zero.sa_mask);
    sigaction(SIGUSR2, &act_zero, NULL);

    // Добавляем блокировки
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
    sigemptyset(&set);

    pid = fork();

    // CHILD (sender)
    if (pid == 0) 
    {
        int fd = 0;
        char c = 0;
        sigemptyset(&set);

        // SIG_USR1 -> nothing()
        struct sigaction act_empty;
        memset(&act_empty, 0, sizeof(act_empty));
        act_empty.sa_handler = nothing;
        sigfillset(&act_empty.sa_mask);
        sigaction(SIGUSR1, &act_empty, NULL);

        // SIG_ALRM -> exit_parent()
        struct sigaction act_alarm;
        memset(&act_alarm, 0, sizeof(act_alarm));
        act_alarm.sa_handler = exit_parent;
        sigfillset(&act_alarm.sa_mask);
        sigaction(SIGALRM, &act_alarm, NULL);

        int i;
        size_t size = 0;

        while (read(fd, &c, 1) > 0)
        {
            // SIGALRM Будет получен если родитель не успеет ответить за секунду
            alarm(1);

            // Побитовые операции
            for ( i = 128; i >= 1; i /= 2)
            {
                if ( i & c )              // 1
                    kill(parent_pid, SIGUSR1);
                else                      // 0
                    kill(parent_pid, SIGUSR2);
                
                // Ждём подтверждения от родителя
                // приостанавливает процесс до получения сигнала
                sigsuspend(&set);
            }

            //fprintf(stdout, "message was written, message is :\"%s\"\n", (char*)(ptr + size));
            size += strlen( (char*)(ptr + size));
        }
        // ввод кончился
        exit(EXIT_SUCCESS);
    }

    // Получаем пока ребёнок не умрёт
    FILE *fout = fopen(argv[1], "w");
    do 
    {
        if(counter == 0) 
        {
            write(fileno(fout), &out_char, 1);
            fflush(fout);
            counter=128;
            out_char = 0;
        }
        
        sigsuspend(&set); // Ждём сигнал от ребёнка
    } while (1);

    shm_unlink( NAME );
    munmap( ptr, SIZE );
    kill( parent_pid, SIGCHLD );
    exit(EXIT_SUCCESS);
}