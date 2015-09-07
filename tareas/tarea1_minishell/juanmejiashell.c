#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  printf("----------------------------------------------------------------\n");
  FILE *in;
  char salir[5];
	extern FILE *popen();
  char command[256];
	char buff[1024];

  while(1){
    printf("mini_shell> ");
    fgets(command, sizeof(command), stdin);
    salir[0]=command[0];
		salir[1]=command[1];
		salir[2]=command[2];
		salir[3]=command[3];

    if(!strcmp(salir,"exit")){
      exit(0);
    }
    else {
      in = popen(command,"r");

    	while(fgets(buff, sizeof(buff), in)!=NULL){
    		printf("%s", buff);
    	}
    	pclose(in);
    }
  }
}
