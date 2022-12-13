#include "Inotify_service.h"
#include "Demon.h"
#include "Directory_service.h"
#include "User_service.h"



void child_handler(int signo, siginfo_t *si, void *ucontext)
{
	T = (si->si_value).sival_int;
}



int main(int argc, char **argv)
{
	pid_t pid;
	char *pid_arg = argv[2];
	char *way = argv[1];
	T = 2;
	pid = get_pid(pid_arg);//make int from char*
	my_pid = getpid();

	get_process_dir(pid, process_dir);
	get_process_dir(my_pid, my_dir);
	create_dir("reg_files");
	create_dir("diffs");
	system("touch last_diffs.txt");

	read_directory(process_dir);

	if (!strcmp(way, "-d"))
	{
		is_demon = 1;
		skeleton_daemon();
		make_fifo_and_log();
	}

	my_pid = getpid();
	char str[256] = {};
	sprintf(str, "Мой пид: %d\n", my_pid);
	inform(str);

	init_inotify();

	int DO_WORK = 1;
	int answer;

	CHILD_PID = fork();
	if (CHILD_PID == 0)
	{
		struct sigaction act;
		act.sa_sigaction = child_handler;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &act, NULL);

		while (1)
		{
			sleep(T);
			kill(my_pid, SIGUSR1);
		}
	}
	else
	{
		signal(SIGUSR1, make_sample);

		while(DO_WORK)
		{
			inform("Что вы хотите сделать?\n1 - вывести k диффов\n2 - изменить период семплирования\n3 - выход\n");

			answer = get_ans();

			if (answer < 1 || answer > 3)
			{
				inform("Ответ не распознан. exit.\n");
				exit(0);
			}

			switch (answer)
			{
				case 1:
					int k;
					inform("Введите число k: \n");
					k = get_ans();
					if (print_diffs(k))
					{
						inform("Еще не набралось столько диффов.\n");
					}
					break;
				case 2:
					int t;
					inform("Введите новый период: \n");
					t = get_ans();
					change_period(t);
					break;
				case 3:
					inform("До свидания!\n");
					DO_WORK = 0;
					break;
			}
		}
	}

	if (is_demon)
	{
		close(log_file);
		
		system(str);
		fclose(fifo);
		system("rm /tmp/file.fifo");
	}
	
	if (inotify_rm_watch(fd, watch_d))
	{
		perror ("inotify_rm_watch");
		exit(1);
	}
	if (close(fd))
	{
		perror("close");
		exit(1);
	}
	
	return 0;
}


