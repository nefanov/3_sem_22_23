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
        
        proc = atoi(argv[1]);
        printf("proc pid is %d\n", proc);
        //while ( !scanf( "%d", &proc ) );
        close(1);
        close(0);
        close(2);
        umask(0);
        chdir("/");
        if ( back_up( proc, DAEMON_PATH ) == -1 ){
            return -1;
        }

        int fd = inotify_ctor( proc );
        int watch_d = set_watch( fd, proc );
        int count = 0, diff = 0;
        while ( 1 ){ 

            if ( count % 150 == 0 ) {
                monitor( fd, watch_d, proc, &diff);
                count = 0;
            }

            count++;

        }

    }

    return 0;

}