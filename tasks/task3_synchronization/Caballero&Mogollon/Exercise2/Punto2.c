#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4
char buffer[100];
int p=0;
pthread_mutex_t bloqueo;


void *agregar(void *tid)
{
	
	int *mytid;
	mytid = (int *) tid;
	
	while(1)
	{
		pthread_mutex_lock (&bloqueo);
		buffer[p]=*mytid;
		printf("Escribí un %d en la posición %d del buffer\n",*mytid,p);
		p++;
		if (p>99)
		{
			exit(1);
		}
		
		//sleep(*mytid);
		pthread_mutex_unlock(&bloqueo);
		sleep(1);
	}
	
	pthread_exit(NULL);

}


int main(int argc, char *argv[]){
	int i, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&bloqueo, NULL);

	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, agregar, (void *) &tids[i]);
	}

	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloqueo);
	pthread_exit(NULL);
}