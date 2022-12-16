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

int cmd_flag = 0;

#define DAEMON_PATH "~/backup/"
#define MAX_LINE 1024
#define MAX_DIFF 6
#define get_dir(pid) char src_path[MAX_LINE] = {};\
    sprintf( src_path, "/proc/%d/cwd/", pid );

void work_handler( int signo ) {

    cmd_flag = 1;
    return;

}

int get_sample ( char* src_path, char* backup_path, int diff, char * file_name ) {

    char cmd[3*MAX_LINE] = {};
    //printf( "src is: %s\n backup is: %s\n", src_path, backup_path);
    sprintf( cmd, "diff %s %s > %sdiffs/%d.patch", backup_path, src_path, DAEMON_PATH, diff);
    //printf( "cmd is: %s\n", cmd);
    system(cmd);
    sprintf( cmd, "echo %s > %sdiffs/%d.txt", file_name, DAEMON_PATH, diff);
    system(cmd);
    /*sprintf( cmd, "patch %s %sdiffs/%d.patch > %stemp.txt", backup_path, DAEMON_PATH, diff, DAEMON_PATH);
    system(cmd);*/
    return 0;

}

int get_patch ( int diff ){

    char backup_path[MAX_LINE] = "";
    sprintf( backup_path, "%sdiffs/%d.txt", DAEMON_PATH, diff);
    FILE* file = fopen( backup_path, "r");
    if ( !file ) {
        
        //printf( "error while opening the file\n");
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
    system( cmd ); //created back_up directory
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

    return 0;

}

int inotify_ctor( pid_t pid ) {

    get_dir( pid )

    int fd = inotify_init();//, watch_d = 0;
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

int monitor( int fd, int watch_d, int pid, int *diff ) {

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
                //get sample
                strcat( backup_path, event->name );
                strcat( src_path, event->name );
                //printf( "src is: %s\n backup is: %s\n", src_path, backup_path);
                get_sample( src_path, backup_path, (*diff)%MAX_DIFF, event->name );
                //printf("diff num is -> %d, %d\n", diff, diff%MAX_DIFF );
                (*diff)++;
                src_path[src_size] = '\0';
                backup_path[path_size] = '\0';
                //printf ( "%s was modified mayhem\n", event->name );
            } 

        }

        i+= (sizeof(struct inotify_event) + event->len);

    }

    return 0;

}

int main( int argc, char* argv[] ) {

    pid_t proc = 0;
    scanf( "%d", &proc );
    printf( "%d\n", proc);

    char cmd[MAX_LINE] = {};

    printf( "debug\n");
    if ( back_up( proc, DAEMON_PATH ) == -1 ) {

        printf( "debug\n");
        return -1;

    }
    int fd = inotify_ctor( proc );
    int watch_d = set_watch( fd, proc);
    int count = 0, diff = 0;
    while ( 1 ){ 

        if ( count % 150 == 0 ) {
            monitor( fd, watch_d, proc, &diff);
            count = 0;
        }

        count++;

    }

    return 0;

}