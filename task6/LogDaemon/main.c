#include <LogDaemon.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

#include <string.h>

int main(int argc, const char* argv[])
{
    // Get pid from cmd
    pid_t pid = 0;
    pid = argc > 1 ? atoi(argv[1]) : 1;

    // Create a dir for stroring file history
    int ret = mkdir(DAEMON_DIR, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        LOG_ERROR("mkdir");
        return ERROR;
    }

    // Get work dir
    char* proc_dir_path[PATH_MAX] = {};
    sprintf(proc_dir_path, "/proc/%d/cwd", pid);
    // puts(proc_dir_path);

    char* work_dir_path[PATH_MAX] = {};
    CATCH(
          "readlink",
          readlink(proc_dir_path, work_dir_path, PATH_MAX)
         );
    log("Proc work dir: %s\n", work_dir_path);    
}