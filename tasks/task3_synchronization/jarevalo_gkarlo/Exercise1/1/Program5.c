#include <pthread.h>
#include <stdio.h>

// gcc -o Program5 Program5.c -lpthread

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
	A veces imprime 42, a veces 31.
	Similar al Program3.c, con dos funciones: *thread y *thread2.
	Se crean dos hilos que ejecutan las funciones *thread y *thread2.
	Es un programa sencillo que inicializa globalmente la variable i en 42,
	sin embargo, uno de los hilos asigna i a 31 y el otro solamente imprime 
	la variable i, entonces dependiendo de la sincronizacion de los hilos, se 
	imprimira 31 o 42, si se ejecuta primero el hilo 1, se imprime 42, sino se 
	imprime 31. El retorno del estado de las variables no es usado, el 
	pthread_join solo espera hasta que el hilo respectivo haya acabado.
	 
*/

int i = 42;
void *thread(void *vargp)
{
	printf("%d\n",i);
}
void *thread2(void *vargp)
{
	i = 31;
}
int main()
{
	pthread_t tid, tid2;
	pthread_create(&tid2, NULL, thread2, (void*)&i);
	pthread_create(&tid, NULL, thread, (void*)&i);
	pthread_join(tid, (void**)&i);
	pthread_join(tid2, NULL);
}
