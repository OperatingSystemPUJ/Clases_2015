#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 10    /* size of the queue */

int queue[SIZE];   /* queue of products (values) */
int tail, head;    /* tail and head of the queue */

/* semaphores representing number of free places and products */
sem_t empty, products;

void *producer(void *arg)      /* producer function */
{
	int value;
	do      /* produce a values (products) until produced value is 0 */
	{
		int value = rand() % 100;  /* generate a value */
		printf("produced: %d\n", value);
		sem_wait(&empty);          /* wait for free place in the queue */
		queue[tail] = value;       /* insert new value to the queue */
		tail = (tail + 1) % SIZE;
		sem_post(&products);       /* increment number of products in the queue */
		sleep(rand() % 5);         /* rest after production */
	}while(value);
	pthread_exit(NULL);          /* terminate producer thread */
}

void *consumer(void *arg)      /* consumer function */
{
	int value;
	do      /* consume a values (products) until consumed value is 0 */
	{
		sem_wait(&products);  /* wait for products in the queue */
		value = queue[head];  /* take a value from the queue */
		head = (head + 1) % SIZE;
		printf("consumed: %d\n", value);
		sem_post(&empty);         /* increment number of free places in the queue */
		sleep(rand() % 5);        /* consume taken product */
	}while(value);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t prod_t, cons_t;
	srand(time(0));  /* initialize a random number generator */
	/* initialize a semaphores */
	sem_init(&empty, 0, SIZE);
	sem_init(&products, 0, 0);
	/* create producer and consumer threads */
	pthread_create(&prod_t, NULL, producer, NULL);
	pthread_create(&cons_t, NULL, consumer, NULL);
	/* wait for terminate producer and consumer threads */
	pthread_join(prod_t, NULL);
	pthread_join(cons_t, NULL);
	/* destroy a semaphores */
	sem_destroy(&empty);
	sem_destroy(&products);
	return 0;
}