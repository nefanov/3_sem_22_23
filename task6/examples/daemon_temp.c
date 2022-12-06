#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

void do_work(void *args) {
	int fd = open("/home/nefanov/log2.txt", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
	while (1) {
	//	int fd = open("/home/nefanov/log2.txt", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
		write(fd, "hello\n", 7);
		//printf("Daemon func stub\n");
		sleep(1);
	}
	close(fd);
	return 0;
}

int main(int argc, char **argv) {
	pid_t pid; 

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	} else if (pid > 0) {
		signal(SIGCHLD, SIG_IGN);
	} else {
		int sid = setsid();
		if (sid == -1) {
			perror("setsid");
			exit(1);
		}
		printf("%d\n" ,getpid());
		close(0);
		close(1);
		close(2);
		umask(0);
		//printf("%d\n" ,getpid());
		chdir("/");
		do_work(NULL);
	}

	return 0;
}
