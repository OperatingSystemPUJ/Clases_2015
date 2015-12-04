/*
Presentado por:
-Tatiana Peña Prias
-Darío Fernández Astudillo


4. Ping with threads.
Write a multithread program that allow To determine in a local network which addresses 
are active or which computers are active, using the ping command (you can use the ping_test.c code). 
A solution without threads is highly inefficient, because the program will 
have to wait for every ping. 


*/
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define NTHREADS 10
//Network IP
char ipaddr[]="192.168.1.57";
//Thread IP
int ipactual=1;
pthread_mutex_t p;

int ping(void) {
  char *command = NULL;
  char buffer[1024];
  FILE *fp;
  int stat = 0;
  asprintf (&command, "%s %s", "ping", ipaddr);
  printf("%s\n", command);

  fp = popen(command, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to execute fping command\n");
    free(command);
    return -1;
  }

  while(fread(buffer, sizeof(char), 1024, fp)) {
    if (strstr(buffer, "1 received"))
      return 0;
  }
  stat = pclose(fp);
  free(command);
  return 1;
}

void* pingtest(void* actual){
	int *ptr= (int*) actual;	
	pthread_mutex_lock (&p);
	//char str[1];
	//sprintf(str, "%d", ipactual);
	//ipaddr[12]=ipactual;
	ipactual=0;
	int status=ping();
	if (status == 0) {
    	printf("Could ping %s successfully, status %d\n", ipaddr, status);
  	} 
  	else {
    	printf("Machine not reachable, status %d\n", status);
  	}
  	pthread_mutex_unlock (&p);
  	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int i, start, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;

	/* Pthreads setup: initialize mutex and explicitly create threads in a
	 joinable state (for portability).  Pass each thread its loop offset */
	pthread_mutex_init(&p, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, pingtest, (void *) &ipactual);
	}

	/* Wait for all threads to complete then print global sum */ 
	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&p);
	pthread_exit (NULL);
}