#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/inotify.h>

#ifndef INCLUDED
#define INCLUDED

#define DAEMON_PATH "~/backup/"
#define MAX_LINE 1024
#define MAX_DIFF 6
#define DEFAULT_T 2 
#define get_dir(pid) char src_path[MAX_LINE] = {};\
    sprintf( src_path, "/proc/%d/cwd/", pid );

#define FIFO "lucifer.fifo"

enum CMD {

    stop     = 0,
    show     = 1,
    time_set = 2,

};

int DO_WORK = 1;
int cmd_flag = 0;

void stop_handler( int signo ) {

    DO_WORK = 0;
    return;
}

void cmd_handler( int signo ) {

    cmd_flag = 1;
    return;

}

void set_signals( void (*handler)(int) ){

    signal(SIGINT,  handler);
    signal(SIGQUIT,  handler);
    signal(SIGABRT,  handler);
    signal(SIGTRAP,  handler);
    signal(SIGTERM,  handler);
    signal(SIGCONT,  handler);
    signal(SIGTSTP,  handler);

    return;

}

int get_sample ( char* src_path, char* backup_path, int diff, char * file_name ) {

    char cmd[3*MAX_LINE] = {};
    sprintf( cmd, "diff %s %s > %sdiffs/%d.patch", backup_path, src_path, DAEMON_PATH, diff);
    system(cmd);
    sprintf( cmd, "echo %s > %sdiffs/%d.txt", file_name, DAEMON_PATH, diff);
    system(cmd);
    return 0;

}

int get_patch ( int diff ){

    char backup_path[MAX_LINE] = "";
    sprintf( backup_path, "%sdiffs/%d.txt", DAEMON_PATH, diff);
    FILE* file = fopen( backup_path, "r");
    if ( !file ) {

        return -1;

    }
    fread( backup_path, sizeof(char), MAX_LINE, file );
    char cmd[3*MAX_LINE] = {};

    sprintf( cmd, "patch %s%s %sdiffs/%d.patch > %stemp.txt", DAEMON_PATH, backup_path, DAEMON_PATH, diff, DAEMON_PATH);
    printf( "%s\n", cmd);
    system(cmd);
    fclose( file );
    return 0;

}

int is_txt ( char* name ) {

    char cmd[MAX_LINE] = "";

    sprintf( cmd,"file %s | grep :*text > %stemp.txt", name, DAEMON_PATH );
    if ( !system(cmd) ) return 1;

    return 0;

}

int back_up ( const int pid ,const char* res_path ) {

    char src_path[MAX_LINE] = {};
    sprintf( src_path, "/proc/%d/cwd/", pid );
    char cmd[5*MAX_LINE] = {};
    sprintf( cmd, "mkdir %s", res_path );
    system( cmd ); 
    sprintf( cmd, "mkdir %sdiffs/", res_path );
    system( cmd );
    char src_file[2*MAX_LINE] = {};
    char res_file[2*MAX_LINE] = {};
    size_t des_len = strlen(src_path), res_len = strlen(res_path);
    sprintf(res_file, "%s", res_path);
    sprintf(src_file, "%s", src_path);
    DIR* dir = NULL;
    dir = opendir( src_path );
    struct dirent *dp;

    if ( dir == NULL ) {

        return -1;
    
    }

    while ( 1 ) {

        dp = readdir( dir );

        if ( dp == NULL )
            break;

        if ( dp->d_type != DT_REG )
            continue;
        strcat(res_file, dp->d_name);
        strcat(src_file, dp->d_name);
        if ( is_txt( src_file ) ) { 
            sprintf( cmd, "cp %s %s", src_file, res_file);
            system( cmd );
        }
        src_file[des_len] = '\0';
        res_file[res_len] = '\0';
    
    }
    closedir( dir );
    return 0;

}

int inotify_ctor( pid_t pid ) {

    get_dir( pid )

    int fd = inotify_init();
    fcntl( fd, F_SETFL, O_NONBLOCK );
    if ( fd == -1 ){

        printf( " inotify ctor error\n");
        return -1;

    }
    
    return fd;
}

int set_watch( int fd, int pid ) {

    get_dir( pid )
    int watch_d = inotify_add_watch( fd, src_path, IN_CREATE | IN_DELETE | IN_MODIFY );
    if( watch_d == -1 ) {

        printf( "watch_d error\n");
        return -1;
    }

    return watch_d;
}

int monitor( int fd, int pid, int *diff ) {

    get_dir( pid )
    int i = 0 , sz, path_size = 0, src_size = 0;
    char buf[MAX_LINE] = {};
    char backup_path[MAX_LINE] = DAEMON_PATH;
    char cmd[3*MAX_LINE] = {};
    src_size = strlen( src_path ); 
    path_size = strlen( backup_path );
    sz = read(fd, buf, MAX_LINE);
    while ( i  < sz ) {
       
        struct inotify_event *event = (struct inotify_event*)&buf[i];
        if ( event->len ) {
            
            if ( event->mask & IN_CREATE ) {

                strcat( backup_path, event->name );
                strcat( src_path, event->name );
                if ( is_txt(src_path) ) {

                    sprintf( cmd, "cp %s %s", backup_path, src_path);
                    system(cmd);
                    cmd[0] = '\0';
                
                }
                backup_path[path_size] = '\0';
                src_path[src_size] = '\0';

            }   else if ( event->mask & IN_DELETE ) {

                strcat( src_path, event->name );
                strcat( backup_path, event->name );
                if ( is_txt(src_path) ) {
                    sprintf( cmd, "rm %s", backup_path );
                    system(cmd);
                    cmd[0] = '\0';
               
                }
                src_path[src_size] = '\0';
                backup_path[path_size] = '\0';

            } else if ( event->mask & IN_MODIFY ) {
                strcat( backup_path, event->name );
                strcat( src_path, event->name );
                get_sample( src_path, backup_path, (*diff)%MAX_DIFF, event->name );
                (*diff)++;
                src_path[src_size] = '\0';
                backup_path[path_size] = '\0';
            } 

        }

        i+= sizeof(struct inotify_event) + event->len;

    }

    return 0;

}

int fifo_ctor( );

int print_diff( int diff, int k) {

    if ( diff > k ){
        return -1;
    }
    char cmd[MAX_LINE] = {};
    sprintf( cmd, "./diff_print %d", diff);
    printf( "%s\n", cmd );
    system(cmd);
    return 0;
}

int func_verify( char* cmd ) {

    if ( !strcmp( cmd, "print" ) ) return show;
    if ( !strcmp( cmd, "set_t" ) ) return time_set;
    if ( !strcmp( cmd, "stop" ) ) return stop;
    return -1; 

}

#endif