#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 5    /* size of the queue */

int lectores=0;

/* semaphores representing number of free places and pelquear */
sem_t BarberoOcupado, SiendoPeluqueado,mutex;

void *Barber(void *arg){

while(1){
		printf("Barbero Desocupado\n");
		sem_wait(&BarberoOcupado);
		printf("[Cliente] Llego Cliente\n");
		printf("BarberoOcupado...\n");
		sem_post(&BarberoOcupado);
		printf("Barbero Acabe, Libre de nuevo\n");
		sleep(rand() % 5);
	}
	pthread_exit(NULL);          /* terminate pelquearr thread */

}


void *Client(void *arg){

int hilo=(int) arg;
	while(1){
		//pthread_mutex_lock (&mutex);
		printf("Cliente Numero (%d) llego a peluquearse \n",hilo );
	    lectores++;
		sem_wait(&mutex);
	

		printf("Hay %d clientes \n",lectores);
		//printf("zona critica 1 (%d)\n",hilo );
		if (lectores>=1)
		{
			sem_wait(&BarberoOcupado);
			printf("El cliente (%d) esta siendo atendido, esperando BarberoOcupado\n",hilo );
			
		}
		sem_post(&mutex);
	
		
		printf("Cliente %d peluqueado!\n",hilo);
		
		sem_wait(&mutex);
		
		lectores--;
		printf("Ahora mismo hay %d clientes \n",lectores);

		if (lectores==0)
		{
			printf("No hay clientes esperando, barbero puede irse a dormir porque el cliente (%d) ya esta peluqueado\n" ,hilo );
			sem_post(&BarberoOcupado);
		}
		sem_post(&mutex);
		
		sleep(rand() % 5); 
	}
	pthread_exit(NULL);
	


}

int main(int argc, char *argv[])
{
	pthread_t pelquear;
	pthread_t read[SIZE];
	srand(time(0));  /* initialize a random number generator */
	/* initialize a semaphores */
	
	sem_init(&SiendoPeluqueado, 0, SIZE);
	sem_init(&BarberoOcupado, 0, 1);
	sem_init(&mutex, 0, 1);
	/* create pelquearr and reader threads */
	pthread_create(&pelquear, NULL, Barber, NULL);
	int i=0;
	for (i = 0; i < SIZE; ++i)
	{
		pthread_create(&read[i], NULL, Client, (void *) i);
	}
	/* wait for terminate pelquearr and reader threads */
	pthread_join(pelquear, NULL);
	for (i = 0; i < SIZE; ++i)
	{
		pthread_join(read[i], NULL);
	}
	/* destroy a semaphores */
	sem_destroy(&SiendoPeluqueado);
	sem_destroy(&BarberoOcupado);
	sem_destroy(&mutex);
	return 0;
}