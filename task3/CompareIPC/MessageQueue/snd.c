#include <MSQ.h>

/*

Create file
Create msq
Read to buffer
Send msg

*/

int main()
{
    int input_fd = open(INPUT_NAME, O_RDONLY); // perr
    logs("Open intput");

    char cmd_buf[MAX_CMD_LEN] = {};
    sprintf(cmd_buf, "touch %s", KEY_FILE);
    system(cmd_buf);
    logs("Create file for ftok");

    key_t key = ftok(KEY_FILE, 'b');
    logs("Create key %x", key);

    int msqid = msgget(key, PERMISSIONS | IPC_CREAT);
    logs("Create queue");

    struct messageBuf msg = {};
    msg.mtype = 1;
    size_t read_ret = 0;
    do
    {
        read_ret = read(input_fd, msg.mtext + sizeof(read_ret), BUFFER_SIZE); // perr
        logs("Read %d bytes", read_ret);
        
        *((size_t*) msg.mtext) = read_ret; 

        msgsnd(msqid, &msg, BUFFER_SIZE+sizeof(size_t), 0);
        logs("Send message");
    }
    while (read_ret);

    close(input_fd);
    logs("Close descriptors");

    return 0;
}