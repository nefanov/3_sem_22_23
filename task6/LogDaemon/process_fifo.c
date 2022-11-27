#include <LogDaemon.h>

// returns FIFO fd
int create_fifo()
{
    mkfifo(FIFO_NAME, 0666);

    int fifo_fd = -1;
    fifo_fd = open(FIFO_NAME, O_RDONLY); // block here untill initial write

    return fifo_fd;
}

#define MATCH_CMD(cmd_str, cmd_code) \
    (STR_EQUAL(api_cmd, cmd_str)) return cmd_code;
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
static enum API proc_api(const char* cmd_buffer, int* arg)
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
    if (read_ret == 0 || (read_ret == ERROR && errno == EAGAIN))
        return NO_CMD;

    enum API cmd = proc_api(cmd_buffer, arg);

    return cmd;
}