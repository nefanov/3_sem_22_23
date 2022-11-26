#include <LogDaemon.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
    pid_t pid = 0;
    enum MODE mode = INTERACT;
    mode = proc_cmd(argc, argv, &pid);

    if (mode == DAEMON)
        skeleton_daemon();
    else 
        printf("[NOTE] Make sure that you start programm with 'sudo'\n");

    logs_ctor();
    log("meow");
    log("my pid is %d", getpid());

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
    for (int i = 0;; i++, sleep(T)) 
    {
        log("Try to get sample %d", i);
        poll_sample(inotify_fd, work_dir);
        system("bash get_last_samples.sh -k1");
    }
    logs_dtor();

    return EXIT_SUCCESS;
}