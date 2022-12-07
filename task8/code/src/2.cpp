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

const char *SERV_ADDR = "127.0.0.1";
const int MAX_REQ = 5;


int main (void) {

    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;

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