#include <pthread.h>
#include <stdio.h>

// gcc -o Program3 Program3.c -lpthread

//      ___      ___                     
//     /\  \    /\  \                    
//    /::\  \  /::\  \                   
//   /:/\:\__\/::\:\__\                  
//   \:\ \/__/\/\::/  /                  
//    \:\__\    /:/  /                   
//     \/__/    \/__/                    
//      ___      ___      ___      ___   
//     /\  \    /\__\    /\  \    /\  \  
//    /::\  \  /:/ _/_  /::\  \  /::\  \ 
//   /:/\:\__\/::-"\__\/::\:\__\/\:\:\__\
//   \:\:\/__/\;:;-",-"\/\::/  /\:\:\/__/
//    \::/  /  |:|  |    /:/  /  \::/  / 
//     \/__/    \|__|    \/__/    \/__/  



/*
	A veces imprime 42, a veces 0.
	Similar al Program1.c, con dos funciones: *thread y *thread2.
	Se crean dos hilos que ejecutan las funciones *thread y *thread2, 
	respectivamente, pero, a diferencia de los anteriores, esta vez la 
	la variable i es inicializada en 42. Como el pthread_join del thread 2
	es NULL entonces el valor retornado del pthread_exit (o sea 31) se pierde,
	sin embargo, *ptr que es un puntero de referencia a la variable i se asigna a 0
	e i queda en 0, es por eso que se imprime un 0 o un 42 (dado que
	el primer hilo retorna el 42 mediante un puntero), dependiendo de la 
	sincronización de los hilos, si entra primero al primer hilo (42) o entra primero
	al segundo hilo (0).
	 
*/

void *thread(void *vargp)
{
	int *ptr = (int*)vargp;
	pthread_exit((void*)*ptr);
}
void *thread2(void *vargp)
{
	int *ptr = (int*)vargp;
	*ptr = 0;
	pthread_exit((void*)31);
}
int main()
{
	int i = 42;
	pthread_t tid, tid2;
	pthread_create(&tid, NULL, thread, (void*)&i);
	pthread_create(&tid2, NULL, thread2, (void*)&i);
	pthread_join(tid, (void**)&i);
	pthread_join(tid2, NULL);
	printf("%d\n",i);
}
