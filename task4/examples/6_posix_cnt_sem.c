/* This is example of POSIX counting semaphore usage. It differs from SYS V semaphores which are basic in lectures course.
Nevertheless, it is more logically correspond to Dijksta's classical semaphores (1965) and recommended for usage too.
Finally, the POSIX implementation is more modern than SYS V.
*/
/* usage of this example:
compile this code by command "gcc 6_posix_cnt_sem.c -o posix_cnt_sem", then run 2 terminals;
type "./posix_cnt_sem" into the first of them and "./posix_cnt_sem 1" in the second, then
get the output and interpret the results.
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>

#define SEMAPHORE_NAME "/my_named_posix_semaphore"

int main(int argc, char ** argv) {
    sem_t *sem;

    if ( argc == 2 ) {
        printf("Dropping semaphore...\n");
        if ( (sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED ) {
            perror("sem_open");
            return 1;
        }
        sem_post(sem);
        perror("sem_post");
        printf("Semaphore dropped.\n");
        return 0;
    }

    if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0776, 0)) == SEM_FAILED ) {
        perror("sem_open");
        return 1;
    }

    printf("Semaphore is taken.\nWaiting for it to be dropped.\n");
    if (sem_wait(sem) < 0 )
        perror("sem_wait");
    if ( sem_close(sem) < 0 )
        perror("sem_close");

    return 0;
}
