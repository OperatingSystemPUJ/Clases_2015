/*4. Ping with threads.
Write a multithread program that allow To determine in a local network which addresses 
are active or which computers are active, using the ping command (you can use the ping_test.c code). 
A solution without threads is highly inefficient, because the program will 
have to wait for every ping. */

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 40
char* ipAddress = "192.168.250.";
pthread_mutex_t bloqueo;
int estado=0;


int Comando_ping(char *Direccion) {
  char *comando = NULL;
  char buffer[1024];
  FILE *fp;
  int stat = 0;
  asprintf (&comando, "%s %s", "ping", Direccion);
  printf("%s\n", comando);

  fp = popen(comando, "r");
  if (fp == NULL) {
    fprintf(stderr, "Fallo\n");
    free(comando);
    return -1;
  }

  while(fread(buffer, sizeof(char), 1024, fp)) {
    if (strstr(buffer, "1 received"))
      return 0;
  }
  stat = pclose(fp);
  free(comando);
  return 1;
}


void *Trabajo_ping(void *tid)
{
	char * DireccionIP = ipAddress;
	pthread_mutex_lock (&bloqueo);
	
	printf("%s\n",DireccionIP );
	estado = Comando_ping(DireccionIP);
  	if (estado == 0) {
    	printf("Ping a %s satisfactoriamente", DireccionIP);
 	} else {
    	printf("No responde ");
  	}
  	pthread_mutex_unlock(&bloqueo);
}


int main(int argc, char *argv[]){
	int i, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&bloqueo, NULL);

	char * DireccionIP = ipAddress;
	
	printf("%s\n",DireccionIP );
	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, Trabajo_ping,(void *) &tids[i]);
	}

	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloqueo);
	pthread_exit(NULL);
}