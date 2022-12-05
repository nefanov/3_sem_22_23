#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8125

#define MAXLINE 4096

void* handler(void *args);

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
	unsigned conn_cnt=0;
	while (conn_cnt<10000) {
		connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
		if (connfd < 0) {
			perror("accept");
		}
		conn_cnt++;
		printf("Server accepted connection...\n");
		pthread_t tid;
		pthread_create(&tid, NULL, handler, (void*)&connfd);
	}
	close(sockfd);
	return 0;
}

void* handler(void* argv) {
	int connection = *((int*)argv);
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
		printf("Back from %u\n", connection);
		scanf("%s", buf);
		if (!strcmp(buf, "exit")) {
			printf("exit\n");
			break;
		} else {
			write(connection, buf, strlen(buf));
		}
	}
	close(connection);
	return NULL;
}
