#include <stdio.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#define BF_SIZE 4096

int fd, watch_d, T, time = 0;
pid_t my_pid;
char *patch_array[4096] = {};
char my_dir[PATH_MAX] = {};
char process_dir[PATH_MAX] = {};

FILE *fifo;

void read_directory(char *nm);
void sig_handler_sigint(int sig);
void sig_handler_sigquit(int sig);
void get_process_dir(pid_t pid, char* process_dir);
pid_t get_pid(char *pid_arg);
void make_sample();
void create_dir_with_reg_files();
void copy_file(char *name_dir, char *real_file);
void get_file_name(char *real_file, char *name);
int is_text_file(char *name);
void get_full_file_name(char *dir_name, char *name, char *full_file_name);
void create_dir(const char *name);
void make_patch(char *process_dir, char *my_dir, char *file_name);
void print_diffs(int k);
void change_period(int t);
static void skeleton_daemon();


int main(int argc, char **argv)
{
	pid_t pid;
	char *pid_arg = argv[2];
	char *way = argv[1];

	if (!strcmp(way, "-d"))
	{
		skeleton_daemon();
	}
	
	T = 2;
	pid = get_pid(pid_arg);
	my_pid = getpid();

	signal(SIGINT, sig_handler_sigint);
	signal(SIGQUIT, sig_handler_sigquit);
	
   

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
	//--------------------------
	get_process_dir(pid, process_dir);
	get_process_dir(my_pid, my_dir);
	create_dir("reg_files");
	create_dir("diffs");
	system("touch last_diffs.txt");
	read_directory(process_dir);
	//--------------------------
	char file_name[256] = {};

	fd = inotify_init1(0);
	if (fd == -1)
	{
		perror("inotify_init");
		exit(1);
	}
	
	fcntl(fd, F_SETFL, O_NONBLOCK);
	watch_d = inotify_add_watch(fd, process_dir, IN_MODIFY);

	if (watch_d == -1)
	{
		perror("inotify_add_watch");
		exit(1);
	}
	//---------------------------


	

	int DO_WORK = 1;
	int answer;

	while(DO_WORK)
	{
		
		printf("Что вы хотите сделать?\n1 - вывести k диффов\n2 - изменить период семплирования\n3 - изменить PID просматриваемого процесса\n");
		sleep(10);
		fscanf(fifo, "%d", &answer);


		if (answer < 1 || answer > 3)
		{
			printf("Ответ не распознан. exit.\n");
			exit(0);
		}

		switch (answer)
		{
			case 1:
				int k;
				
				fscanf(fifo, "%d", &k);
				print_diffs(k);
				break;
			case 2:
				int t;
				
				fscanf(fifo, "%d", &t);
				change_period(t);
				break;
			case 3:
				DO_WORK = 0;
		}
		make_sample();
		sleep(T);
	}
	
	return 0;
}


void read_directory(char *name)
{
	int text_file;
	struct dirent *dir_entry;
	char command[BF_SIZE] = {};
	
	char file_info[BF_SIZE] = {};
	char real[256] = {};
	char *file_type;

	DIR *dir = opendir(name);

	if (dir == NULL)
	{
		perror("opendir");
		exit(1);
	}

	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (dir_entry->d_type == DT_REG)
		{
			sprintf(real, "%s/%s", name, dir_entry->d_name);
			
			if (is_text_file(real))
			{
				copy_file(name, dir_entry->d_name);
			}	
		}
	}

	closedir(dir);
}


void sig_handler_sigint(int sig)
{

}


void sig_handler_sigquit(int sig)
{
	fclose(fifo);
	system("rm file.fifo");
	
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

	exit(0);
}


