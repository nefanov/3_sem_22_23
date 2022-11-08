#include <SHM.h>

/*

Create shm
Attach it
Read from file
Write to shm

*/

int main()
{
    int output_fd = open(OUTPUT_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0600); // perr
    logr("Open output");

    char cmd_buf[MAX_CMD_LEN] = {};
    sprintf(cmd_buf, "touch %s", KEY_FILE);
    system(cmd_buf);
    logs("Create file for ftok");

    key_t key = ftok(KEY_FILE, 'b');
    logr("Create key %x", key);

    int shmid = shmget(key, BUFFER_SIZE + sizeof(size_t), PERMISSIONS);
    logr("Create shm");

    char* shm_region = shmat(shmid, 0, 0);
    logr("Attach shm");
     
    
    sem_t* read_sem = sem_open(READ_SEM_NAME, O_CREAT, PERMISSIONS, 0);
    sem_t* write_sem = sem_open(WRITE_SEM_NAME, O_CREAT, PERMISSIONS, 0);
    logr("Open semaphore");

    size_t write_ret = 0;
    do
    {
        sem_wait(write_sem);

            logr("Enter write section\n");

            size_t chunk_len = *((size_t*) shm_region);
            logr("Chunk len is %u", chunk_len);
            
            if (!chunk_len)
                {
                    logr("Zero chunk len");
                    break;
                }

            write_ret = write(output_fd, shm_region + sizeof(size_t), chunk_len); // perr
            logr("Write %d bytes", write_ret);

        sem_post(read_sem);
    }
    while (write_ret);

    sem_close(read_sem);
    sem_close(write_sem);

    shmdt(shm_region);
    logr("Detach shm");

    close(output_fd);
    logr("Close descriptors");

    return 0;
}
