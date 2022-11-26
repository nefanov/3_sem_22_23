#include <LogDaemon.h>
#include <stdio.h>

// returns FIFO fd
int create_fifo()
{
    mkfifo(FIFO_NAME, 0666);

    int fifo_fd = -1;
    CATCH("open fifo", fifo_fd = open(FIFO_NAME, O_RDONLY));

    return fifo_fd;
}

#define MATCH_CMD(cmd_str, cmd_code) \
    (STR_EQUAL(api_cmd, cmd_str)) return cmd_code;

enum API proc_api(const char* cmd_buffer, int* arg)
{   
    char api_cmd[API_CMD_SIZE] = {};
    int ret_val = sscanf(cmd_buffer, "%s %d", api_cmd, arg);

    switch (ret_val)
    {
        case 2:

            if (0) ;
            else if MATCH_CMD("set_T", SET_T)
            else if MATCH_CMD("set_pid", SET_PID)
            else if MATCH_CMD("print_k", PRINT_K)
            else if MATCH_CMD("restore_k", RESTORE_K)
            else return WRONG_CMD;

        case 1:

            if (STR_EQUAL(api_cmd, "stop"))
                return STOP;
            else
                return WRONG_CMD;

        default:

            log("[API_ERRROR] Cant read string %s", cmd_buffer);
            return NO_CMD;
    }
}
#undef MATCH_CMD

enum API read_fifo (int fifo_fd, int* arg)
{
    char cmd_buffer[API_CMD_SIZE] = {};
    size_t read_ret = 0;

    read_ret = read(fifo_fd, cmd_buffer, API_CMD_SIZE); // perr
    if (read_ret == 0) return NO_CMD;

    enum API cmd = proc_api(cmd_buffer, arg);

    return cmd;
}

#define API(...) log(__VA_ARGS__)
int main(int argc, const char* argv[])
{
    logs_ctor();

    int fifo_fd = create_fifo();
    if (fifo_fd == ERROR) return ERROR;

    while(1)
        {
            int arg = -1;
            enum API cmd = read_fifo(fifo_fd, &arg);

            switch (cmd)
            {
                case STOP:
                    API("Stop");
                    exit(EXIT_SUCCESS);

                case SET_T:
                    API("Set time %d", arg);
                    break;

                case SET_PID:
                    API("Set pid %d", arg);
                    break;

                 case PRINT_K:
                    API("Print k %d", arg);
                    break;

                 case RESTORE_K:
                    API("Restore k %d", arg);
                    break;

                case WRONG_CMD:
                    API("Wrong cmd");

                case NO_CMD:
                    break;
            }
        }

    exit(EXIT_SUCCESS);

    pid_t pid = 0;
    enum MODE mode = INTERACT;
    mode = proc_cmd(argc, argv, &pid);

    if (mode == DAEMON)
        skeleton_daemon();
    else 
        printf("[NOTE] Make sure that you start programm with 'sudo'\n");

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

    close(fifo_fd);
    logs_dtor();

    return EXIT_SUCCESS;
}