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

enum MY_SIG
{
    WRITE_SIG = 45,
    READ_SIG  = 46,
    END_SIG   = 2
};

enum BOOL_STATUS
{
    WRITE_BOOL  = 1,
    READ_BOOL   = 2,
    END_WR_BOOL = 3,
    END_RD_BOOL = 4
};

short parent_flag     = 1;
short child_flag      = 0;
short parent_flag_end = 0;
short child_flag_end  = 0;

const char* name = "HESOYAM";

#define SIZE 20480

void parent_sigHandler(int signo, siginfo_t* siginf, void* context)
{
    parent_flag = WRITE_BOOL;
}

void parent_terminationHandler(int signo, siginfo_t* siginf, void* context)
{
    parent_flag_end = END_WR_BOOL;
}

void child_sigHandler(int signo, siginfo_t* siginf, void* context)
{
    child_flag = READ_BOOL;
    FILE* output = fopen("out.txt", "a");

    fprintf(output, "%s\n", (char*)siginf->si_value.sival_ptr );
    fclose(output);

    FILE* log = fopen("log.txt", "a" );
    fprintf(log, "message was read\n" );
    fclose(log);

    return;
}

void child_terminationHandler(int signo, siginfo_t* siginf, void* context)
{
    child_flag_end = END_RD_BOOL;   
}


int main(int argc, const char* argv[])
{
    int shm_fd = -1;
 
    void* ptr = NULL;
 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
 
    ftruncate(shm_fd, SIZE);
 
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    pid_t parent_pid = getpid();
    pid_t child_pid  = fork();

    if (child_pid == -1)
    {
        perror("fork:");
        exit(1);
    }

    if (child_pid > 0) 
    {
        size_t size = 0;
        
        sigset_t mask;

        sigemptyset(&mask);
        sigaddset(&mask, WRITE_SIG);
        sigaddset(&mask, END_SIG);

        struct sigaction sa_write, sa_end;

        sa_end.sa_sigaction   = parent_terminationHandler;
        sa_end.sa_mask        = mask;
        sa_end.sa_flags       = SA_SIGINFO;
        sa_write.sa_sigaction = parent_sigHandler;
        sa_write.sa_mask      = mask;
        sa_write.sa_flags     = SA_SIGINFO;
        
        union sigval val;

        sigaction(END_SIG,   &sa_end,   NULL);
        sigaction(WRITE_SIG, &sa_write, NULL);

        FILE* log = NULL;

        while (1) {

            if (parent_flag == WRITE_BOOL) {
                
                if ( scanf( "%[^\n]", (char*)(ptr + size) ) == 1 ){
                    
                    while( getchar() != '\n' );
                    val.sival_ptr = ptr + size;
                    parent_flag = 0;
                    log = fopen("log.txt", "a" );
                    fprintf(log, "MESSAGE WAS WRITTEN, MSG:\"%s\"\n", (char*)(ptr + size));
                    fclose(log);
                    size += strlen( (char*)(ptr + size) );
                    if ( size >= SIZE ) size = 0;
                    sigqueue( child_pid, READ_SIG, val);
                }

            }

            if ( parent_flag_end == END_WR_BOOL ) 
            {
                log = fopen("log.txt", "a" );
                fprintf( log, "parent work is done\n");
                fclose( log );
                child_flag_end  = END_RD_BOOL;
                parent_flag_end = 0;
                parent_flag     = 0;
                int status = 0;
                waitpid(child_pid, &status, 0);
                kill(child_pid, END_SIG);
                return 0;
            }
        }
    } 
    else
    {
        close(0);
        close(1);
        close(2);
        FILE* log = NULL;
        
        sigset_t mask;
        sigemptyset( &mask );
        sigaddset( &mask, END_SIG );
        sigaddset( &mask, READ_SIG );

        struct sigaction sa_end, sa_read;
        
        sa_end.sa_sigaction = child_terminationHandler;
        sa_end.sa_mask = mask;
        sa_end.sa_flags = SA_SIGINFO;
        sa_read.sa_sigaction = child_sigHandler;
        sa_read.sa_mask = mask;
        sa_read.sa_flags = SA_SIGINFO;
        
        sigaction(END_SIG,  &sa_end,  NULL);
        sigaction(READ_SIG, &sa_read, NULL);

        while(1) 
        {

            if (child_flag == READ_BOOL) {
                
                child_flag = 0;
                kill( parent_pid, WRITE_SIG);
            }   

            if ( child_flag_end == END_RD_BOOL ) 
            {
                
                log = fopen("log.txt", "a" );
                fprintf(log, "child work is done\n");
                fclose(log);
                shm_unlink(name);
                munmap(ptr, SIZE);
                kill(parent_pid, SIGCHLD);
                return 0;
            }
        }
    }

    return 0;
}