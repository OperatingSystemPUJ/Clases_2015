/*
Presentado por:
-Tatiana Peña Prias
-Darío Fernández Astudillo


3. Write a multithreaded program that calculates various statistical values
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
parent thread will output the values once the workers have exited.


*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	3
float v_average;
int v_minimum;
int v_maximum;
int size;
pthread_mutex_t bloq;

void *minimum(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloq);
	v_minimum=ptr[0];
	while(cont<size ){
		if (v_minimum>ptr[cont]){
			v_minimum=ptr[cont];
		}
		cont++;
	}
	pthread_mutex_unlock(&bloq);
	pthread_exit(NULL);
}

void *maximum(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloq);
	while(cont<size ){
		if (v_maximum<ptr[cont]){
			v_maximum=ptr[cont];
		}
		cont++;
	}
	pthread_mutex_unlock(&bloq);
	pthread_exit(NULL);
}

void *average(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloq);
	while(cont<size ){
		v_average=v_average+ptr[cont];
		cont++;
	}
	pthread_mutex_unlock(&bloq);
	v_average=v_average/cont;
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int cont,tmp,t;
	pthread_t threads[NUM_THREADS];
	printf("Ingrese la cantidad de numeros: ");
	scanf("%d",&size );
	int array[size];
	for (cont=0;cont<size ;cont++){
		printf("Ingrese un numero: ");
		scanf("%d",&tmp);
		array[cont]=tmp;
	}
	pthread_mutex_init(&bloq,NULL);
	pthread_create(&threads[0], NULL, minimum, (void *)&array);
	pthread_create(&threads[1], NULL, maximum, (void *)&array);
	pthread_create(&threads[2], NULL, average, (void *)&array);


	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	printf("\n");
	printf("The average value is %f\n",v_average );
	printf("The minimum value is %d\n",v_minimum );
	printf("The maximum value is %d\n",v_maximum );

	pthread_mutex_destroy(&bloq);
	pthread_exit(NULL);
}