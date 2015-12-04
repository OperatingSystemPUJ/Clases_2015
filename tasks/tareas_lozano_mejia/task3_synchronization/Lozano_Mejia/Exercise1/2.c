#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10 
//Column Index
int i=0;
//nxn matrix
int matrix [SIZE][SIZE];
//scalar default value
int scalar=1;
pthread_mutex_t sum_mutex;
//Multiplication function
void* do_work(void *tid){
	int *mytid = (int *) tid;			
	int j;
	
	if (i > SIZE-1){
		return 0;
		}		
	//index i critical zone
	pthread_mutex_lock (&sum_mutex);
	for (j = 0; j < SIZE; j++){
		matrix[i][j] = matrix[i][j]*scalar;
		printf ("Escribi (valor multiplicado) %d en la posicion %d %d hilo %d\n", matrix[i][j],i,j, *mytid);		
		sleep(1);
	}
	pthread_mutex_unlock (&sum_mutex);
	i++;
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	printf("Ingrese el scalar por el que se multiplicara la matriz\n");
	scanf("%d",&scalar);
	int y, w, start, tids[SIZE];	
	pthread_t threads[SIZE];
	pthread_attr_t attr;
	//Matrix Initialization
	for (w = 0; w < SIZE; w++){
		for (y = 0; y < SIZE; y++){
			/* code */
			matrix[w][y]=1;
		}		
	}
	/* Pthreads setup: initialize mutex and explicitly create threads in a
	 joinable state (for portability).  Pass each thread its loop offset */
	pthread_mutex_init(&sum_mutex, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (y=0; y<SIZE; y++) {
		tids[y] = y;
		pthread_create(&threads[y], &attr, do_work, (void *) &tids[y]);
	}
	/* Wait for all threads to complete then print global sum */ 
	for (y=0; y<SIZE; y++) {
		pthread_join(threads[y], NULL);
	}
	for ( w = 0; w < SIZE; w++){
		for (y = 0; y < SIZE; y++){
			/* code */
			printf("Resultado final%d\n",matrix[w][y]);
		}		
	}
	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
}
