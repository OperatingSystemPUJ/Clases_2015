//Ejercicio 2 = Multiplicar matriz por un escalar

//Make a program using threads that allow multiply a matrix by a scalar.
//http://www.um.es/earlyadopters/actividades/a3/PCD_Activity3_Session1.pdf

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# define NTHREADS 3
int escalar=3;
int matriz[3][3];
pthread_mutex_t bloq;
int *t;


//Inicializa la matriz con algunos valores
/*void init ( float m [3][3]) {
	int i;
	int j;

	for ( i = 0 ; i < 3 ; i ++ ) {
		for ( j = 0 ; j < 3 ; j ++ ) {
			matriz[i][j] = random () *100;
			//printf("%d", m[i][j]);
		}

	}

}*/

//Multiplica la matriz por el escalar 3
void * matrizporescalar ( void * arg ) {
	t=(int *) arg;
	printf("t: %s\n",t );
	pthread_mutex_lock (&bloq);

	
	int i;
	
	
	for ( i = 0 ; i < 3 ; i ++ ) {
		//printf (" Hilo %d multiplicando fila %d \n" , p -> id , i );
		
			matriz [ *t ][ i] =  matriz [ *t ][ i] * escalar ;
			
			//usleep (100000) ;
	pthread_mutex_unlock(&bloq);
		
	}
}

int main ( int argc , char * argv []) {
	int hilos[NTHREADS];
	pthread_t hil[NTHREADS] ;
	pthread_attr_t attr;
	pthread_attr_init(&attr); 
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&bloq, NULL);
	//pthread_t h2 ;
	//struct parametros p1 ;
	//struct parametros p2 ;
	/*p1.id = 1;
	p1.escalar = 5.0;
	init ( p1.matriz );
	p2.id = 2;
	p2.escalar = 10.0;
	init ( p2 . matriz );
	pthread_create (& h1 , NULL , matrizporescalar , ( void *) & p1);
	pthread_create (& h2 , NULL , matrizporescalar , ( void *) & p2);
	pthread_join ( h1, NULL ) ;
	pthread_join ( h2, NULL ) ;
	printf ( " Fin \n ");*/
	pthread_create (&hil[0] , &attr , matrizporescalar , ( void *) &hilos[0]);
	pthread_create (&hil[1] , &attr , matrizporescalar , ( void *) &hilos[1]);
	pthread_create (&hil[2] , &attr , matrizporescalar , ( void *) &hilos[2]);

	pthread_join(hil[0], NULL);
	pthread_join(hil[1], NULL);
	pthread_join(hil[2], NULL);

	//Imprimimos la matriz Matriz*escalar:
	int j;
	int k;

	for (j = 0; j < 3; j++)
	{
		for (k = 0; k < 3; k++)
		{
			printf("%d", matriz[j][k] );
		}
		
	}

	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloq);
	pthread_exit(NULL);

}