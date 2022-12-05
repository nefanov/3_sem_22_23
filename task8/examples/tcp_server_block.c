#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8123

#define MAXLINE 4096

void handler(int);

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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
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
	close(sockfd);
	return 0;
}

void handler(int connection) {
	char buf[MAXLINE];
	memset(buf, 0, MAXLINE);
	int len;
	while(1) {
		len = read(connection, buf, MAXLINE);
		if (len == -1) {
			perror("read");
			break;
		}
		buf[len]='\0';
		printf("%u: %s\n", connection, buf);
		scanf("%s", buf);
		if (!strcmp(buf, "exit")) {
			printf("exit\n");
			break;
		} else {
			write(connection, buf, strlen(buf));
		}
	}
	close(connection);
	return;
}
