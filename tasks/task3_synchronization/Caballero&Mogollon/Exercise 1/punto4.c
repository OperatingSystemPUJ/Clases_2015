#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 40
char* network = "192.168.250.";
pthread_mutex_t bloqueo;
int status=0;
int total=0;


int ping(char *ipaddr) {
  char *command = NULL;
  char buffer[1024];
  FILE *fp;
  int stat = 0;
  //asprintf (&command, "%s %s -q 2>&1", "fping", ipaddr);
  asprintf (&command, "%s %s -c 1", "ping", ipaddr);
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


void *do_ping(void *tid)
{
	char ip [25];
	pthread_mutex_lock (&bloqueo);
	total++;
	sprintf(ip, "192.168.250.%d",total);
	printf("%s\n",ip );
	status = ping(ip);
  	if (status == 0) {
    	printf("Could ping %s successfully, status %d\n", ip, status);
 	} else {
    	printf("Machine not reachable, status %d\n", status);
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
	printf("TODO BIEn\n");
	//char ip [25];
	//printf("TODO BIEn\n");
	//sprintf(ip, "192.168.250.%d",4);
	//printf("TODO BIEn\n");
	//printf("%s\n",ip );

	//char *suma = ip+"hola";

	for (i=0; i<NTHREADS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, do_ping,(void *) &tids[i]);
	}

	for (i=0; i<NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_attr_destroy (&attr);
	pthread_mutex_destroy (&bloqueo);
	pthread_exit(NULL);
}