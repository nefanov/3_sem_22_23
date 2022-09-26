/* Задание: запустить данную программу и убедиться, что последовательность вывода порождаемых потоков нарушается: чем дольше они работают,
тем "сильнее разбегаются" значения, выведенные ими в stdout. Такое поведение-следствие работы планировщика.
Замечание: компилировать нужно с опцией -pthread, а если Вы на Windows и компилируете MinGW, то -mthread.
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];

void* thread_stuff(void *arg)
{
    unsigned long i = 0;
    pthread_t id = pthread_self();

    for (i=0;i<0xFFFFFFFF;i++) {
        if(pthread_equal(id,tid[0])) {
                printf("1:thread %lu on iteration %lu\n", (unsigned long)id, i);
        } else {
                printf("2:thread %lu on iteration %lu\n", (unsigned long)id, i);
        }
     }
    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    while(i < 2) {
        err = pthread_create(&(tid[i]), NULL, &thread_stuff, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread %d created successfully with tid=%lu\n", i, (unsigned long)tid[i]);
        i++;
    }

    sleep(5);
    return 0;
}
