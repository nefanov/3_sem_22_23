#include <LogDaemon.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h> // readlink

#include <string.h> // atoi

#include <sys/inotify.h>
#include <limits.h>

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
            observed_file_path, backup_file, DIFF_DIR, file_name);
    
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

    inotify_add_watch(inotify_fd, work_dir, IN_MODIFY);

    char buf[BUF_SIZE] = {};
    int T = 15;
    for (int i = 0; i < 2; sleep(T)) 
    {
        /* Read events forever, but sampling time >= T */
        int numRead = read(inotify_fd, buf, BUF_SIZE);
        // create_diff(work_dir, DAEMON_DIR);

        if (numRead == 0)
            log("read() from inotify fd returned 0!");

        if (numRead == -1)
            LOG_ERROR("read");

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        /* Process all of the events in buffer returned by read() */
        char* p = NULL;
        int is_text_changed = 0;
        for (p = buf; p < buf + numRead; ) 
        {
            struct inotify_event* event = (struct inotify_event *) p;

            char file[PATH_MAX] = {};
            sprintf(file, "%s/%s", work_dir, event->name);

            log("FILENAME: %s\n", file);

            if (is_text_file(file))
            {   
                log("Is text");
                create_diff(file, event->name);
                is_text_changed = 1;
            }

            p += sizeof(struct inotify_event) + event->len;
        }

        if (is_text_changed)
            {
                create_sample();
                i++;
            }
    }
}