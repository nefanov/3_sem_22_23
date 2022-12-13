#include <SHM.h>

/*

Create shm
Attach it
Read from file
Write to shm

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

    int shmid = shmget(key, BUFFER_SIZE + sizeof(size_t), PERMISSIONS | IPC_CREAT);
    logs("Create shm");

    char* shm_region = shmat(shmid, 0, 0);
    logs("Attach shm");

    sem_t* read_sem = sem_open(READ_SEM_NAME, O_CREAT, PERMISSIONS, 0);
    sem_t* write_sem = sem_open(WRITE_SEM_NAME, O_CREAT, PERMISSIONS, 0);
    logs("Open semaphore");

    sem_post(read_sem);
    size_t read_ret = 0;
    do 
    {
        sem_wait(read_sem);
        
            logs("Enter read section\n");

            logs("Passed sem");
            read_ret = read(input_fd, shm_region + sizeof(size_t), BUFFER_SIZE); // perr
            logs("Read %d bytes", read_ret);

            *((size_t*) shm_region) = read_ret;

        sem_post(write_sem);
    }
    while (read_ret);

    shmdt(shm_region);
    logs("Detach shm");

    close(input_fd);
    logs("Close descriptors");

    return 0;
}
