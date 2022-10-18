#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>

int main()
{
    sem_t* s = sem_open("/sem1", O_RDWR);

    sem_wait(s);

    printf("Leaving semaphore %lu\n", getpid());
    return 0;
}