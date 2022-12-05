#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8123

#define MAXLINE 4096

int main() {
        int sockfd;
        char buffer[MAXLINE];
        char str_addr[256];
        struct sockaddr_in servaddr, cliaddr;
	char *hello_msg = "Hello from client\n";
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
		perror("sockopt");
	}

	sendto(sockfd, (const char*)hello_msg, strlen(hello_msg), MSG_CONFIRM, (const struct sockaddr*)&servaddr,sizeof(servaddr));

	int len;
	int size_saddr = sizeof(servaddr);
	len = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&servaddr, &size_saddr);
	if (len > 0) {
		buffer[len]='\0';
		printf("%s\n", buffer);
	}
	close(sockfd);
	return 0;
}
