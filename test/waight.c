#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int main()
{

    sem_t *s =  sem_open("/sem1", O_CREAT|O_RDWR);
        if (s != SEM_FAILED)
        sem_wait(s, 1, 5);
    else
        perror("sem_open");
        sem_post(s);
    sem_post(s);
        return 0;
}