#include <pthread.h>
#include <stdio.h>

// gcc -o Program1 Program1.c -lpthread

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
	Imprime 42!
	Se crea un hilo que ejecuta la funcion *thread. En esta funcion el 
	hilo termina por un llamado a pthread_exit que retorna el 42. El
	proceso padre recibe el numero con el pthread_join y se imprime con el printf.
*/

void *thread(void *vargp)
{
	pthread_exit((void*)42);	
}
int main()
{
	int i;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, (void **)&i);
	printf("%d\n",i);
}
