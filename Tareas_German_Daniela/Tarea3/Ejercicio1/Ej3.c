//Funciona bien!!

/*3. Write a multithreaded program that calculates various statistical values
for a list of numbers. This program will be passed a series of numbers on
the command line and will then create three separate worker threads.
One thread will determine the average of the numbers, the second
will determine the maximum value, and the third will determine the
minimum value. For example, suppose your program is passed the
integers: 90 81 78 95 79 72 85
The program will report:

The average value is 82
The minimum value is 72
The maximum value is 95

The variables representing the average, minimum, and maximum values
will be stored globally. The worker threads will set these values, and the
parent thread will output the values once the workers have exited.*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NTHREADS 3

int promedio=0;
int maximo=0;
int minimo=1000;
int longitud=0;
pthread_mutex_t bloqueo;

void* Valor_minimo(void *l){
	int *p = (int*)l;
	int a=0;
	
	pthread_mutex_lock(&bloqueo);
	for (a=0; a < longitud; a++)
	{
		if (p[a]<=minimo){
			minimo=p[a];


		}
		
	}
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(NULL);


}

void* Valor_maximo(void *l){
	int a;
	int *p = (int*)l;
	pthread_mutex_lock(&bloqueo);
	for (a=0; a < longitud; a++)
	{
		if (p[a]>=maximo){
			maximo=p[a];


		}
		
	}
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(NULL);


}

void* Valor_promedio(void *l){
	int *p = (int*) l;

	int a=0;
	
	pthread_mutex_lock(&bloqueo);
	for (a=0; a < longitud; a++)
	{
		promedio=promedio+p[a];
	}
	promedio=promedio/a;
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(NULL);


}

int main(int argc, char *argv[])
{
	int b;
	
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	printf("Ingrese la cantidad de numeros que desee operar: \n");
	scanf("%d",&longitud);

	int lista[longitud];
	for (b = 0; b < longitud; b++)
	{
		int numeros;
		//printf("Valor %d\n", b+1);
		scanf("%d", &numeros);
		lista[b]=numeros;
		//printf("a");
		
	}

	pthread_mutex_init(&bloqueo, NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&threads[0], &attr, Valor_minimo, (void *) &lista);
	
	pthread_create(&threads[1], &attr, Valor_maximo, (void *) &lista);
	
	pthread_create(&threads[2], &attr, Valor_promedio, (void *) &lista);
	
	for (b=0; b<NTHREADS; b++) {
		pthread_join(threads[b], NULL);
	}
	printf("Minimo: %d\n", minimo);
	printf("Maximo: %d\n", maximo);
	printf("Promedio: %d\n", promedio);

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&bloqueo);
	pthread_exit (NULL);
}