#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <pthread.h>

#define STORAGE_MIN 0
#define STORAGE_MAX 20

/* Разделяемый ресурс */
int storage = STORAGE_MIN;

pthread_mutex_t mutex;
pthread_cond_t condition;

/* Функция потока потребителя */
void *consumer(void *args)
{
	puts("[CONSUMER] thread started");
	int toConsume = 0;
	
	while(1)
	{
		pthread_mutex_lock(&mutex);
		/* Если значение общей переменной меньше максимального, 
		 * то поток входит в состояние ожидания сигнала о достижении
		 * максимума */
		while (storage < STORAGE_MAX)
		{
			pthread_cond_wait(&condition, &mutex);
		}
		toConsume = storage-STORAGE_MIN;
		printf("[CONSUMER] storage is maximum, consuming %d\n", \
			toConsume);
		/* "Потребление" допустимого объема из значения общей 
		 * переменной */
		storage -= toConsume;
		printf("[CONSUMER] storage = %d\n", storage);
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
}

/* Функция потока производителя */
void *producer(void *args)
{
	puts("[PRODUCER] thread started");
	
	while (1)
	{
		usleep(200000);
		pthread_mutex_lock(&mutex);
		/* Производитель постоянно увеличивает значение общей переменной */
		++storage;
		printf("[PRODUCER] storage = %d\n", storage);
		/* Если значение общей переменной достигло или превысило
		 * максимум, поток потребитель уведомляется об этом */
		if (storage >= STORAGE_MAX)
		{
			puts("[PRODUCER] storage maximum");
			pthread_cond_signal(&condition);
		}
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int res = 0;
	pthread_t thProducer, thConsumer;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);
	
	res = pthread_create(&thProducer, NULL, producer, NULL);
	if (res != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&thConsumer, NULL, consumer, NULL);
	if (res != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	pthread_join(thProducer, NULL);
	pthread_join(thConsumer, NULL);
	
	return EXIT_SUCCESS;
}
