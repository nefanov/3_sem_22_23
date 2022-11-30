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
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return 1;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if (bind (sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("bind");
		return 1;
	}

	int len, input_len;
	len = sizeof(cliaddr);
	input_len = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
	buffer[input_len]='\0';
	printf("Data from client: %s\n", buffer);
	printf("IP addr of client is: %s\n", inet_ntoa(cliaddr.sin_addr));
	char *back = "Send back\n";
	sendto(sockfd, back, strlen(back), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
	printf("Back message is sent\n");
	return 0;
}
