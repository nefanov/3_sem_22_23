#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <time.h>
#include <errno.h>
#include "../include/query.hpp"


const char *GLOBAL_BROADCAST_ADDR = "255.255.255.255";
const char *LOCAL_BROADCAST_ADDR = "192.168.1.255";
const char *SERV_ADDR = "192.168.1.122";
// const char *SERV_ADDR = "10.55.129.74";
const int   UDP_PORT = 3000;
const int   TCP_PORT = 51000;
const char *LOOP_BACK = "127.0.0.1";
const int MAX_REQ = 5;

int main (void) {

//ДОПОЛНИТЕЛЬНО:
//МОЖНО СПЕЦИФИРОВАТЬ ЗАПРОСЫ ПО ТИПУ HTTP
//ЗАВЕТСТИ СТАНДАРТНЫЕ СТРУКТУРЫ, ЧТОБЫ ВЫПОЛНЯЛОСЬ ПО УМОЛЧАНИЮ
//И ДОПОЛНИТЕЛЬНО ВЕСТИ ПОЛЛИНГ ПО ПРИХОДУ НОВОЙ ИНФЫ


    int tsockfd, usockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int broadcast = 1;


/////////////////////////////////////////////////TCP INIT
    tsockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (tsockfd < 0) {
        perror ("ERROR IN SOCK INIT");
        exit (0);
    }
    fcntl (tsockfd, F_SETFL, O_NONBLOCK);

/////////////////////////////////////////////////SERV INFO
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (TCP_PORT);
    if (inet_aton (SERV_ADDR, &servaddr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (tsockfd);
        exit (0);
    }

/////////////////////////////////////////////////
    if (bind (tsockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        perror ("BIND ERROR");
        close (tsockfd);
        exit (0);
    }

    if (listen (tsockfd, MAX_REQ) < 0) {
        perror ("LISTERN ERROR");
        close (tsockfd);
        exit (0);
    }

/////////////////////////////////////////////////


    if ((usockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror ("socket");
        exit (1);
    }

    if (setsockopt (usockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof (broadcast)) == -1) {
        perror ("setsockopt (SO_BROADCAST)");
        exit (1);
    }

    memset (&their_addr, 0, sizeof (their_addr));
    their_addr.sin_family = AF_INET;
    their_addr.sin_port   = htons (UDP_PORT);
    if (inet_aton (SERV_ADDR, &their_addr.sin_addr) < 0) {
    // if (inet_aton (LOCAL_BROADCAST_ADDR, &their_addr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (usockfd);
        exit (1);
    }

    const char *msg = "CONNECT VIA TCP";
    if ((numbytes = sendto (usockfd, msg, strlen (msg), 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        exit(1);
    }
    printf("sent %d bytes to %s\n", numbytes,
        inet_ntoa(their_addr.sin_addr));

    close(usockfd);


/////////////////////////////////////////////////
    struct client {
        struct pollfd poll {};
        int core_num;
    };
    std::vector <struct client> clients {};


    time_t start, end;
    time (&start);

    puts ("WAITING CONNECTIONS");

    char buf [100] {};
    double TOTAL_CORES = 0;
    while (1) {

        time (&end);
        if (difftime (end, start) > 2) {
            break;
        }

        clilen = sizeof (cliaddr);
        
        if ((newsockfd = accept (tsockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            
            if (errno == EAGAIN) {
                continue;
            }
            else {
                perror ("ACCEPT ERROR");
                close (tsockfd);
                close (newsockfd);
                exit (0);
            }
        }

        fcntl (newsockfd, F_SETFL, O_NONBLOCK);

        printf ("CONNECTED: %s\n", inet_ntoa (cliaddr.sin_addr));

        int read_size = 0;

        int num = 0;
        while (1) {
            read_size = read (newsockfd, buf, sizeof (buf));
            
            if (read_size > 0) {
                break;
            }
            else if (read_size < 0 && errno != EAGAIN) {
                perror ("READ ERROR CORE NUM");
                exit (1);
            }
        }

        puts (buf);
        
        printf ("%d\n", read_size);
        num = atoi (buf);
        printf ("MAX PROC: %d\n", num);
    

        struct client tmp {};
        tmp.poll.fd = newsockfd;
        tmp.poll.events = POLLIN;
        tmp.core_num = num;

        clients.push_back (tmp);
        
        TOTAL_CORES += num;
    }

    puts ("CLIENTS CORES:\n");
    for (int i = 0; i < clients.size (); ++i) {
        printf ("%d: %d\n", i, clients[i].core_num);
    }


    //ДОДЕЛАТЬ РАЗДЕЛЕНИЕ ПО ТАСКАМ!
///
// 
//
    const int X_MIN = 0;
    const int X_MAX = 10;
    const int Y_MIN = 0;
    const int Y_MAX = 100;

    int x_tmp = X_MIN;
    int y_tmp = Y_MIN;
    int x_diff = X_MAX - X_MIN;
    int y_diff = Y_MAX - Y_MIN;

    for (int i = 0; i < clients.size (); ++i) {
/////////////////////////////////////////////////WRITE QUESTION

        Query task {};
        task.x_min = x_tmp;
        task.x_max = x_tmp + (double)clients[i].core_num / (double)TOTAL_CORES * x_diff;
        x_tmp = task.x_max;

        task.y_min = y_tmp;
        task.y_max = y_tmp + (double)clients[i].core_num / (double)TOTAL_CORES * y_diff;
        y_tmp = task.y_max;
        // sprintf (buf, "%d", clients[i].core_num);
        // int buf_size = strlen (buf);

        if (write (clients[i].poll.fd, (void *)&task, sizeof (task)) != sizeof (task)) {
            perror ("SEND ERROR");
            close (tsockfd);
            close (newsockfd);
            exit (0);
        }
    }




/////////////////////////////////////////////////GET ANSWERS
    struct pollfd fds [100];
    for (int i = 0; i < clients.size (); ++i)
        fds[i] = clients[i].poll;

    int ret = poll (fds, clients.size (), 10000);
    if (ret == -1) {
        perror ("POLL ERROR");
        //need close sockets....
        exit (1);
    }
    else if (ret == 0) {
        perror ("NO EVENTS OCCURRED");
        exit (1);
    }
    else {
        for (int i = 0; i < clients.size (); ++i) {
            if (fds[i].revents & POLLIN) {

                int num = 0;
                int read_size = 0;

                if (read_size = read (fds[i].fd, buf, sizeof (buf)), read_size > 0) {
                    // puts (buf);
                    // printf ("%d\n", read_size);
                    num = atoi (buf);
                    printf ("NUMBER: %d\n", num);
                }

            }
            else {
                perror ("NO EVENT POLLIN");
                exit (1);
            }
        }

    }

    close (tsockfd);
    close (newsockfd);

    puts ("END");

}