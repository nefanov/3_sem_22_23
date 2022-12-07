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


const char *GLOBAL_BROADCAST_ADDR = "255.255.255.255";
const char *LOCAL_BROADCAST_ADDR  = "192.168.1.255";
// const char *SERV_ADDR = "192.168.1.122";
const char *SERV_ADDR = "10.55.129.74";
const int   SERV_PORT = 4950;
const char *LOOP_BACK = "127.0.0.1";
const int MAX_REQ = 5;


int main (void) {
    int sockfd, newsockfd;
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
    servaddr.sin_port = htons (SERV_PORT);
    if (inet_aton (LOOP_BACK, &servaddr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (sockfd);
        exit (0);
    }

    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &optval, sizeof (optval)) != 0) {
        perror("listener: setsockopt");
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


/////////////////////////////////////////////////SEND BACK
    int numbytes = 0;
    if ((numbytes = sendto (sockfd, "ANS", strlen ("ANS"), 0,
             (struct sockaddr *)&servaddr, sizeof (servaddr))) == -1) {
        perror("sendto");
        exit(1);
    }

    close(sockfd);

    return 0;








/////////////////////////////////////////////////SOCKET INIT
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("ERROR IN SOCK INIT");
        exit (0);
    }

/////////////////////////////////////////////////SERV INFO
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (51000);
    if (inet_aton (SERV_ADDR, &servaddr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (sockfd);
        exit (0);
    }

/////////////////////////////////////////////////
    if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        perror ("BIND ERROR");
        close (sockfd);
        exit (0);
    }

    if (listen (sockfd, MAX_REQ) < 0) {
        perror ("LISTERN ERROR");
        close (sockfd);
        exit (0);
    }

    while (1) {

        clilen = sizeof (cliaddr);
        if ((newsockfd = accept (sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0) {
            perror ("ACCEPT ERROR");
            close (sockfd);
            close (newsockfd);
            exit (0);
        }
        fcntl(newsockfd, F_SETFL, O_NONBLOCK);


        printf ("CONNECTED: %s\n", inet_ntoa (cliaddr.sin_addr));

        char buf [100] {};
        int read_size = 0;

        int num = 0;
        while (read_size = read (newsockfd, buf, sizeof (buf)) , read_size > 0) {
            puts (buf);
            
            printf ("%d\n", read_size);
            num = atoi (buf);
            printf ("NUMBER: %d\n", num);
        }


/////////////////////////////////////////////////WRITE BACK
        sprintf (buf, "%d", num + 1);
        int buf_size = strlen (buf);
        // puts (buf);
        for (int i = 0; i < buf_size; ++i) {
            printf ("%c\n", buf[i]);
        }
        puts ("END");

        if (write (newsockfd, buf, buf_size) != buf_size) {
            perror ("SEND ERROR");
            close (sockfd);
            close (newsockfd);
            exit (0);
        }

        
    }


    close (sockfd);
}