#include <stdio.h>
#include <stdlib.h>

//Germán Camilo Narváez Libreros

int main(void) {
	FILE *in;
	extern FILE *popen();
	char buff[512];
	char instruccion[512]; 
	
	while(0==0){ 
	printf("German'sConsole$ ");
	fgets(instruccion,512,stdin);
	
	if(!(in = popen(instruccion, "r"))){
		exit(1);
	}


	while(fgets(buff, sizeof(buff), in)!=NULL){
		printf("%s", buff);
	}
	pclose(in);

   }

}