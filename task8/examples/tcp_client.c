#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8125

#define MAXLINE 4096

void handler_client(int connection);

int main() {
        int sockfd;
        char buffer[MAXLINE];
        char str_addr[256];
        struct sockaddr_in servaddr, cliaddr;
	int connfd;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                return 1;
        }
        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("connect");
	}

	handler_client(sockfd);
	/*
	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		perror("bind");
	}

	if (listen(sockfd, 3) == -1) {
		perror("listen");
	}
	printf("Listening\n");
	int len = sizeof(cliaddr);

	connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
	if (connfd < 0) {
		perror("accept");
	}
	printf("Server accepted connection...\n");
	handler(connfd);
	*/
	close(sockfd);
	return 0;
}

void handler_client(int connection) {
	char buf[MAXLINE];
	memset(buf, 0, MAXLINE);
	int len;
	while(1) {
		scanf("%s", buf);
		if (!strcmp(buf, "exit")) {
                        printf("exit\n");
                        break;
                } else {
			len = write(connection, buf, strlen(buf));
                	if (len == -1) {
                        	perror("write");
                        	break;
                	}
                }

		len = read(connection, buf, MAXLINE);
		if (len == -1) {
			perror("read");
			break;
		}
		buf[len]='\0';
		printf("%s\n", buf);
	}
	close(connection);
	return;
}
