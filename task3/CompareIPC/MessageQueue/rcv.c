#include <MSQ.h>

/*

Create file
Create msq
Read to buffer
Send msg

*/

int main()
{
    int output_fd = open(OUTPUT_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0600); // perr
    logr("Open intput");

    key_t key = ftok(KEY_FILE, 'b');
    logr("Create key %x", key);

    int msqid = msgget(key, PERMISSIONS);
    logr("Join queue");

    struct messageBuf msg = {};
    size_t msgrcv_ret = 0;
    do
    {
        msgrcv_ret = msgrcv(msqid, &msg, BUFFER_SIZE+sizeof(size_t), 0, 0);
        logr("Received %d bytes", msgrcv_ret);
        size_t msg_len = *((size_t*) msg.mtext);

        if (!msg_len)
            break;

        write(output_fd, msg.mtext + sizeof(msgrcv_ret), msg_len); // perr
        logr("Write");
    }
    while (msgrcv_ret);

    msgctl(msqid, IPC_RMID, 0);
    logr("Destroy queue");

    close(output_fd);
    logr("Close descriptors");

    return 0;
}