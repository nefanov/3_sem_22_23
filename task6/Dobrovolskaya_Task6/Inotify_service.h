#ifndef INOTIFY_SERVICE
#define INOTIFY_SERVICE

#include <sys/inotify.h>
#include "Directory_service.h"

void init_inotify();
void make_sample(int sign);
void make_patch(char *process_dir, char *my_dir, char *file_name);




void init_inotify()
{
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
}


void make_sample(int sign)
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

        if (is_text_file(file_name) || is_demon)
        {
        	patch_array[time] = (char*)malloc(strlen(event->name));
        	strcpy(patch_array[time], event->name);
           	make_patch(process_dir, my_dir, event->name);  
        }				

        p += sizeof(struct inotify_event) + event->len;
    }
}


void make_patch(char *process_dir, char *my_dir, char *file_name)
{
	int old_patch, new_patch;
	char command[256] = {};
	char new_files[256] = {};
	char old_files[256] = {};
	char full_file_name[256] = {};
	char buffer[BF_SIZE] = {};

	sprintf(full_file_name, "Делаем патч номер %d с файлом %s\n", time, file_name);
	inform(full_file_name);

	sprintf(full_file_name, "%s/%s", process_dir, file_name);
	sprintf(old_files, "%s/reg_files", my_dir);

	sprintf(command, "diff %s/%s %s > %s/diffs/%d.patch", old_files, file_name, full_file_name, my_dir, time);
	system(command);

	sprintf(command, "patch %s/%s %s/diffs/%d.patch > %s/busket.txt", old_files, file_name, my_dir, time, my_dir);
	system(command);

	time++;
}

#endif
