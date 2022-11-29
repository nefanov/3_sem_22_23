#include "../headers/ss_log.h"

static int log_fd;

void print_instruction(const char* cmd) 
{
    printf("Usage: %s [PID] [OPTIONS]\n", cmd);
    printf("PID:\n");
    printf("%20s -- pid of tracking process\n", "pid");
    printf("OPTIONS:\n");
    printf("%20s -- start the system service as a daemon\n", "-d, --daemon");
    printf("%20s -- call this instruction\n", "-h, --help");
}

int start_log() 
{
    // Open log file, if -d option
    const char log_path[] = "../log/log.txt";
    log_fd = open(log_path, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    if (log_fd == -1) 
    {
        perror("Can't open log.txt");
        return -1;
    }
    return 0;
}

char* get_time() 
{
    time_t now = time(NULL);
    char* time_mark = ctime(&now);
    time_mark[strlen(time_mark)-1] = '\0';
    
    return time_mark;
}

void create_log(const char* format, ...) 
{
    char* log = (char*)(calloc(PATH_LEN, sizeof(char)));
    char* time_mark = get_time();
    
    strcat(log, time_mark);
    strcat(log, "\t");
    
    va_list ptr;
    va_start(ptr, format);
    char msg[MSG_LEN];
    vsprintf(msg, format, ptr);
    va_end(ptr);
    
    strcat(log, msg);
    strcat(log, "\n");
    
    write(log_fd, log, strlen(log));
    free(log);
}

void print_parsed_maps_line(MapsLine* PML) 
{
    char msg[MSG_LEN] = "\0";
    if (PML->address != NULL) 
    {
        strcat(msg, PML->address);
        strcat(msg, " ");
    }
    if (PML->perms != NULL) 
    {
        strcat(msg, PML->perms);
        strcat(msg, " ");
    }
    if (PML->offset != NULL) 
    {
        strcat(msg, PML->offset);
        strcat(msg, " ");
    }
    if (PML->device != NULL) {
        strcat(msg, PML->device);
        strcat(msg, " ");
    }
    if (PML->inode != NULL) 
    {
        strcat(msg, PML->inode);
        strcat(msg, " ");
    }
    if (PML->path != NULL) 
    {
        strcat(msg, PML->path);
    }
    create_log(msg);
}

void print_diff_PML(MapsLine* Cur, MapsLine* Next) 
{
    create_log("<<<<<<<");
    print_parsed_maps_line(Cur);
    create_log("=======");
    print_parsed_maps_line(Next);
    create_log(">>>>>>>");
}