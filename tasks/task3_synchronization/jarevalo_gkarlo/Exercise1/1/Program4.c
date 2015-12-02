#include <pthread.h>
#include <stdio.h>

// gcc -o Program4 Program4.c -lpthread

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
	Se crea un hilo que ejecuta la funcion *thread. En este programa
	se usan dos llamados, pthread_detach y pthread_self. El primero
	detach o "desconecta" el hilo, cuando un hilo detach ha terminado,
	sus recursos son automaticamente liberados nuevamente al sistema sin
	la necesidad de que otro hilo espere del resultado de su terminacion, esto
	incluye al proceso padre tambien. El segundo se usa para capturar el id
	del hilo, el mismo valor que retorna en la creacion de un hilo.
	
	A pesar del uso de estas dos funciones para el hilo, pthread_detach y pthread_self, 
	su invocacion se hace dentro de la funcion del hilo y no desde el padre, 
	por lo tanto, no hay un hilo que se involucre en ese llamado ni afecte a otro proceso.
	Es entonces que el pthread_exit retorna el 42 normalmente y el pthread_join que 
	esta en el padre recibe ese valor y lo imprime desde la variable i.
*/

void *thread(void *vargp)
{
	pthread_detach(pthread_self());
	pthread_exit((void*)42);
}
int main()
{
	int i = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, (void*)&i);
	pthread_join(tid, (void**)&i);
	printf("%d\n",i);
}
