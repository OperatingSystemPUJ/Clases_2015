1. Ejecute varias veces el codigo, que observa? es correcto? tiene algun error?
   Justifique su respuesta.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS      4
#define ARRAYSIZE   1000000
#define ITERATIONS   ARRAYSIZE / NTHREADS

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

	/* Lock the mutex and update the global sum, then exit */
	pthread_mutex_lock (&sum_mutex);
	sum = sum + mysum;
	pthread_mutex_unlock (&sum_mutex);
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

2. Implementar usando pthreads un programa que:
   Tiene un buffer acotado de tamaño definido por ustedes.
   Lanza 4 threads:  t1;t2;t3;t4.

   Cada thread ti escribe cada i segundos, un número i en la  
   próxima posición libre p del buffer.

   Una vez escrito el elemento el thread ti imprime por pantalla
   “Escribí un i en la posición p del buffer”.

   Se debe cuidar que el acceso al buffer se realice de forma
   exclusiva. 

   Cuando un thread ti descubre que el buffer está lleno, termina.

   El thread original (el que lanzó todos los threads) debe
   esperar a que terminen todos y luego termina.


4. Que esta ocurriendo en este codigo y como lo arreglaria?:
Nota: para compilar agregar -lm

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
   int i;
   double myresult=0.0;
   printf("thread=%ld: starting...\n", threadid);
   for (i=0; i<1000000; i++)
      myresult += sin(i) * tan(i);
   printf("thread=%ld result=%e. Done.\n",threadid,myresult);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  for(t=0;t<NUM_THREADS;t++){
    printf("Main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
      }
    }
  printf("Main: Done.\n");
}


3. Ejecutar el siguiente codigo varias veces, que observa? 
   Como lo podria arreglar?
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Define global data where everyone can see them */
#define NUMTHRDS 8
#define VECLEN 100000
int *a, *b; 
long sum=0;

void *dotprod(void *arg)
{
	/* 
	 * Each thread works on a different set of data.
	 * The offset is specified by the arg parameter. 
	 * The size of the data for each thread is indicated by VECLEN.
	 */
	int i, start, end, offset, len;
	long tid = (long)arg;
	offset = tid;
	len = VECLEN;
	start = offset*len;
	end   = start + len;

	/* Perform my section of the dot product */
	printf("thread: %ld starting. start=%d end=%d\n",tid,start,end-1);
	for (i=start; i<end ; i++) 
	  sum += (a[i] * b[i]);
	printf("thread: %ld done. Global sum now is=%li\n",tid,sum);

	pthread_exit((void*) 0);
}

int main (int argc, char *argv[])
{
	long i;
	void *status;
	pthread_t threads[NUMTHRDS];

	/* Assign storage and initialize values */
	a = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	b = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	  
	for (i=0; i<VECLEN*NUMTHRDS; i++)
	  a[i]= b[i]=1;

	/* Create threads as joinable, each of which will execute the dot product
	 * routine. Their offset into the global vectors is specified by passing
	 * the "i" argument in pthread_create().
	 */
	for(i=0; i<NUMTHRDS; i++) 
	   pthread_create(&threads[i], NULL, dotprod, (void *)i); 

	/* Wait on the threads for final result */
	for(i=0; i<NUMTHRDS; i++) 
	  pthread_join(threads[i], &status);

	/* After joining, print out the results and cleanup */
	printf ("Final Global Sum=%li\n",sum);
	free (a);
	free (b);
	pthread_exit(NULL);
}