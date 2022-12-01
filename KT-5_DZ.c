#include <sys/signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define WritingPageNum 0
#define WritingShiftSize 0
#define MSG_SIZE 13
#define SHM_NAME "HELLO"
#define SIZE 1024
#define Logfile "KT-5_dz-logfile.txt"
#define parent_write_signal 66
#define parent_end_signal 67
#define child_read_signal 66
#define CTRL_C_END 2

siginfo_t siginfo;
typedef int mytype;
int Nnum = 0, Nsize = 0;
int parent_is_ready_to_write = 1;
int parent_wrote_flag = 0;
int child_read_flag = 0;
int prog_is_finished_parent = 0;
int prog_is_finished_child = 0;
void* p = NULL;


void* allocator_for_parent(){
    int shmid;
    int new = 1;
    char pathname[] = "KT-5-DZ.c";
    key_t key;
    void* ptr;
    int shm_fd = 0;
    Nsize = 2;
    Nnum = 2;

    
    printf("\nAllocating %d sheets of %d bytes...\n", Nnum, Nsize);


    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666 );
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED ) {
        perror("mmap");
        return NULL;
    } 

    return ptr;
}


void parent_action_after_writing(int signo, siginfo_t *siginf, void *context ){

    parent_wrote_flag = 1;
    
    return;
}

void child_recieving_signal(int signo, siginfo_t *siginf, void *context){


    FILE* logfile = fopen(Logfile, "a");
    printf(logfile, "CHILD: Recieved message: %s\n", siginf->si_value.sival_ptr);
    fclose(logfile);
    return;
}

void child_is_over(int signo, siginfo_t *siginf, void *context){

    prog_is_finished_child = 1;

    return;
}

void exitingfunc(int signo){
    shm_unlink(p);
    exit(1);
}

int main(){

    char message[] = "Hello hello";
    int shmid;
    int new = 1;
    char pathname[] = "KT-5-DZ.c";
    pid_t pid;
    pid_t parent_pid;

    if((pid = fork()) < 0){
        printf("fork is failed :(\n");
        exit(-1);
    }
    
    parent_pid = getpid();

    if(pid > 0){
        // PARENT PROCESS

        sigset_t mysigset;
        sigemptyset(&mysigset);
        sigaddset(&mysigset, parent_write_signal);
        sigaddset(&mysigset, parent_end_signal);
        struct sigaction sigaction_write, sigaction_end;
        sigaction_write.sa_mask = mysigset;
        sigaction_write.sa_flags = SA_SIGINFO;
        sigaction_write.sa_sigaction = parent_action_after_writing;      //отправка сигнала ребёнку
        sigaction(child_read_signal, &sigaction_write, NULL);


        p = allocator_for_parent();


        scanf("%[^\n]", (char*)(p));

        //запись в LOG
        FILE* logfile = fopen(Logfile, "a");
        fprintf(logfile, "PARENT: Message sended! Message:\n");
        fprintf(logfile, "%s", (char*)p);
        fprintf(logfile, "\n");
        fclose(logfile);

        // сигнал ребёнку

        union sigval val;
        val.sival_ptr = p;
        sigqueue(pid, parent_write_signal, val);


        signal(SIGINT, exitingfunc);
    
        // sig QUEUE???
        // kill(pid, parent_write_signal);
        // Акт отправки окончен
        exit(1);
    }
    else{
        // CHILD PROCESS
        sigset_t mysigset;
        sigemptyset(&mysigset);         
        sigaddset(&mysigset, CTRL_C_END);
        sigaddset(&mysigset, child_read_signal);
        struct sigaction sigaction_end, sigaction_read;
        sigaction_read.sa_mask = mysigset;
        sigaction_read.sa_flags = SA_SIGINFO;
        sigaction_read.sa_sigaction = child_recieving_signal;
        sigaction_end.sa_mask = mysigset;
        sigaction_end.sa_flags = SA_SIGINFO;
        sigaction_end.sa_sigaction = child_is_over;
        sigaction(CTRL_C_END, &sigaction_end, NULL);
        sigaction(parent_write_signal, &sigaction_read, NULL);
        
        while(1){   

            if(parent_wrote_flag){

                parent_wrote_flag = 0;

                kill(parent_pid, child_read_signal);

            }

            if(prog_is_finished_child){
                FILE* logfile = fopen(Logfile, "a");
                fprintf(logfile, "CHILD: Finished work.\n");
                fclose(logfile);
                shm_unlink(SHM_NAME);
                munmap(p, SIZE);
                kill(parent_pid, SIGCHLD);
                exit(1);
            }
        }

    	union sigval value;
        value.sival_ptr = (void*)"ACKnowledgement";

        if (sigqueue(parent_pid, SIGUSR2, value) == -1)
        {
            printf("%d sigqueue", __LINE__)
            exit(1);
        }

            exit(1);
        }
    
    return 0;
}