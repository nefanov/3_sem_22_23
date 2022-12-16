#include "config.h"

int reg_handle( int cmd_fd, int ack_fd, int* cmd_set, int* file_set, int plc, struct pollfd* fds ) {

    if ( plc >= MAX_N ) {

        printf( "Error, maximum of users is reached\n" );
        return 0;
    
    }

    char buf[MAX_SIZE] = {};
    char cmd[MAX_SIZE] = {};
    read( cmd_fd, buf, MAX_SIZE );
    int size = strlen(buf), offset = 0;
    offset = skip_spaces( buf );
    sscanf( buf + offset, "%s", cmd );

    if ( !strcmp( cmd, REG ) ) {

        offset += REG_SZ;
        offset += skip_spaces( buf + offset );
        
        if ( (size - offset) <= 0 ) {
        
            printf( "invalid cmd, fifo wasn't inputed\n" );
            write( ack_fd, "IDK", 4 );
            return 0;
        }
        
        sscanf( buf + offset, "%s", cmd );//get file fd 
        mknod( cmd, S_IFIFO | 0666, 0 );
        cmd_set[plc] = open( cmd, O_RDWR );

        if ( cmd_set[plc] < 0 ) { 

            fprintf( stderr, "ERROR WHILE OPENING PIPE\n" );
            write( ack_fd, "IDK", 4 );
            return 0;

        }
        
        offset += strlen( cmd );
        offset += skip_spaces( buf + offset );
        size -= offset;

        if ( size <= 0 ) {

            close( cmd_set[plc] );
            printf ( "invalid cmd, file fifo wasn't inputed\n" );
            write( ack_fd, "IDK", 4 );
            return 0;

        }

        sscanf( buf + offset, "%s", cmd );
        mknod( cmd, S_IFIFO | 0666, 0 );
        file_set[plc] = open( cmd, O_RDWR );

        if ( file_set[plc] < 0 ) { 

            close( cmd_set[plc] );
            fprintf( stderr, "ERROR WHILE OPENING PIPE\n" );
            write( ack_fd, "IDK", 4 );
            return 0;

        }

        fds[plc].fd = cmd_set[plc];
        fds[plc].events = POLLIN;

        write( ack_fd, "ACK", 4 );

        return 1;

    } 
    
    write( ack_fd, "IDK", 4 );
    return 0;

}

typedef struct thread_data_t {

    int r_fd;
    int w_fd;

} thread_data;

void* thread_func( void* transmit_data ) {

    if( !transmit_data ) {
        
        printf( "ERROR, DATA NULL PTR\n" );
        return NULL;

    }

    thread_data* data = (thread_data*)transmit_data; 
    char cmd[MAX_SIZE] = {};
    char parse[MAX_SIZE] = {};
    int size = read( data->r_fd, cmd, MAX_SIZE);
    sscanf( cmd, "%s", parse );
    if( !strcmp(parse, "GET") ) {

        size -= 3;

        if ( size <= 0 ) {
            write( data->w_fd, "IDK", 4 );
            printf( "invalid input, no filename\n" );
            return NULL;

        }

        sscanf( cmd + 3, "%s", parse );

        FILE *input = fopen( parse, "r" );

        if ( !input ) {

            printf ( "No such file\n" );
            write( data->w_fd, "IDK", 4 );
            return NULL;

        } 
        write( data->w_fd, "ACK", 4 );
        while( ( size = fread( parse, sizeof(char), MAX_SIZE - 1, input ) ) > 0 ) {

            parse[ size ] = '\0';
            write( data->w_fd, parse, size + 1 );
            read( data->r_fd, parse, 3);
        }
        write( data->w_fd, "ACK", 4 );
        printf( "work is done\n");
        fclose( input );
        return NULL;

    }

    printf( "unknown command\n" );
    write( data->w_fd, "IDK", 4 );
    return NULL;

}


int main() {

    mknod( CMD_FIFO, S_IFIFO | 0666, 0 );
    int cmd_fd = open( CMD_FIFO, O_RDWR ); 
    
    if ( cmd_fd < 0 ) {
        fprintf ( stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        return -1;
    }

    mknod( ACK_FIFO, S_IFIFO | 0666, 0 );
    int ack_fd = open( ACK_FIFO, O_RDWR );

    if ( ack_fd < 0 ) { 
        fprintf ( stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        return -1;
    }

    int epoll_fd = 0;
    struct epoll_event event;
    struct epoll_event events[MAX_N];
    epoll_fd = epoll_create1( 0 );

    if  ( epoll_fd == -1 ) {

        perror ( "epoll open" );
        return -1;

    }

    event.events = EPOLLIN;
    event.data.fd = cmd_fd;

    if ( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, cmd_fd, &event ) ) {

        perror( "epoll_ctl" );
        return -1;

    }


    int event_count = 0;
    int cmd_set[MAX_N] = {};
    int file_set[MAX_N] = {};
    int plc = 0, ret = 0;
    struct pollfd fds[MAX_N] = {};
    pthread_t tid;
    thread_data transmit_data[MAX_N] = {}; 

    while ( 1 ) {
        
        event_count = epoll_wait( epoll_fd, events, MAX_N, TIMEOUT );

        if ( event_count == -1 ) {

            perror ( " epoll_wait " );
            return -1;
        }

        if ( event_count > 0 ) {

            for( int i = 0; i < event_count; i++ ) {

                printf( "Handling fd: %u\n", events[i].data.fd );
                plc += reg_handle( cmd_fd, ack_fd, cmd_set, file_set, plc, fds );
                printf( "Is handled\n" );

            }

        }

        if ( plc > 0 ) {

            ret = poll( fds, plc, TIMEOUT );

            if ( ret > 0 ) {

                for( int i = 0; i < plc; i++ ) {
                    
                    if ( fds[i].revents & POLLIN ) {
                        
                        printf( "handling %u\n", fds[i].fd );
                        transmit_data[i].r_fd = fds[i].fd;
                        transmit_data[i].w_fd = file_set[i];
                        pthread_create( &tid, NULL, thread_func, (void*)(transmit_data + i) );

                    }

                }

            }

            else if ( ret < 0 ){

                perror( "poll" );

            }

        }

    }

    return 0;
}   