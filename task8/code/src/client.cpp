#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


const char *GLOBAL_BROADCAST_ADDR = "255.255.255.255";
const char *LOCAL_BROADCAST_ADDR = "192.168.1.255";
// const char *SERV_ADDR = "192.168.1.122";
const char *SERV_ADDR = "10.55.129.74";
const int   SERV_PORT = 4950;
const char *LOOP_BACK = "127.0.0.1";

int main (void) {

    int sockfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int broadcast = 1;


    if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror ("socket");
        exit (1);
    }

    if (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof (broadcast)) == -1) {
        perror ("setsockopt (SO_BROADCAST)");
        exit (1);
    }

    memset (&their_addr, 0, sizeof (their_addr));
    their_addr.sin_family = AF_INET;
    their_addr.sin_port   = htons (SERV_PORT);
    // their_addr.sin_addr = INADDR_BROADCAST;
    if (inet_aton (LOOP_BACK, &their_addr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (sockfd);
        exit (1);
    }

    if ((numbytes = sendto (sockfd, "KEK", strlen("KEK"), 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        exit(1);
    }

/////////////////////////////////////////////////
    int size_read = 0;
    char buf[100] {};
    if ((size_read = recvfrom (sockfd, buf, 1000 - 1 , 0,
        (struct sockaddr *) &cliaddr, &clilen)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    
    printf("sent %d bytes to %s\n", numbytes,
        inet_ntoa(their_addr.sin_addr));


    printf ("ANSWER: %s \t %d\n", inet_ntoa (cliaddr.sin_addr), ntohs (cliaddr.sin_port));

    close(sockfd);

    return 0;













/////////////////////////////////////////////////SOCKET INIT
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror ("ERROR IN SOCK INIT");
        exit (0);
    }

/////////////////////////////////////////////////SERV INFO
    struct sockaddr_in servaddr {};
    memset (&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (SERV_PORT);
    if (inet_aton (SERV_ADDR, &servaddr.sin_addr) < 0) {
        perror ("ERROR CONVERT IP ADDR SERVER");
        close (sockfd);
        exit (0);
    }

/////////////////////////////////////////////////

    if (connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        perror ("CONNECT ERROR");
        exit (0);
    }

    sprintf (buf, "%d", 512);
    int buf_size = strlen (buf);
    // puts (buf);
    for (int i = 0; i < buf_size; ++i) {
        printf ("%c\n", buf[i]);
    }
    puts ("END");

    if (write (sockfd, buf, buf_size) != buf_size) {
        perror ("SEND ERROR");
        close (sockfd);
        exit (0);
    }

/////////////////////////////////////////////////READING

    int num = 0;
    int read_size = 0;
    while (read_size = read (sockfd, buf, sizeof (buf)) , read_size > 0) {
        puts (buf);
        
        printf ("%d\n", read_size);
        num = atoi (buf);
        printf ("NUMBER: %d\n", num);
    }

    close (sockfd);
}