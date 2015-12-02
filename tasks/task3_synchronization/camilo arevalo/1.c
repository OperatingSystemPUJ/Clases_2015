#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS      4
#define ARRAYSIZE   1000000
#define ITERATIONS   ARRAYSIZE / NTHREADS

//Ejecute varias veces el codigo, que observa? es correcto? tiene algun error?
//Justifique su respuesta.
///////////////////////////////////
//Respuesta
//el codigo hace la suma de los primeros n numeros, el codigo usa los mutex para ayudar en la seccion critica
//entre los hilos y asi no causar problemas coherencia, por tanto no tiene ningun error.
double  sum=0.0, a[ARRAYSIZE];
pthread_mutex_t sum_mutex;

void *do_work(void *tid) 
{
	int i, start, *mytid, end;
	double mysum=0.0;

	/* Initialize my part of the global array and keep local sum */
	mytid = (int *) tid;
	start = (*mytid * ITERATIONS);
	end = start + ITERATIONS;
	printf ("Thread %d doing iterations %d to %d\n",*mytid,start,end-1); 
	for (i=start; i < end ; i++) {
		a[i] = i * 1.0;
		mysum = mysum + a[i];
	}
	sleep(1);

	/* Lock the mutex and update the global sum, then exit */
	pthread_mutex_lock (&sum_mutex); //seccion critica
	sum = sum + mysum;
	pthread_mutex_unlock (&sum_mutex); //seccionciritica
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
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
	printf ("Done. Sum= %e \n", sum);

	sum=0.0;
	for (i=0;i<ARRAYSIZE;i++){ 
	a[i] = i*1.0;
	sum = sum + a[i]; }
	printf("Check Sum= %e\n",sum);

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
}