#include "daemon.h"

int pollSamples(int inotify_fd, const char* work_dir)
{
    char buf[NOTIFY_SIZE] = {};
    errno = 0;

    int numRead = read(inotify_fd, buf, NOTIFY_SIZE);
    
    if (numRead == 0) 
    {
        LOG("read() from inotify fd returned 0!");
    }
    else if (numRead == ERROR && errno == EAGAIN)
    {
        LOG("No events happened");
        return 0;
    }
    else if (numRead == ERROR)
    {
        LOG("inotify read error");
        return ERROR;
    }

    char* p = NULL;
    short f_textChanged = 0;
    for (p = buf; p < buf + numRead; ) 
    {
        struct inotify_event* event = (struct inotify_event *) p;
        logVar("There is change with %s\n", event->name);

        char file[PATH_MAX] = {};
        sprintf(file, "%s/%s", work_dir, event->name);

        if (is_text_file(file))
        {   
            logVar("%s is text", event->name);
            create_diff(file, event->name);
            f_textChanged = 1;
        }

        p += sizeof(struct inotify_event) + event->len;
    }

    if (f_textChanged)
        {
            create_sample();
        }
    
    return f_textChanged;
}



void create_diff (const char* observed_file_path, const char* file_name)
{
    char backup_file[PATH_MAX] = {};
    sprintf(backup_file, "%s/%s", DAEMON_DIR, file_name);

    char cmd[CMD_SIZE] = {};
    sprintf(cmd, "diff -Nbur %s %s > %s/%s.diff", 
            backup_file, observed_file_path, DIFF_DIR, file_name);
    
    system(cmd);
}

int is_text_file(const char* file)
{
    char cmd[CMD_SIZE] = {};
    sprintf(cmd, "file %s | grep \":*text\" > /dev/null", file);
    
    int sys_ret = 0;
    sys_ret =  system(cmd);

    return sys_ret == 0 ? 1 : 0; 
}

void create_sample()
{
    char timestamp[STAMP_SIZE] = {};
    getTimestamp(timestamp);

    char cmd[CMD_SIZE] = {};
    
    sprintf(cmd, "cat %s/*.diff > %s/%s.sample", DIFF_DIR, SAMPLE_DATA_DIR, timestamp);
    logVar("SAMPLE execut: %s", cmd);
    system(cmd);

    sprintf(cmd, "rm %s/*.diff", DIFF_DIR);
    system(cmd);    
}

void getTimestamp(char* timestamp)
{
  static int timetmp = 0;
  
  sprintf (timestamp,"%d", timetmp);
  timetmp++;

  for (; *timestamp; timestamp++)
    if (*timestamp == ' ' || *timestamp == '\n') *timestamp = '_';

  return 0;
}