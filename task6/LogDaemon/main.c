#include <LogDaemon.h>
#include <stdio.h>

#define API(...) log("[API] :" __VA_ARGS__)
int main(int argc, const char* argv[])
{
    create_dir("info");

    logs_ctor();
    register_signals();

    FILE* file_terminal = fopen(DAEMON_TERMINAL, "w"); 

    pid_t pid = 0;
    enum MODE mode = INTERACT;
    printf("[NOTE] Make sure that you start program with 'sudo'\n");
    mode = proc_cmd(argc, argv, &pid);

    if (mode == DAEMON)
        skeleton_daemon();

    log("Daemon pid is %d", getpid());

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

    int fifo_fd = create_fifo();
    if (fifo_fd == ERROR) return ERROR;
    fcntl (fifo_fd, F_SETFL, fcntl (inotify_fd, F_GETFL) | O_NONBLOCK);

    inotify_add_watch(inotify_fd, work_dir, IN_MODIFY);

    int T = 5;
    char cmd_buffer[CMD_LEN] = {};
    int DAEMON_ALIVE = 1;
    for (int i = 0; DAEMON_ALIVE; i++, sleep(T)) 
    {
        log("Try to get sample %d", i);

        int arg = -1;
        enum API cmd = read_fifo(fifo_fd, &arg);

        switch (cmd)
        {
            case STOP:
                API("Stop");
                DAEMON_ALIVE = 0;
                break;

            case SET_T:
                API("Set time %d", arg);
                T = arg;
                break;

            // case SET_PID:
            //     API("Set pid %d", arg);
            //     break;

            case PRINT_K:
                API("Print k %d", arg);
                sprintf(cmd_buffer, "bash print_diff.sh %d %s",
                        arg, DAEMON_TERMINAL);
                system(cmd_buffer);
                break;

            case RESTORE_K:
                API("Restore k %d", arg);
                sprintf(cmd_buffer, "bash restore_k.sh %s %d",
                        work_dir, arg);
                system(cmd_buffer);
                break;

            case WRONG_CMD:
                API("Wrong cmd");

            case NO_CMD:
                break;
        }

        poll_sample(inotify_fd, work_dir);
        system("bash get_last_samples.sh -k1");
    }

    fclose(file_terminal);
    close(fifo_fd);
    logs_dtor();

    return EXIT_SUCCESS;
}
#undef API