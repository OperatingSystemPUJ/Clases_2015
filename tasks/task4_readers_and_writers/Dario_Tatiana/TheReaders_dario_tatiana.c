#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 10
#define BUF_SIZE 5


sem_t mutex;
sem_t mutex_Rw;
int read_count;


void Reader (void *ptr);
void Writer (void *ptr);


int main(int argc, char const *argv[]){

	sem_init(&mutex, 0, 5);
	sem_init(&mutex_Rw, 0, 1);

	int rc, j;
	int i[11] = {0,1,2,3,4,5,6,7,8,9,10};

	pthread_t tidW;
	pthread_create(&tidW, NULL, (void *) &Writer, (void *) &i[0]);

	pthread_t tid;
	for (j = 1; j <= NTHREADS; j++){
		rc = pthread_create(&tid, NULL, (void *) &Reader, (void *) &i[j]);
	}

	sem_destroy(&mutex);
	sem_destroy(&mutex_Rw);
	pthread_exit(NULL);

	return 0;
}

void Writer(void *ptr){
	int x;
	x = *((int *) ptr);
	do {
		sem_wait(&mutex_Rw);
		printf("Thread %d: Esta actualizando la informacion\n" , x);
		sleep(5);
		sem_post(&mutex_Rw);	

	} while(1);
	pthread_exit(NULL);
}

void Reader(void *ptr){
	int x;
	x = *((int *) ptr);

	do {
		printf("Thread %d: Esta esperando\n", x);
		sem_wait(&mutex);
		read_count++;

		if (read_count == 1){
			sem_wait(&mutex_Rw);
		}

		sem_post(&mutex);

		printf("Thread %d: Esta leyendo la informacion \n", x);
		sleep(2);

		sem_wait(&mutex);
		read_count--;

		if (read_count == 0){
			sem_post(&mutex_Rw);
		}
		sem_post(&mutex);
		
	} while(1);

	//printf("Thread %d: Esta esperando\n", x);

	pthread_exit(NULL);

}