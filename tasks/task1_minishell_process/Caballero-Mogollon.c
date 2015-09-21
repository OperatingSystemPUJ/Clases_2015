#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	FILE *in;
	extern FILE *popen();
	char buff[512];
	char *entrada;
	int n=512;
	char salir[4];
	entrada = (char *) malloc (n);
	printf("\n");
	while(1){
		printf( "Mini_shell@tarea:~$ " );
		getline(&entrada,&n,stdin );
		salir[0]=entrada[0];
		salir[1]=entrada[1];
		salir[2]=entrada[2];
		salir[3]=entrada[3];
		if (strcmp(salir,"quit")==1){
			printf("Saliendo de la Mini_shell\n");
			printf("\n");
			exit(0);
		}
		if(!(in = popen(entrada, "r"))){
			exit(1);
		}

		while(fgets(buff, sizeof(buff), in)!=NULL){
			printf("%s", buff);
		}
	}
	pclose(in);
}