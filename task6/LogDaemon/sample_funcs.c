#include <LogDaemon.h>

static void create_diff (const char* observed_file_path, const char* file_name);
static void create_sample();
static int is_text_file(const char* file);

void create_diff (const char* observed_file_path, const char* file_name)
{
    char backup_file[PATH_MAX] = {};
    sprintf(backup_file, "%s/%s", DAEMON_DIR, file_name);

    char cmd[CMD_LEN] = {};
    sprintf(cmd, "diff -Nbur %s %s > %s/%s.diff", 
            backup_file, observed_file_path, DIFF_DIR, file_name);
    
    system(cmd);
}

int is_text_file(const char* file)
{
    char cmd[CMD_LEN] = {};
    sprintf(cmd, "file %s | grep \":*text\" > /dev/null", file);
    
    int sys_ret = 0;
    sys_ret =  system(cmd);

    return sys_ret == 0 ? 1 : 0; 
}

void create_sample()
{
    char timestamp[TIMESTAMP_LEN] = {};
    get_timestamp(timestamp);

    char cmd[CMD_LEN] = {};
    
    sprintf(cmd, "cat %s/*.diff > %s/%s.sample", DIFF_DIR, SAMPLE_DIR, timestamp);
    log("SAMPLE: Gonna execut: %s", cmd);
    system(cmd);

    sprintf(cmd, "rm %s/*.diff", DIFF_DIR);
    system(cmd);    
}

int poll_sample(int inotify_fd, const char* work_dir)
{
    char buf[BUF_SIZE] = {};
    errno = 0;

    int numRead = read(inotify_fd, buf, BUF_SIZE);
    
    if (numRead == 0) 
    {
        log("read() from inotify fd returned 0!");
    }
    else if (numRead == ERROR && errno == EAGAIN)
    {
        log("No events happened");
        return 0;
    }
    else if (numRead == ERROR)
    {
        log("inotify read error");
        return ERROR;
    }

    // Process all of the events in buffer returned by read()
    char* p = NULL;
    int is_text_changed = 0;
    for (p = buf; p < buf + numRead; ) 
    {
        struct inotify_event* event = (struct inotify_event *) p;
        log("There is change with %s\n", event->name);

        char file[PATH_MAX] = {};
        sprintf(file, "%s/%s", work_dir, event->name);

        if (is_text_file(file))
        {   
            log("%s is text", event->name);
            create_diff(file, event->name);
            is_text_changed = 1;
        }

        p += sizeof(struct inotify_event) + event->len;
    }

    if (is_text_changed)
        {
            create_sample();
        }
    
    return is_text_changed;
}