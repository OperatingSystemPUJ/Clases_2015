/* 2. Implementar usando pthreads un programa que:
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
   esperar a que terminen todos y luego termina. */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# define NTHREADS 4

char buffer[10];

pthread_mutex_t bloq;

void *AgregarNumerosBuffer(void *hilid){

   int *hil;
   hil=(int *) hilid;
   int i=0;

   while(i<10){

      pthread_mutex_lock (&bloq);
      buffer[i]=hil;
      printf("Escribí un %d en la posición %d del buffer\n", *hil,i);
      i=i+1;
      pthread_mutex_unlock(&bloq);
      sleep(1);
   }

   pthread_exit(NULL);



}

int main(int argc, char const *argv[])
{
   /* code */
   int hilos[NTHREADS];
   pthread_t hil[NTHREADS] ;
   pthread_attr_t attr;
   pthread_attr_init(&attr); 
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   pthread_mutex_init(&bloq, NULL);

   pthread_create (&hil[0] , &attr , AgregarNumerosBuffer , ( void *) &hilos[0]);
   pthread_create (&hil[1] , &attr , AgregarNumerosBuffer , ( void *) &hilos[1]);
   pthread_create (&hil[2] , &attr , AgregarNumerosBuffer , ( void *) &hilos[2]);
   pthread_create (&hil[3] , &attr , AgregarNumerosBuffer , ( void *) &hilos[2]);

   pthread_join(hil[0], NULL);
   pthread_join(hil[1], NULL);
   pthread_join(hil[2], NULL);
   pthread_join(hil[3], NULL);

   pthread_attr_destroy (&attr);
   pthread_mutex_destroy (&bloq);
   pthread_exit(NULL);


}
