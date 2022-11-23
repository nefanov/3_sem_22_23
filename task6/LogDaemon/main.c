#include <LogDaemon.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
    // Get pid from cmd
    pid_t pid = 0;
    pid = argc > 1 ? atoi(argv[1]) : 1;

    CATCH("create_closets", create_closets());

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
        system("bash get_last_samples.sh -k1");
    }
}