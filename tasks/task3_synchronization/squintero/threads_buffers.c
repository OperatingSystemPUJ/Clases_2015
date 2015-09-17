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
 *     Creado por: Santiago Quintero     
 */

 /*
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS   4
#define BUFFERSIZE 256

int iter = 0, buffer[BUFFERSIZE];
pthread_mutex_t sum_mutex;

void* do_work(void *tid)
{
	int *mytid = (int *) tid;
	*mytid += 1;
	int finish = 0;

	while (1)
	{
		pthread_mutex_lock (&sum_mutex);
		if (iter >= BUFFERSIZE) finish = 1;
		else
		{		
			buffer[iter] = *mytid;
			printf ("Escribi %d en la posicion %d del buffer\n", *mytid, iter++);
		}
		pthread_mutex_unlock (&sum_mutex);
		if (finish == 1) break;
		sleep(*mytid);
	}
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

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
}
