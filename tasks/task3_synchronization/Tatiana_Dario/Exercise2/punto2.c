#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4
#define SIZE 10
char buffer[SIZE];
int p = 0;
pthread_mutex_t bloq;


void *algo(void *tid)
{
	
	int *mytid;
	mytid = (int *) tid;
	
	while(1){	
		pthread_mutex_lock (&bloq);
		buffer[p]=*mytid;
		p++;
		pthread_mutex_unlock(&bloq);
		printf("Escribí un %d en la posición %d del buffer\n",*mytid,p);
		sleep(1);		
		if (p>SIZE-1)
		{
			exit(1);
		}
	}

	
	pthread_exit(NULL);

}


int main(int argc, char *argv[]){
	int i, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&bloq, NULL);

	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, algo, (void *) &tids[i]);
	}

	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloq);
	pthread_exit(NULL);
}