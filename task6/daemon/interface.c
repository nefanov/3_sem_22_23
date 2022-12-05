#include "config.h"

int main() {

    int do_work = 1, code = 0, diff = 0, time = 0;
    pid_t daemon = 0;
    printf( "Hello, this is the interface for daemon\n" );
    printf( "Please, enter the daemon pid\n" );
    scanf( "%d", &daemon );
    char cmd[MAX_LINE] = {};
    mknod(FIFO, S_IFIFO | 0666, 0);
    int fd = open(FIFO, O_WRONLY);

     if ( fd < 0 ) {
        printf("%s\n", "ERROR WHILE CREATING NAMED PIPE" );
        return -1;
    }

    printf( "Please, enter the command\n" );
    
    while ( do_work ){

        scanf( "%s", cmd );
        code = func_verify( cmd );

        switch ( code ) {
            case stop:
                do_work = 0;
                kill( daemon, SIGUSR1 );
                break;
            case show:
                printf( "Please enter how many diffs you want to see\n" );
                scanf( "%d", &diff);
                //this variation doesn't work due to undefined circumstances
                /*kill( daemon, SIGUSR2 );
                write( fd, &code, sizeof(int) );
                write( fd, &diff, sizeof(int) );*/
                
                sprintf( cmd, "./diff_print %d", diff);
                system(cmd);
                break;
            case time_set:
                printf( "Please enter how many seconds you want daemon to sleep\n" );
                scanf( "%d", &time );
                kill( daemon, SIGUSR2 );
                write( fd, &code, sizeof(int) );
                write( fd, &time, sizeof(int) );
                
                break;
            default:
                printf( "Unknown command\n");
                break;
        }

    }
    close(fd);
    return 0;

}

