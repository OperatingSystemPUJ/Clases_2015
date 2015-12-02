#include <pthread.h>
#include <stdio.h>

// gcc -o Program2 Program2.c -lpthread

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
	NO Imprime 42!
	Similar al Program1.c
	Se crea un hilo que ejecuta la funcion *thread. En esta funcion el 
	hilo intenta terminar por un llamado a exit(), sin embargo es una
	funcion incompatible para threads dado que se usa para terminar un
	proceso, no un hilo. Cuando se compila se imprime un warning de 
	declaracion implicita de la funcion exit(), por tanto, el 42
	no se retorna al proceso padre y no puede imprimirse.
	 
*/

void *thread(void *vargp)
{
	exit(42);
}
int main()
{
	int i;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, (void **)&i);
	printf("%d\n",i);
}
