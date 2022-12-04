#ifndef USER_SERVICE
#define USER_SERVICE

#include <errno.h>
#include <string.h>
#include "Demon.h"

int print_diffs(int k);
void change_period(int t);
int get_ans();
void inform(const char* str);
void make_fifo_and_log();
pid_t get_pid(char *pid_arg);




void make_fifo_and_log()
{
	char name_log_file[256] = {};
	sprintf(name_log_file, "%s/log.txt", my_dir);

	log_file = open(name_log_file, O_WRONLY | O_CREAT, 0666);

	if (mknod("/tmp/file.fifo", S_IFIFO | 0666, 0)  < 0)
	{
		perror("mknod");
		exit(-1);
	}

	fifo = fopen("/tmp/file.fifo", "r+");

	if (!fifo)
	{
		perror("open");
		exit(-1);
	}
}


int print_diffs(int k)
{
	if (k > time)
	{
		return 1;
	}

	int diffd, start_time = time, last_time = time - k;
	char namedif[256] = {};
	char name_file[256] = {};
	char buffer[BF_SIZE] = {};
	FILE *last_dif;

	sprintf(name_file, "%s/last_diffs.txt", my_dir);
	last_dif = fopen(name_file, "a");

	for (int i = start_time - 1; i >= last_time; i--)
	{
		fprintf(last_dif, "%d. Изменение с файлом %s\n",start_time - i , patch_array[i]);
		
		sprintf(namedif, "%s/diffs/%d.patch", my_dir, i);

		diffd = open(namedif , O_RDONLY);
		read(diffd, buffer, BF_SIZE);
		close(diffd);

		fprintf(last_dif, "%s\n\n", buffer);
		
		//sprintf(command, "patch %s/%s diffs/%d.patch", process_dir, patch_array[i], i);применение патча
	}
	fprintf(last_dif, "\n--------------------------------------------------------\n");

	fclose(last_dif);

	sprintf(name_file, "Последние %d диффов записаны в файл last_dif.txt в директории из-которой был запущен процесс\n", k);
	inform(name_file);

	return 0;
}


void change_period(int t)
{
	T = t;
	union sigval value;
	value.sival_int = t;

	if (sigqueue(CHILD_PID, SIGUSR1, value) == -1)
	{
		perror("sigqueue");
		_exit(1);
	}
}


int get_ans()
{
	int answer;

	if (is_demon)
	{
		fscanf(fifo, "%d", &answer);
	}
	else
	{
		scanf("%d", &answer);
	}

	return answer;
}


void inform(const char* str)
{
	if (is_demon)
	{
		write(log_file, str, strlen(str));
	}
	else
	{
		printf("%s", str);
	}
}


pid_t get_pid(char *pid_arg)
{
	char *num_of_pid = pid_arg;
	int coef, len = 0;
	pid_t pid = 0;

	while (*num_of_pid <= '9' && *num_of_pid >= '0')
	{
		num_of_pid++;
		len++;
	}
	
	for (int num = 0; num < len; num++)
	{
		coef = 1;
		for (int i = 0; i < (len - num - 1); i++)
		{
			coef *= 10;
		}
		
		pid += coef * (pid_arg[num] - '0');
	}

	return pid;
}

#endif
