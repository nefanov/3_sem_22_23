#ifndef DIR_SERVICE
#define DIR_SERVICE

#include <dirent.h>
#include "User_service.h"

void read_directory(char *nm);
void get_process_dir(pid_t pid, char* process_dir);
void create_dir(const char *name);
void copy_file(char *name_dir, char *real_file);
int is_text_file(char *name);


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


void create_dir(const char *name)
{
	char command[256] = {0};
	sprintf(command, "cd %s", name);

	if (system(command))
	{
		sprintf(command, "mkdir %s", name);
		system(command);
	}
	else
	{
		system("cd ..");
	}

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


void get_process_dir(pid_t pid, char* process_dir)
{
    char proc_dir[PATH_MAX] = {};

    sprintf(proc_dir, "/proc/%d/cwd", pid);

   	readlink(proc_dir, process_dir, PATH_MAX);
}


int is_text_file(char *name)
{
	char command[BF_SIZE] = {};
	char busk[256] = {};

	sprintf(busk, "%s/busket.txt", my_dir);

	sprintf(command, "file %s | grep "":*text"" > %s", name, busk);

	if (!system(command))
	{
		return 1;
	}

	return 0;
}

#endif
