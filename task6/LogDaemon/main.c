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
    log("Proc work dir: %s\n", work_dir);
}

#define CMD_LEN 255
void create_diff (const char* dir, const char* backup)
{
    static int patch_id = 0;
    char cmd[CMD_LEN] = {};
    sprintf(cmd, "diff -bur %s %s > " DIFF_DIR "/%d.txt", backup, dir, patch_id++);
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
    for (;;) 
    {
        /* Read events forever */
        int numRead = read(inotify_fd, buf, BUF_SIZE);
        create_diff(work_dir, DAEMON_DIR);

        if (numRead == 0)
            log("read() from inotify fd returned 0!");

        if (numRead == -1)
            LOG_ERROR("read");

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        /* Process all of the events in buffer returned by read() */
        char* p = NULL;
        for (p = buf; p < buf + numRead; ) 
        {
            struct inotify_event* event = (struct inotify_event *) p;
            log(event->name);
            p += sizeof(struct inotify_event) + event->len;
        }
    }
}