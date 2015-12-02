#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
int ping(char *ipaddr) {
  char *command = NULL;
  char buffer[1024];
  FILE *fp;
  int stat = 0;
  //asprintf (&command, "%s %s -q 2>&1", "fping", ipaddr);
  asprintf (&command, "%s %s -c 1", "ping", ipaddr);
  ////printf("%s\n", command);

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

/*  Check if an ip address is valid */
int isValidIpAddress(char *ipaddr) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
    return result != 0;
}

void *Ping(void *ipaddr){
  int status = 0;
  char *ip=ipaddr;
  status = ping(ip);
  if (status == 0) {
    printf("Could ping %s successfully, status %d\n", ip, status);
  } else {
    printf("Machine not reachable, status %d\n", status);
  }
  //pthread_exit(0);
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Example Usage: %s 192.168.250.1 to start a scan net in direcction\n", argv[0]);
    return 1;
  } else if(!isValidIpAddress(argv[1])) {
    printf("%s is an invalid IP Address\n", argv[1]);
    return 1;
  }
  char *dup = strdup(argv[1]);//Esto hace magia
  char *token = strtok(dup,".");//Esto separa magicamente el string por espacios
  int c=0;
  int dir_p;
  char *dir="";
  while( token != NULL)//Estos es necesario
  {
      if (c==0)
      {
        asprintf (&dir, "%s", token);
      }
      else if (c<3)
      {
        //sprintf(dir,"%s.%s",dir,token);
        asprintf (&dir, "%s.%s", dir, token);
      }
      else{
        dir_p=atoi(token);
      }
      c++;
      token = strtok(NULL,".");
  }
  printf("scan 253 pints in %s\n",dir);
  int i=0;
  for (i = dir_p; i < 255; ++i)
  {
    char * dir_t="";
    asprintf (&dir_t, "%s.%d", dir, i);
    //sprintf(dir_t,"%d",i);
    pthread_t tid; /* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */
    /* get the default attributes */
    pthread_attr_init(&attr);
    /* create the thread */
    pthread_create(&tid,&attr,Ping,dir_t);
  }
  return 0;
}