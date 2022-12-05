#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


void *f(void* arg) {
	char c;
	while(1)
		scanf("%c", &c);
	return NULL;
}

#define TIMEOUT 5*1000

int main() {
	struct pollfd fds[2];

	fds[0].fd = 0;
	fds[0].events = POLLIN;

	fds[1].fd = 1;
	fds[1].events = POLLOUT;
	pthread_t tid;
	//pthread_create(&tid, NULL ,f , NULL);
	//sleep(1);
	int ret = poll(fds, 2, TIMEOUT);

	if (!ret) {
		printf("TIMEOUT!\n");
	} else if (ret > 0) {
		if (fds[0].revents & POLLIN) {
			char str[4096];
			printf("POLLIN on stdin\n");
			scanf("%s", str);
			printf("%s\n", str);
		} 
	       	if (fds[1].revents & POLLOUT) {
			printf("POLLOUT on stdout\n");
		}
	} else {
		perror("poll");
	}

	return 0;
}

