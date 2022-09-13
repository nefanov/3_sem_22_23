#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
	pid_t pids[10];
	int i;

	for (i = 9; i >= 0; --i) {
		pids[i] = fork();
		if (pids[i] == 0) {
			sleep(i+1);
			printf(" %d:termination\n", getpid());
			exit(0);
		}
	}

	for (i = 9; i >= 0; --i)
		waitpid(pids[i], NULL, 0);

	return 0;
}
