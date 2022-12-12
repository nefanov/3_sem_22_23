#include "config.h"


int main() {

    mknod( CMD_FIFO, S_IFIFO | 0666, 0 );
    int cmd_fd = open( CMD_FIFO, O_WRONLY ); 
    
    if ( cmd_fd < 0 ) {
        fprintf ( stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        return -1;
    }

    mknod( ACK_FIFO, S_IFIFO | 0666, 0 );
    int ack_fd = open( ACK_FIFO, O_RDONLY );

    if ( ack_fd < 0 ) { 
        fprintf ( stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        return -1;
    }

    char cmd[MAX_SIZE] = {};

    printf( "Please, register to the server\nTo do it type REGISTER \
    <file/tx/path> <fifo/rx/path> on your keyboard\n");
    fgets( cmd, MAX_SIZE, stdin );
    char buf[MAX_SIZE] = {};
    int offset = 0, size = 0;
    write( cmd_fd, cmd, MAX_SIZE );
    read( ack_fd, buf, MAX_SIZE );
    
    if ( strcmp( buf, "ACK" ) ) { 

        printf( "error, bad input\n" );
        return -1;

    }
    offset = skip_spaces( cmd );
    sscanf( cmd, "%s", buf );
    offset += REG_SZ;
    offset += skip_spaces( cmd + offset );
    sscanf( cmd + offset, "%s", buf );
    mknod( buf, S_IFIFO | 0666, 0 );
    int w_fd = open( buf, O_WRONLY );

    if ( w_fd < 0 ) {

        printf( "error while opening pipe\n");
        return -1;

    }
    offset += strlen( buf );
    sscanf( cmd + offset, "%s", buf );
    mknod( buf, S_IFIFO | 0666, 0 );
    int r_fd = open( buf, O_RDONLY );

    if ( r_fd < 0 ) {

        close(w_fd);
        printf( "error while opening pipe\n");
        return -1;
        
    }

    printf( "Registration is successful\n");

    while ( 1 ) {

        fgets( cmd, MAX_SIZE, stdin );
        write( w_fd, cmd, MAX_SIZE );
        read( r_fd, cmd, 4 );
        if ( !strcmp( cmd, "ACK" ) ) {


            while ( ( size = read( r_fd, cmd, MAX_SIZE ) ) > 0 ) {
                if ( !strcmp(cmd, "ACK") ){
                    printf( "That's it\n");
                    break;
                }

                printf( "%s\n", cmd );
                write( w_fd, "ACK", 3);
            }

        }

    }


    return 0;

}