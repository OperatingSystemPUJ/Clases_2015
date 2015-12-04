/*
Presentado por:
-Tatiana Peña Prias
-Darío Fernández Astudillo

1.Ejecute varias veces el codigo, que observa? es correcto? tiene algun error?Justifique su respuesta.

R/Observamos que se tiene un vector de tamaño 1.000.000 el cual se divide en 4 partes iguales.Se le asigna cada parte a un hilo de la siguiente manera: 
*Hilo 0 = 0 hasta 249.999
*Hilo 1 = 250.000 hasta 499.999
*Hilo 2 = 500.000 hasta 749.999
*Hilo 3 = 750.000 hasta 999.999
Y su parte critica se encuentra en la suma de cada hilo, asegurando un resultado preciso, evitando datos redundantes por causa de interrupciones.

2.Implementar usando pthreads un programa que:
   Tiene un buffer acotado de tamaño definido por ustedes.
   Lanza 4 threads:  t1;t2;t3;t4.

R/ Ver archivo  punto2.c
 

3. Ejecutar el siguiente codigo varias veces, que observa? 
   Como lo podria arreglar?

R/Primero se observa que no tiene los suficientes "mutex" para las variables de la función dotprod, ya que estas variables se usan en todos los hilos.

Para arreglarlo se le agregarian estos Mutex.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Define global data where everyone can see them */
#define NUMTHRDS 8
#define VECLEN 100000
int *a, *b; 
long sum=0;
pthread_mutex_t mutex; //Mutex for the critical section
void *dotprod(void *arg){
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
	//Critical section, mutex is active.
	pthread_mutex_lock (&mutex);	
	printf("thread: %ld starting. start=%d end=%d\n",tid,start,end-1);
	for (i=start; i<end ; i++){
	  sum += (a[i] * b[i]);
	}
	printf("thread: %ld done. Global sum now is=%li\n",tid,sum);
	pthread_mutex_unlock (&mutex);//End of crtical section
	pthread_exit((void*) 0);
}

int main (int argc, char *argv[]){
	long i;
	void *status;
	pthread_t threads[NUMTHRDS];
	pthread_mutex_init(&mutex, NULL);
	/* Assign storage and initialize values */
	a = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	b = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	  
	for (i=0; i<VECLEN*NUMTHRDS; i++)
	  a[i]= b[i]=1;

	/* Create threads as joinable, each of which will execute the dot product
	 * routine. Their offset into the global vectors is specified by passing
	 * the "i" argument in pthread_create().
	 */
	for(i=0; i<NUMTHRDS; i++){
	   pthread_create(&threads[i], NULL, dotprod, (void *)i); 
	}

	/* Wait on the threads for final result */
	for(i=0; i<NUMTHRDS; i++){
	  pthread_join(threads[i], &status);
	}	

	/* After joining, print out the results and cleanup */
	printf ("Final Global Sum=%li\n",sum);
	free (a);
	free (b);
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
}
/*
4. Que esta ocurriendo en este codigo y como lo arreglaria?:

R/El problema que tenia este ejercicio es que el proceso que arroja
el resultado termina antes de los hilos, la solucion la implementamos
al final del programa, haciendo una espera para que los hilos terminaran 
de hacer su ejecucion.



Nota: para compilar agregar -lm
*/
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
    for (t=0; t<NUM_THREADS; ++t) {
    pthread_join(threads[t], NULL);
  }
 
}  
  printf("Main: Done.\n");
}
