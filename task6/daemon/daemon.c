#include "config.h"

int main( int argc, char* argv[] ) {

    pid_t pid = 0;
    pid = fork();
    if ( pid == -1 ) {
        perror("fork");
        exit(1);
    } else if ( pid > 0 ) { //parent
        
        signal( SIGCHLD, SIG_IGN );

    } else {

        int sid = setsid();
        if ( sid == -1 ) {
            perror("setsid");
            exit(1);
        }
        printf("daemon pid is %d\n", getpid());
        pid_t proc = 0;
        if ( argc < 2 ) {
            printf( "pid wasn't inputed!\n");
            return -1;
        }
        proc = atoi(argv[1]);
        close(0);
        umask(0);
        set_signals( SIG_IGN );
        signal( SIGUSR1, stop_handler );
        if ( back_up( proc, DAEMON_PATH ) == -1 ){
            return -1;
        }

        int fd = inotify_ctor( proc );
        int watch_d = set_watch( fd, proc );
        int diff = 0, t = DEFAULT_T;
        if ( argc == 2 || (argc > 2 && !strcmp(argv[2], "-i")) ) {

            int code = 0, k = 0;
            mknod( FIFO, S_IFIFO | 0666, 0 );
            int fd_r = open( FIFO, O_RDONLY );
            if ( fd_r < 0 ){
                printf( "error while creating named pipe\n");
                DO_WORK = 0;
            }
            signal( SIGUSR2, cmd_handler );
            
            while ( DO_WORK ){

                if( cmd_flag ){
                    read( fd_r, &code, sizeof(int) );
                    switch( code ) {

                        case time_set: 
                            read( fd_r, &t, sizeof(int) );
                            break;
                        case show:
                            read( fd_r, &k, sizeof(int) );
                            print_diff( k, diff );
                            break;
                        default:
                            break;
                    }
                }
                sleep(t);
                monitor( fd, proc, &diff);
                
            }

            if ( close(fd_r) ){

                return -1;
            }

            system( "rm lucifer.fifo");

        } else if ( argc >= 3 && !strcmp(argv[2], "-d") ){
            
            close(1);
            close(2);
            chdir("/");
            while ( DO_WORK ){ 
                
                sleep( t );
                monitor( fd, proc, &diff);

            }

        }

        if (inotify_rm_watch(fd, watch_d)) {

            return -1;
        }
        if (close(fd)) {

            return -1;
        }

    }

    return 0;

}