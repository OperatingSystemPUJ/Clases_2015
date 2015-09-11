#include <stdio.h>
#include <stdlib.h>

int main(void) {
	FILE *in;
	extern FILE *popen();
	char buff[512];
	printf("\n");
	printf("**************************** Bienvenido a la consola ****************************\n");
	printf("\n");
	while(1){

		char algo[500];
		printf(">> ");
		fgets(algo,500,stdin);
		if(!(in = popen(algo, "r"))){
			exit(1);
		}

		while(fgets(buff, sizeof(buff), in)!=NULL){
			printf("   %s", buff);
		}
		pclose(in);
	}
}