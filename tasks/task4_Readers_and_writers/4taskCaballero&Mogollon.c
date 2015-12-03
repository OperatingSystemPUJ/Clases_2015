#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>


#define Cantidad 10

int leen=0;


sem_t escritor;
sem_t lector;
sem_t mutex;


void *lea(void *arg)      
{
	int nmrHilo=(int) arg;
	while(1){		
		sem_wait(&mutex);
		leen++;
		if (leen==1)
		{
			printf("No se puede leer, el (%d) escribe\n",nmrHilo );
			sem_wait(&escritor);
		}
		sem_post(&mutex);
		printf("Leyendo %d\n",nmrHilo);
		sem_wait(&mutex);
		leen--;
		if (leen==0)
		{
			printf("Escritor libre (%d)\n",nmrHilo );
			sem_post(&escritor);
		}
		sem_post(&mutex);
		sleep(rand() % 5); 
	}
	pthread_exit(NULL);
}


void *escriba(void *arg)  
{
	while(1){
		printf("Escribiendo\n");
		sem_wait(&escritor);
		printf("Escribiendo\n");
		sem_post(&escritor);
		printf("Escribiendo\n");
		sleep(rand() % 5);
	}
	pthread_exit(NULL);          
}


int main(int argc, char *argv[])
{
	int x;
	pthread_t Lectores[Cantidad];
	pthread_t Escritor;
	srand(time(0));  
	sem_init(&lector, 0, Cantidad);
	sem_init(&escritor, 0, 1);
	sem_init(&mutex, 0, 1);
	pthread_create(&Escritor, NULL, escriba, NULL);
	for (x = 0; x < Cantidad; ++x)
	{
		pthread_create(&Lectores[x], NULL, lea, (void *) x);
	}
	pthread_join(Escritor, NULL);
	for (x = 0; x < Cantidad; ++x)
	{
		pthread_join(Lectores[x], NULL);
	}
	sem_destroy(&lector);
	sem_destroy(&escritor);
	sem_destroy(&mutex);
	return 0;
}