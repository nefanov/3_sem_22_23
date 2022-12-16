#include "config.h"

double integral_func( double x ) {

    return x; 
}

int calculate ( double left, double right ) {

    unsigned int seed = time(NULL);
    double x = 0, y = 0;
    int res = 0;

    for ( int i = 0; i < ACCURACY; i++ ) {

        x = left + (double)(rand_r(&seed) * (right - left)/(double)RAND_MAX);
        y = Y_MIN + (double)(rand_r(&seed) * (Y_MAX - Y_MIN)/(double)RAND_MAX);
        if ( y <= integral_func( x ) ) res++;

    }

    return res;

}



void* handler( void* argv ) {

    int connfd = *((int*)argv), len = 0, offset = 0;
    double left = 0, right = 0, tosend = 0;
    int res = 0;
    char cmd[MAX_LINE] = {};
    char parse[MAX_LINE] = {};

    while( 1 ) {

        len = read( connfd, cmd, MAX_LINE );

        if ( len == -1 ) {

            perror( "read" );
            break;

        }

        cmd[len] = '\0';
        printf( "connections is %u, command is %s\n", connfd, cmd );
        
        if ( !strcmp( cmd, "stop" ) ) {

            printf( "Client %u has lost\n", connfd );
            break;

        } else {

            sscanf( cmd, "%s%n", parse, &offset );
            if ( !strcmp( parse, "calculate" ) ) {

                //offset += 10;
                
                if ( sscanf( cmd + offset, "%lf%lf", &left, &right ) != 2 ) {

                    printf( "cmd is :%s\n", cmd + offset );
                    printf( "error, wrong usage of command calculate on %u\n", connfd );
                    write( connfd, "IDK", 4);

                }

                printf( "left is %lf, right is %lf\n", left, right );
                res = calculate( left, right );
                printf( "res is %d\n", res );
                tosend = ((double)res)/((double)ACCURACY) * (right - left) * (Y_MAX - Y_MIN);
                printf( "to send is %lf\n", tosend );
                memcpy( cmd, &tosend, sizeof(double));
                write( connfd, cmd, MAX_LINE);

            } else write( connfd, "IDK", 4);

        }       

    }

    close( connfd );
    return NULL;

}



int main() {

    int sockfd = 0, connfd = 0;
    struct sockaddr_in srv_addr, clt_addr;

    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {

        perror("socket");
        return -1;

    }

    memset( &srv_addr, 0, sizeof(srv_addr) );
    memset( &clt_addr, 0, sizeof(clt_addr) );
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    srv_addr.sin_port = htons(PORT4);

    if ( bind(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr) ) == -1 ) {

        perror( "bind");
        return -1;

    }

    //accepting listen
    if ( listen(sockfd, MAX_CLT) == -1 ) {

        perror("listen");
        return -1;

    }

    printf( "Listening\n" );
    int len = sizeof(clt_addr), conn_count = 0;
    pthread_t tid = 0;

    while( conn_count < MAX_CLT ) {

        connfd = accept( sockfd, (struct sockaddr*)&clt_addr, &len );

        if ( connfd < 0 ) {

            perror( "accept" );
            return -1;

        }

        printf( "accepted connection\n" );
        conn_count++;
        pthread_create( &tid, NULL, handler, (void*)&connfd );

    }
    
    printf( "close\n" );
    close(sockfd);
    return 0;

}
