#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	3
float var_promedio=0;
int var_minimo=0;
int var_maximo=0;
int longitud=0;
pthread_mutex_t bloqueo;

void *minimo(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloqueo);
	var_minimo=ptr[0];
	while(cont<longitud){
		if (var_minimo>ptr[cont]){
			var_minimo=ptr[cont];
		}
		cont++;
	}
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(NULL);
}

void *maximo(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloqueo);
	while(cont<longitud){
		if (var_maximo<ptr[cont]){
			var_maximo=ptr[cont];
		}
		cont++;
	}
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(NULL);
}

void *promedio(void *array)
{
	int *ptr = (int*)array;
	int cont=0;
	pthread_mutex_lock(&bloqueo);
	while(cont<longitud){
		var_promedio=var_promedio+ptr[cont];
		cont++;
	}
	pthread_mutex_unlock(&bloqueo);
	var_promedio=var_promedio/cont;
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int cont,tmp,t;
	pthread_t threads[NUM_THREADS];
	pthread_mutex_init(&bloqueo,NULL);
	printf("Ingrese la cantidad de numeros");
	scanf("%d",&longitud);
	int array[longitud];
	for (cont=0;cont<longitud;cont++){
		printf("Ingrese un numero: ");
		scanf("%d",&tmp);
		array[cont]=tmp;
	}
	pthread_create(&threads[0], NULL, minimo, (void *)&array);
	pthread_create(&threads[1], NULL, maximo, (void *)&array);
	pthread_create(&threads[2], NULL, promedio, (void *)&array);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	printf("\n");
	printf("The average value is %f\n",var_promedio );
	printf("The minimum value is %d\n",var_minimo );
	printf("The maximum value is %d\n",var_maximo );

	pthread_mutex_destroy(&bloqueo);
	pthread_exit(NULL);
}