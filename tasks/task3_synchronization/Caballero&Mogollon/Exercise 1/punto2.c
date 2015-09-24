#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  3
pthread_mutex_t bloqueo;
const int scalar = 2;
const int matrix[3][4]=
{
    {1, 5, 6, 7},
    {4, 4, 8, 0},
    {2, 3, 4, 5}
};

int x=3,y=4;
int matrixsol[3][4];
int i,j=0;
int *mytid;


void *multiplicar(void *tid)
{	
	mytid=(int *) tid;
	pthread_mutex_lock (&bloqueo);		
	for (j=0;j<y;j++)
		matrixsol[*mytid][j]=matrix[*mytid][j]*scalar;		
	pthread_mutex_unlock(&bloqueo);

}


int main(int argc, char *argv[]){
	int tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&bloqueo, NULL);

	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, multiplicar,(void *) &tids[i]);
	}

	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Matriz Original\n");
	for (i=0; i<x; i++){
		for (j=0; j<y;j++)
			printf("%d ",matrix[i][j]);
		printf("\n");
	}
	printf("\nLa multiplicamos por 2\n");
	
	for (i=0; i<x; i++){
		for (j=0; j<y;j++)
			printf("%d ",matrixsol[i][j]);
		printf("\n");
	}
	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloqueo);
	pthread_exit(NULL);
}