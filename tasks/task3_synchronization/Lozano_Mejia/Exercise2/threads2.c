/*
Sebastian Lozano H.
Juan Pablo Mejía D.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS   4
#define SIZE 256

int it = 0, buffer[SIZE];
pthread_mutex_t sum_mutex;

void* do_work(void *tid){
	int *mytid = (int *) tid;		
	int end = 0;

	while(1){
		pthread_mutex_lock (&sum_mutex);
		if (it > SIZE-1){
			end = 1;
		} 
		else{		
			buffer[it] = *mytid;
			printf ("Escribi (num hilo) %d en la posicion %d del buffer\n", *mytid, it++);
		}
		pthread_mutex_unlock (&sum_mutex);
		if (end == 1){
			return 0;
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int i, start, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;

	/* Pthreads setup: initialize mutex and explicitly create threads in a
	 joinable state (for portability).  Pass each thread its loop offset */
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, do_work, (void *) &tids[i]);
	}

	/* Wait for all threads to complete then print global sum */ 
	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
}
