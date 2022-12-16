#include "config.h"

int handler( int* sockfd ) {

    char cmd[MAX_LINE] = {};
    double res = 0, left = 0, right = 0, integral = 0;

    while ( 1 ) {

        printf( "Enter the command\n" );
        scanf( "%s", cmd );
        
        if ( !strcmp( cmd, "stop" ) ) {

            return 0;

        }

        if ( !strcmp( cmd, "calculate") ) {

            printf( "Enter the left and right ends of the interval\n" );
            scanf( "%lf%lf", &left, &right );
            integral = 0;

            for ( int i = 0; i < SRV_MAX; i++) {

                sprintf( cmd, "calculate %lf %lf", left + (right-left)/SRV_MAX * i, left + (right - left)/SRV_MAX*(i + 1)  );
                printf( "%s\n", cmd);
                write( sockfd[i], cmd, strlen(cmd) );
                read( sockfd[i], cmd, MAX_LINE );
                
                if ( !strcmp( cmd, "IDK" ) ) printf( "some error on %d server\n", i);
                else {

                    memcpy( &res, cmd, sizeof(double) );
                    integral += res;
                    printf( "integral %d is %lf\n", i, res );

                }

            }

            printf( "integral is %lf\n", integral);

        }   else printf( "Unknown command\n");

    }

    return 0;

}

int main() {

    int sockfd[SRV_MAX] = {};
    struct sockaddr_in srv_addr;

    for ( int i = 0 ; i < SRV_MAX; i++ ) {
        if ( (sockfd[i] = socket( AF_INET, SOCK_STREAM, 0)) == -1 ) {

            perror( "socket");
            return -1;

        }

        memset( &srv_addr, 0, sizeof(srv_addr) );
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_addr.s_addr = inet_addr(CLT_DEFAULT);
        srv_addr.sin_port = htons(PORT1 + i);

        if ( connect(sockfd[i], (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1 ) {

            perror( "connect" );
            return -1;

        }
    }
    printf( "connected\n" );
    handler(sockfd);
    for ( int i = 0; i < SRV_MAX; i++ ){
        close(sockfd[i]);
    }
    return 0;

}