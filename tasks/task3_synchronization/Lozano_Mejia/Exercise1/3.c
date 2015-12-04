#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NTHREADS 3
int av=0;
int min=1000000;
int max=0;
int lenght=0;
pthread_mutex_t access;

//SOLO FUNCIONA CON 7 NUMEROS
//Average function

void* average(void *vargp){
	int *ptr = (int*) vargp;
	int i=0;	
	pthread_mutex_lock (&access);
	for(i=0;i<lenght;i++){
		av+=ptr[i];		
		printf("Sum avg %d\n",av );
	}	
	av=av/i;	
	printf("Avg %d\n",av);
	pthread_mutex_unlock (&access);
	pthread_exit(NULL);
}

//Minimum function
void* minimum(void *vargp){
	int *ptr = (int*) vargp;
	int i=0;
	pthread_mutex_lock (&access);	
	for(i=0;i<lenght;i++){
		if (ptr[i]<=min){
			min=ptr[i];			
			printf("Current min %d\n",min );
		}		
	}	
	printf("Final min %d\n",min );
	pthread_mutex_unlock (&access);
	pthread_exit(NULL);
}
//Maximum function
void* maximum(void *vargp){
	int *ptr = (int*) vargp;
	int i=0;
	pthread_mutex_lock (&access);
	for(i=0;i<lenght;i++){
		if (ptr[i]>=max){
			max=ptr[i];	
			printf("Current max %d\n",max );		
		}		
	}	
	printf("Final max %d\n",max );
	pthread_mutex_unlock (&access);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	int y, w, start; //tids[SIZE];	
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	printf("Cuantos numeros ingresara:\n");
	scanf("%d",&lenght);
	if (lenght<=0){
		printf("Cantidad invalida\n");
		exit(1);
	}	
	int nums[lenght];
	for (y=0; y < lenght; y++){		
		int tmp;
		printf("Ingrese el valor %d\n",y+1);
		scanf("%d",&tmp);
		nums[y]=tmp;
	}
	/* Pthreads setup: initialize mutex and explicitly create threads in a
	 joinable state (for portability).  Pass each thread its loop offset */
	pthread_mutex_init(&access, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//Average
	pthread_create(&threads[0], &attr, average, (void *) &nums);
	//Minimum
	pthread_create(&threads[1], &attr, minimum, (void *) &nums);
	//Maximum
	pthread_create(&threads[2], &attr, maximum, (void *) &nums);
	
	/* Wait for all threads to complete then print global sum */ 
	for (y=0; y<NTHREADS; y++) {
		pthread_join(threads[y], NULL);
	}
	printf("The average value is: %d\n", av);
	printf("The minimum value is: %d\n", min);
	printf("The maximum value is: %d\n", max);

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&access);
	pthread_exit (NULL);
}
