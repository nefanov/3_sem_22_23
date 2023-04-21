#include "Creator.h"



int main()
{
    int total = 0;
    double step;

    step = (double)(DOMAIN_UP - DOMAIN_DOWN) / (double)THREADS;

    while(total < THREADS)
    {
        double args[] = {(double)(DOMAIN_DOWN + total * step), (double)(DOMAIN_DOWN + (total + 1) * step), (double)RANGE_DOWN, (double)RANGE_UP};
    
        if (pthread_create(&(tid[total]), NULL, &counter, (void*)args) != 0)
        {
        	perror("pthread_create");
        }
        
        total++;
        sleep(0.001);
    }

    for (int thread = 0; thread < THREADS; thread++)
    {
    	pthread_join(tid[thread], NULL); 
    }
    
    printf("Количество потоков в программе: %d\nИнтеграл: %.2f\n", THREADS, integral);

    return 0;
}

