// Implementar usando pthreads un programa que:
//    Tiene un buffer acotado de tamaño definido por ustedes.
//    Lanza 4 threads:  t1;t2;t3;t4.

//    Cada thread ti escribe cada i segundos, un número i en la  
//    próxima posición libre p del buffer.

//    Una vez escrito el elemento el thread ti imprime por pantalla
//    “Escribí un i en la posición p del buffer”.

//    Se debe cuidar que el acceso al buffer se realice de forma
//    exclusiva. 

//    Cuando un thread ti descubre que el buffer está lleno, termina.

//    El thread original (el que lanzó todos los threads) debe
//    esperar a que terminen todos y luego termina.
/////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NTHREADS      4
#define ARRAYSIZE   10
#define ITERATIONS   ARRAYSIZE / NTHREADS

int buffer[ARRAYSIZE]
pthread_mutex_t sum_mutex;

int c=0;

struct  timeval  t0 , t1;

void *do_work(void *tid) 
{
   int i, start, *mytid, end;
   double mysum=0.0;

   /* Initialize my part of the global array and keep local sum */
   mytid = (int *) tid;
   
   while(1){
      pthread_mutex_lock (&sum_mutex); //seccion critica
      gettimeofday(&t1 , NULL);
      unsigned  int  ut1 = t1.tv_sec *1000000+ t1.tv_usec ;
      unsigned  int  ut0 = t0.tv_sec *1000000+ t0.tv_usec ;
      int tmp=(ut1 -ut0)/100;
      buffer[c]=tmp;
      printf ("Thread %d put %d in position %d\n",*mytid,tmp,c); 
      pthread_mutex_unlock (&sum_mutex); //seccionciritica
      sleep(1);
   }
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   int i, start, tids[NTHREADS];
   pthread_t threads[NTHREADS];
   pthread_attr_t attr;

   gettimeofday(&t0, NULL);

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
   for ( i = 0; i < ARRAYSIZE; ++i)
   {
      printf("%d|",buffer[i]);  
      if (i%10==0)
      {
         printf("\n");
      }
   }

   /* Clean up and exit */
   pthread_attr_destroy(&attr);
   pthread_mutex_destroy(&sum_mutex);
   pthread_exit (NULL);
}