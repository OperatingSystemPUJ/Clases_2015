/*     #######             #####         
 *  #############       ###########      
 * ####       ####    ####       ####    
 *###           ###  ###           ###   
 * ####              ###           ###   
 *   #####           ###           ###   
 *     #####         ###           ###   
 *       #####       ###  ##       ###   
 *         #####     ###  ####     ###   
 *           #####   ###    ####   ###   
 *###           ###  ###      #### ###   
 * ####       ####    ####      #####    
 *  #############       ###############
 *     #######             #####    ####
 *         Santiago  Quintero     
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 10    /* number of readers */
int magic_value = 0;   /* value to read and modify */   

/* semaphores representing locks for ONE writer or many readers */
sem_t write_lock, lock;
int readers_count = 0;    /* readers accessing magic_value */

void *writer(void *arg)
{
	int value;
	do      /* produce a values (products) until produced value is 0 */
	{
		sem_wait(&write_lock);          /* wait for no readers left */
		int value = rand() % 100;  /* generate a value */
		printf("writed: %d\n", value);
		magic_value = value;       /* replace magic container with new value */
		sem_post(&write_lock);       /* allow readers or writers to access magic_value */
		sleep(rand() % 5);         /* rest after writing */
	}while(1);
	pthread_exit(NULL);          /* terminate producer thread */
}

void *reader(void *arg)
{
	int value;
	do      /* reads a values (magic_value) */
	{
		sem_wait(&lock);  /* wait for readers to modify reader_count */
		readers_count++;  /* announce that a new reader is reading the value */
		if (readers_count == 1)
		{
			sem_wait(&write_lock); /* Wait for no writers altering magic value */
		}
		sem_post(&lock); /* Allow other readers to announce their interactions */

		value = magic_value;  /* read the value from magic_value */
		printf("readed: %d\n", value);

		sem_wait(&lock);  /* wait for readers to modify reader_count */
		readers_count--;  /* announce that a new reader is reading the value */
		if (readers_count == 0)
		{
			sem_post(&write_lock); /* Allow writers to access magic_value */
		}
		sem_post(&lock); /* Allow other readers to announce their interactions */
		sleep(rand() % 5);    /* rest after reading */
	}while(1);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t writer1_t, writer2_t, threads[NTHREADS];
	srand(time(0));  /* initialize a random number generator */
	/* initialize a semaphores */
	sem_init(&lock, 0, 1);
	sem_init(&write_lock, 0, 1);
	/* create writers and readers threads */
	pthread_create(&writer1_t, NULL, writer, NULL);
	pthread_create(&writer2_t, NULL, writer, NULL);

	for (i=0; i<NTHREADS; i++) {
        pthread_create(&threads[i], NULL, reader, NULL);
    }

	/* wait for terminate writers threads */
	pthread_join(writer1_t, NULL);
	pthread_join(writer2_t, NULL);
    
    /* Wait for all reader threads to complete */ 
    for (i=0; i<NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
	/* destroy a semaphores */
	sem_destroy(&lock);
	sem_destroy(&write_lock);
	return 0;
}