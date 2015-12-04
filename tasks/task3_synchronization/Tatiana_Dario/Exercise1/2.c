/*
Presentado por:
-Tatiana Peña Prias
-Darío Fernández Astudillo


2. Make a program using threads that allow multiply a matrix by a scalar.

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 3
int i=0;
int matrix [SIZE][SIZE];
int scalar=1;
pthread_mutex_t sum_mutex;


void* do_work(void *tid){
	int *mytid = (int *) tid;			
	int j;
	
	if (i > SIZE-1){
		return 0;
		}
	for (j = 0; j < SIZE; j++){
		pthread_mutex_lock (&sum_mutex);
		matrix[i][j] = matrix[i][j]*scalar;
		printf ("Escribi %d en la posicion %d %d hilo %d\n", matrix[i][j],i,j, *mytid);		
		sleep(1);
		pthread_mutex_unlock (&sum_mutex);
	}
	
	i++;
	pthread_exit(NULL);
}



int main(int argc, char *argv[]){
	int y, w, start, tids[SIZE];	
	pthread_t threads[SIZE];
	pthread_attr_t attr;
	printf("los valores de la matriz son: \n");
	for (w = 0; w < SIZE; w++){
		for (y = 0; y < SIZE; y++){
			matrix[w][y]=y+w;
			printf("En la posicion %d %d: %d\n", w,y,matrix[w][y]);
		}		
	}
	printf("Ingrese el scalar por el que se multiplicara la matriz\n");
	scanf("%d",&scalar);
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (y=0; y<SIZE; y++) {
		tids[y] = y;
		pthread_create(&threads[y], &attr, do_work, (void *) &tids[y]);
	}
	for (y=0; y<SIZE; y++) {
		pthread_join(threads[y], NULL);
	}
	for ( w = 0; w < SIZE; w++){
		for (y = 0; y < SIZE; y++){
			printf("Resultado final en la posicion %d %d : %d \n",w,y, matrix[w][y]);
		}		
	}
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
}