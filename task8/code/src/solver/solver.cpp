#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <error.h>
#include <stdlib.h>
#include <fcntl.h>
#include "header.hpp"


const char *GLOBAL_BROADCAST_ADDR = "255.255.255.255";
// const char *LOCAL_BROADCAST_ADDR  = "192.168.1.255";
const char *SERV_ADDR = "172.20.10.10";
// const char *SERV_ADDR = "10.55.129.74";
const int   UDP_PORT = 3000;
// const char *LOOP_BACK = "127.0.0.1";
const int TCP_PORT = 51000;

int main (void) {

    int sockfd;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;

/////////////////////////////////////////////////SOCKET INIT
    sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror ("ERROR IN SOCK INIT");
        exit (0);
    }

/////////////////////////////////////////////////SERV INFO
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (UDP_PORT);
    if (inet_aton (GLOBAL_BROADCAST_ADDR, &servaddr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (sockfd);
        exit (0);
    }

    int optval = 1;
    if (setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)) != 0) {
        perror ("listener: setsockopt");
        exit (1);
    }   

/////////////////////////////////////////////////
    if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        perror ("BIND ERROR");
        close (sockfd);
        exit (0);
    }


    printf("listener: waiting to recvfrom...\n");

    char buf[1000] {};
    clilen = sizeof (cliaddr);
    int size_read = 0;
    if ((size_read = recvfrom (sockfd, buf, 1000 - 1 , 0,
        (struct sockaddr *) &cliaddr, &clilen)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf ("CONNECTED: %s \t %d\n", inet_ntoa (cliaddr.sin_addr), ntohs (cliaddr.sin_port));

    printf ("listener: packet is %d bytes long\n", size_read);
    // buf[size_read] = '\0';
    printf ("listener: packet contains \"%s\"\n", buf);
    strcpy (buf, "");

    close(sockfd);



/////////////////////////////////////////////////SOCKET INIT
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("ERROR IN SOCK INIT");
        exit (0);
    }

/////////////////////////////////////////////////SERV INFO
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons (TCP_PORT);
    // if (inet_aton (cliaddr.sin_addr, &servaddr.sin_addr) < 0) {
    //     perror ("ERROR CONVERT IP ADDR SERVER");
    //     close (sockfd);
    //     exit (0);
    // }
    servaddr.sin_addr = cliaddr.sin_addr;
/////////////////////////////////////////////////

    while (connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) 
        perror ("CONNECT ERROR");

    int proccess_max = sysconf (_SC_NPROCESSORS_ONLN);
    if (proccess_max < 0) {
        perror ("SYSTEM GET ERROR");
        close (sockfd);
        exit (1);
    }
    sprintf (buf, "%d", proccess_max);
    int buf_size = strlen (buf);
    // printf ("PROCESS MAX: %s\n", buf);
    
    if (write (sockfd, buf, buf_size) != buf_size) {
        perror ("SEND ERROR");
        close (sockfd);
        exit (0);
    }

///////////////////////////////////////////////READING

    int num = 0;
    int read_size = 0;

    Query task {};
    
    read_size = read (sockfd, (void*)&task, sizeof (task));
    // printf ("%d\n", read_size);
    // num = atoi (buf);
    // printf ("NUMBER: %d\n", num);

/////////////////////////////////////////////////SOLVING
    int ans = count_monte (proccess_max, task.x_min, task.x_max, task.x_min, task.y_max);
    
    sprintf (buf, "%d", ans);
    buf_size = strlen (buf);

    if (write (sockfd, buf, buf_size) != buf_size) {
        perror ("SEND ERROR");
        close (sockfd);
        exit (1);
    }


    puts ("END");

    close (sockfd);
}