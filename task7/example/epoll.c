#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int epoll_fd;
	struct epoll_event event;
	struct epoll_event events[5];
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		perror("epoll open");
	event.events = EPOLLIN;
	event.data.fd = 0;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)) {
		perror("epoll ctl");
	}

	int i = 0;
	int event_count = epoll_wait(epoll_fd, events, 5, 5000);
	if (event_count == -1) {
		perror("epoll wait");
	} else {
		printf("Events count %d\n" , event_count);

		for (; i < event_count; i++ ) {
			char buf[4096];
			printf("Handling FD %u\n", events[i].data.fd);
			int len_rd = read(events[i].data.fd, buf, sizeof(buf));
			printf("%s\n", buf);
		}
	}
	return 0;
}
