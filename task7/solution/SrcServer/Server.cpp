#include "Config.hpp"
#include "Threads.hpp"
#include "Handlers.hpp"

int main() {
    mknod(CMD_FIFO, S_IFIFO | 0666, 0);

    int cmdFd = open(CMD_FIFO, O_RDWR); 
    if (cmdFd < 0) {
        fprintf(stderr, "ERROR WHILE CREATING NAMED PIPE\n");

        return -1;
    }

    mknod(ACK_FIFO, S_IFIFO | 0666, 0);

    int ackFd = open(ACK_FIFO, O_RDWR );
    if (ackFd < 0) { 
        fprintf(stderr, "ERROR WHILE CREATING NAMED PIPE\n");
        return -1;
    }

    epoll_event event = {};
    epoll_event events[MAX_N] = {};

    int epollFD = epoll_create1(0);
    ASSERTED(epoll_create1, epollFD, -1, -1);

    event.events = EPOLLIN;
    event.data.fd = cmdFd;
    if (epoll_ctl( epollFD, EPOLL_CTL_ADD, cmdFd, &event )) {
        perror( "epoll_ctl" );
        
        return -1;
    }


    int event_count = 0;

    int cmdSet[MAX_N] = {};
    int fileSet[MAX_N] = {};

    int plc = 0, ret = 0;
    
    struct pollfd fds[MAX_N] = {};
    
    pthread_t tid;
    ThreadData transmitData[MAX_N] = {}; 

    while (1) {
        event_count = epoll_wait(epollFD, events, MAX_N, TIMEOUT);

        if (event_count == -1) {
            perror("epoll_wait");
            
            return -1;
        }

        if (event_count > 0) {
            for( int i = 0; i < event_count; i++ ) {
                printf("Handling fd: %u\n", events[i].data.fd );
                
                plc += RegHandle(cmdFd, ackFd, cmdSet, fileSet, plc, fds);
                
                printf("Is handled\n");
            }
        }

        if (plc > 0) {
            ret = poll(fds, plc, TIMEOUT);
            if (ret > 0) {
                for(int i = 0; i < plc; i++) {
                    if ( fds[i].revents & POLLIN ) {
                        printf("handling %u\n", fds[i].fd );

                        transmitData[i].rFD = fds[i].fd;
                        transmitData[i].wFD = fileSet[i];

                        pthread_create( &tid, NULL, ThreadFunc, (void*)(transmitData + i) );
                    }
                }
            }

            else if (ret < 0){
                perror( "poll" );
            }
        }
    }
    return 0;
}   