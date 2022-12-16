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

#define NAME "SUNSETSHIMMER"
#define SIZE 10240
#define LOG "log.txt"
#define OUT "out.txt"
#define END_SIG 2
#define WRITE_SIG 45
#define READ_SIG 46

int write_flag = 1;
int read_flag = 0;
int end_child_flag = 0;
int end_parent_flag = 0;

void parent_sig_handler( int signo, siginfo_t *siginf, void *context ) {

    write_flag = 1;
    return;

}

void child_sig_handler( int signo, siginfo_t *siginf, void *context ) {

    read_flag = 1;
    FILE* output = fopen( OUT,"a" );
    fprintf( output, "%s\n", (char*)siginf->si_value.sival_ptr );
    fclose( output );
    FILE* log = fopen( LOG, "a" );
    fprintf( log, "message was read\n" );
    fclose( log );    
    return;

}

void end_sig_handler( int signo, siginfo_t *siginf, void *context ) {

    end_parent_flag = 1;
    return;

}

void child_end_handler( int signo, siginfo_t *siginf, void *context ) {

    end_child_flag = 1;
    return;

}

int main(){

    int shm_fd = 0;

    void* ptr;

    shm_fd = shm_open( NAME, O_CREAT | O_RDWR, 0666 );
    ftruncate(shm_fd, SIZE);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if ( ptr == MAP_FAILED ) {
        perror("mmap");
        return -1;
    } 

    union sigval val;

    pid_t parent_pid = getpid();
    pid_t child_pid = fork();

    if ( child_pid == -1 ) {
        perror("fork");
        exit(1);
    }   else if ( child_pid > 0 ) { //we are in parent

        size_t size = 0;
        FILE* log = NULL;//fopen( LOG, "w" );;
        sigset_t mask;
        sigemptyset( &mask );
        sigaddset( &mask, END_SIG );
        sigaddset( &mask, WRITE_SIG );
        struct sigaction sa_end, sa_write;
        sa_end.sa_sigaction = end_sig_handler;
        sa_end.sa_mask = mask;
        sa_end.sa_flags = SA_SIGINFO;
        sa_write.sa_sigaction = parent_sig_handler;
        sa_write.sa_mask = mask;
        sa_write.sa_flags = SA_SIGINFO;
        sigaction( END_SIG, &sa_end, NULL );
        sigaction( WRITE_SIG, &sa_write, NULL );
        while ( 1 ) {

            if ( write_flag == 1) {
                
                if ( scanf( "%[^\n]", (char*)(ptr + size) ) == 1 ){
                    
                    while( getchar() != '\n' );
                    val.sival_ptr = ptr + size;
                    write_flag = 0;
                    log = fopen( LOG, "a" );
                    fprintf( log, "message was written, message is :\"%s\"\n", (char*)(ptr + size));
                    fclose( log );
                    size += strlen( (char*)(ptr + size) );
                    if ( size >= SIZE ) size = 0;
                    //printf( "sending read signal to %u\n", child_pid );
                    sigqueue( child_pid, READ_SIG, val );
                }

            }

            if ( end_parent_flag == 1 ) {
            
                log = fopen( LOG, "a" );
                fprintf( log, "parent work is done\n");
                fclose( log );
                end_child_flag = 1;
                end_parent_flag = 0;
                write_flag = 0;
                printf("%d\n", end_child_flag);
                int status = 0;
                waitpid( child_pid, &status, 0);
                kill( child_pid, END_SIG );
                return 0;

            }

        }


    } else { //we are in child

        close( 0 );
        close( 1 );
        close( 2 );
        FILE* log = NULL;
        sigset_t mask;
        sigemptyset( &mask );
        sigaddset( &mask, END_SIG );
        sigaddset( &mask, READ_SIG );
        struct sigaction sa_end, sa_read;
        sa_end.sa_sigaction = child_end_handler;
        sa_end.sa_mask = mask;
        sa_end.sa_flags = SA_SIGINFO;
        sa_read.sa_sigaction = child_sig_handler;
        sa_read.sa_mask = mask;
        sa_read.sa_flags = SA_SIGINFO;
        sigaction( END_SIG, &sa_end, NULL );
        sigaction( READ_SIG, &sa_read, NULL );

        while( 1 ) {

            if ( read_flag == 1) {
                
                read_flag = 0;
                kill( parent_pid, WRITE_SIG );

            }   

            if ( end_child_flag == 1 ) {
                
                log = fopen( LOG, "a" );
                fprintf( log, "child work is done\n");
                fclose( log );
                shm_unlink( NAME );
                munmap( ptr, SIZE );
                kill( parent_pid, SIGCHLD );
                return 0;

            }

        }


    }

    return 0;

}