void get_process_dir(pid_t pid, char* process_dir)
{
    char proc_dir[PATH_MAX] = {};

    sprintf(proc_dir, "/proc/%d/cwd", pid);

   	readlink(proc_dir, process_dir, PATH_MAX);
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


void make_sample()
{		
	char* p = NULL;
	char buffer[BF_SIZE];
	char file_name[PATH_MAX];
	int size;
	size = read(fd, buffer, PATH_MAX);

    for (p = buffer; p < buffer + size; ) 
    {
        struct inotify_event* event = (struct inotify_event *) p;

        sprintf(file_name, "%s/%s", process_dir, event->name);

        if (is_text_file(file_name))
        {
        	patch_array[time] = (char*)malloc(strlen(event->name));
        	strcpy(patch_array[time], event->name);
           	make_patch(process_dir, my_dir, event->name);  
        }				

        p += sizeof(struct inotify_event) + event->len;
    }
}


void create_dir(const char *name)
{
	char command[256] = {0};
	sprintf(command, "mkdir %s", name);
	system(command);
}


void copy_file(char *name, char *real_file)
{
	char command[BF_SIZE] = {};
	char real[256] = {};
	char cpy_file[256] = {};

	sprintf(cpy_file, "reg_files/%s", real_file);
	sprintf(real, "%s/%s", name, real_file);

	sprintf(command, "cp %s %s", real, cpy_file);
	system(command);
}


void get_full_file_name(char *dir_name, char *name, char *full_file_name)
{
	sprintf(full_file_name, "%s/%s", dir_name, name);
}


void get_file_name(char *real_file, char *name)
{
	char *curr_name, *prev_name;
	char cp_real_file[PATH_MAX] = {};
 
	strcpy(cp_real_file, real_file);

	prev_name = strtok(cp_real_file,"/");
	while((curr_name = strtok(NULL,"/")) != NULL)
	{
		strcpy(prev_name, curr_name);
	}
    
	sprintf(name, "reg_files/%s", prev_name);
}


int is_text_file(char *name)
{
	char command[BF_SIZE] = {};

	sprintf(command, "file %s | grep "":*text"" > busket.txt", name);

	if (!system(command))
	{
		return 1;
	}

	return 0;
}


void make_patch(char *process_dir, char *my_dir, char *file_name)
{
	int old_patch, new_patch;
	char command[256] = {};
	char new_files[256] = {};
	char old_files[256] = {};
	char full_file_name[256] = {};
	char buffer[BF_SIZE] = {};

	get_full_file_name(process_dir, file_name, full_file_name);
	get_full_file_name(my_dir, "reg_files", old_files);

	//printf("ЗАФИКСИРОВАНО ИЗМЕНЕНИЕ %d С ФАЙЛОМ %s\n", time, file_name);

	sprintf(command, "diff %s/%s %s > diffs/%d.patch", old_files, file_name, full_file_name, time);
	system(command);

	sprintf(command, "patch reg_files/%s diffs/%d.patch", file_name, time);
	system(command);

	time++;
}


void print_diffs(int k)
{
	int diffd, start_time = time, last_time = time - k;
	char namedif[256] = {};
	char buffer[BF_SIZE] = {};
	FILE *last_dif;

	last_dif = fopen("last_diffs.txt" , "a");

	for (int i = start_time - 1; i >= last_time; i--)
	{
		fprintf(last_dif, "%d. Изменение с файлом %s\n",start_time - i , patch_array[i]);
		
		sprintf(namedif, "diffs/%d.patch", i);

		diffd = open(namedif , O_RDONLY);
		read(diffd, buffer, BF_SIZE);
		close(diffd);

		fprintf(last_dif, "%s\n\n", buffer);
		
		//printf("Сейчас применим патч %d к файлу %s\n", i, patch_array[i]);
		//sprintf(command, "patch %s/%s diffs/%d.patch", process_dir, patch_array[i], i);
	}
	fprintf(last_dif, "\n--------------------------------------------------------\n");

	fclose(last_dif);

	printf("Последние %d диффов записаны в файл last_dif.txt\n", k);
}


void change_period(int t)
{
	T = t;
}


static void skeleton_daemon()
{
    pid_t pid;
    
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
   
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    
    chdir("/");
    
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
 
}


