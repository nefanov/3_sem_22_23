#include <LogDaemon.h>

int create_dir(const char* dir_name)
{
    mode_t old_umask = umask(0);

    int ret = mkdir(dir_name, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        log("mkdir %s", dir_name);
        return ERROR;
    }

    umask(old_umask);
}

// Create a dir for stroring file history
int create_closets()
{
    mode_t old_umask = umask(0);

    CATCH(" ", create_dir(DAEMON_DIR));
    CATCH(" ", create_dir(DIFF_DIR));
    CATCH(" ", create_dir(SAMPLE_DIR));
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