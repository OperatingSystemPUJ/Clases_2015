
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 

void *func1(void *arg) 
{ 
	pthread_t tid; 
    tid = pthread_self();
          
    printf("Soy el thread 1 [TID:%ld] y voy a esperar\n",tid); 
    sleep(10);

    printf("Soy el thread 1 [TID:%ld] y he terminado\n", tid); 
	pthread_exit(NULL); 
} 

int main(void) 
{
	pthread_t thread1, thmain;
	pthread_attr_t attr;
    thmain = pthread_self();
    pthread_attr_init (&attr);
    printf("Soy main [TID: %ld] y voy a lanzar un thread\n", thmain); 
	pthread_create(&thread1, &attr, func1, NULL); 
    sleep(2); 

    if (pthread_cancel(thread1)!=0) 
        perror("Error cancelando thread..."); 
    else 

	printf("Soy main [TID: %ld] he cancelado el thread [TID: %ld] y termino bien\n", thmain, thread1); 
    pthread_exit(NULL); 
} 