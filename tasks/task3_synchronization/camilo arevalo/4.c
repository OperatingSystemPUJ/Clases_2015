//4. Que esta ocurriendo en este codigo y como lo arreglaria?:
//Nota: para compilar agregar -lm
//////////////////////////////////////
//Respuesta
//este codigo tenia 2 problemas.
//1. el tipo de dato que el entraba a la funcion lo tomaba como void * por
//como tiene que entrar el parametro a la funcion del thread y habai que castearlo para
//que el programa lo reconociera
//2. al lanzar los trhead, el codigo principal que esta en el main
//segia la ejecucion y finalizaba el programa matando a todos los hilos sin haber terminado
//lo que estaban haciendo, por tanto se tubo que dejar que al final el programa 
//esperara a que los hilos terminara de hacer su trabajo.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	5

void *PrintHello(void *t)
{
   long threadid;
   threadid=(long *)t;
   int i;
   double myresult=0.0;
   //printf("thread=%ld: starting...\n", threadid);

   printf("thread=%ld: starting...\n", threadid);

   for (i=0; i<1000000; i++){
      myresult += sin(i) * tan(i);
    }
   //printf("thread=%ld result=%e. Done.\n",threadid,myresult);
    printf("miow miow\n");

    printf("thread=%ld result=%e. Done.\n",threadid,myresult);

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  for(t=0;t<NUM_THREADS;t++){
    printf("Main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *) t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
      }
    }
  int i;
  for (i = 0; i < NUM_THREADS; ++i)
  {
    pthread_join(threads[i], NULL);
  }
  printf("Main: Done.\n");
}
