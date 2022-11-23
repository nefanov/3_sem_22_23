#include <LogDaemon.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h> // readlink

#include <string.h> // atoi

#include <sys/inotify.h>
#include <limits.h>

#include <fcntl.h>

// Buffer for 10 inotify events
#define BUF_SIZE (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

// Create a dir for stroring file history
int create_closet()
{
    mode_t old_umask = umask(0);

    int ret = mkdir(DAEMON_DIR, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        LOG_ERROR("mkdir closet");
        return ERROR;
    }

    ret = mkdir(DIFF_DIR, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        LOG_ERROR("mkdir diffs");
        return ERROR;
    }

    ret = mkdir(SAMPLE_DIR, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        LOG_ERROR("mkdir samples");
        return ERROR;
    }

    umask(old_umask);
}

// Get work dir path
int get_work_dir(pid_t pid, char* work_dir)
{
    char* proc_dir_path[PATH_MAX] = {};
    sprintf(proc_dir_path, "/proc/%d/cwd", pid);

    CATCH(
          "readlink",
          readlink(proc_dir_path, work_dir, PATH_MAX)
         );
    log("Proc work dir: %s", work_dir);
}

#define CMD_LEN 255
void create_diff (const char* observed_file_path, const char* file_name)
{
    char backup_file[PATH_MAX] = {};
    sprintf(backup_file, "%s/%s", DAEMON_DIR, file_name);

    char cmd[CMD_LEN] = {};
    sprintf(cmd, "diff -Nbur %s %s > %s/%s.diff", 
            backup_file, observed_file_path, DIFF_DIR, file_name);
    
    printf("Gonn execute %s\n", cmd);
    system(cmd);
}

int is_text_file(const char* file)
{
    char cmd[CMD_LEN] = {};
    sprintf(cmd, "file %s | grep \":*text\"", file);
    
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

int main(int argc, const char* argv[])
{
    // Get pid from cmd
    pid_t pid = 0;
    pid = argc > 1 ? atoi(argv[1]) : 1;

    CATCH("create_closet", create_closet());

    char* work_dir[PATH_MAX] = {};
    CATCH("get_work_dir", get_work_dir(pid, work_dir));

    // Save initial files condition
    CATCH("save_dir_content", save_dir_content(work_dir));

    // Process events
    int inotify_fd = 0;
    inotify_fd = inotify_init();

    // Make inotify fd non-blocking 
    fcntl (inotify_fd, F_SETFL, fcntl (inotify_fd, F_GETFL) | O_NONBLOCK);

    inotify_add_watch(inotify_fd, work_dir, IN_MODIFY);

    int T = 5;
    for (int i = 0; i < 5; i++, sleep(T)) 
    {
        log("Try to get sample %d", i);
        poll_sample(inotify_fd, work_dir);
        system("bash get_last_samples.sh 1");
    }
}