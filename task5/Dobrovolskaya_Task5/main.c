#include "KV_storage.h"



int main(int argc, char **argv)
{
	LOG = fopen("log.txt", "w");
	fclose(LOG);

	PARENT_PID = getpid();

	int shmid = get_shmid();

	if ((STORAGE = (char*)shmat(shmid, NULL, 0)) == (char*)(-1))
	{
		perror("shmat");
		_exit(-1);
	}

	CHILD_PID = fork();

	if (CHILD_PID == -1)
	{
		perror("fork");
		_exit(1);
	}
	else if (CHILD_PID > 0)// в родителе записываем информацию в STORAGE
	{
		close(1);
		struct sigaction act;
		act.sa_sigaction = parent_handler;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR2, &act, NULL);
		signal(SIGINT, sigint_handler);
		parent_handler(0, NULL, NULL);

		while(1)
		{
			;
		}
	}
	else// в ребенке считываем информацию из STORAGE
	{
		struct sigaction act;
		act.sa_sigaction = child_handler;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &act, NULL);

		while(1)
		{
			;
		}
	}

	return 0;
	
}

