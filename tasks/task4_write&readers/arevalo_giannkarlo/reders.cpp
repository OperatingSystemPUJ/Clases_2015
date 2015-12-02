#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 5    /* size of the queue */

int lectores=0;

/* semaphores representing number of free places and write */
sem_t writing, reading,mutex;
//pthread_mutex_t mutex;

void *writer(void *arg)      /* writer function */
{
	while(1){
		printf("[writer] entre\n");
		sem_wait(&writing);
		printf("[writer] agarre\n");
		printf("writing...\n");
		sem_post(&writing);
		printf("[writer] sali\n");
		sleep(rand() % 5);
	}
	pthread_exit(NULL);          /* terminate writer thread */
}

void *reader(void *arg)      /* reader function */
{
	int hilo=(int) arg;
	while(1){
		//pthread_mutex_lock (&mutex);
		printf("entre (%d)\n",hilo );
		sem_wait(&mutex);
		lectores++;
		printf("zona critica 1 (%d)\n",hilo );
		if (lectores==1)
		{
			printf("zona critica 1, esperando writing (%d)\n",hilo );
			sem_wait(&writing);
		}
		sem_post(&mutex);
		printf("salida zona critica 1 (%d)\n",hilo );
		//pthread_mutex_unlock (&mutex);
		printf("read %d leyendo!\n",hilo);
		//pthread_mutex_lock (&mutex);
		sem_wait(&mutex);
		printf("zona critica 2 (%d)\n",hilo );
		lectores--;
		if (lectores==0)
		{
			printf("zona critica 2, libero writing (%d)\n",hilo );
			sem_post(&writing);
		}
		sem_post(&mutex);
		printf("salida zona critica 2 (%d)\n",hilo );
		//pthread_mutex_unlock (&mutex);
		sleep(rand() % 5); 
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t write;
	pthread_t read[SIZE];
	srand(time(0));  /* initialize a random number generator */
	/* initialize a semaphores */
	//pthread_mutex_init(&mutex,NULL);
	sem_init(&reading, 0, SIZE);
	sem_init(&writing, 0, 1);
	sem_init(&mutex, 0, 1);
	/* create writer and reader threads */
	pthread_create(&write, NULL, writer, NULL);
	int i=0;
	for (i = 0; i < SIZE; ++i)
	{
		pthread_create(&read[i], NULL, reader, (void *) i);
	}
	/* wait for terminate writer and reader threads */
	pthread_join(write, NULL);
	for (i = 0; i < SIZE; ++i)
	{
		pthread_join(read[i], NULL);
	}
	//pthread_join(read, NULL);
	/* destroy a semaphores */
	sem_destroy(&reading);
	sem_destroy(&writing);
	sem_destroy(&mutex);
	return 0;
}