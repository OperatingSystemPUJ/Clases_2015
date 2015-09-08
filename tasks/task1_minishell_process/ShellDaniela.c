#include <stdio.h>
#include <stdlib.h>

int shell() {
	FILE *in;
	extern FILE *popen();
	char buff[512];
 	char entrada[500];
 	printf("---");
 	fgets(entrada, 500, stdin);
 	//scanf("%s",&entrada);
 	//printf("Lo que usted escribio fue: %s\n", entrada);

	if(!(in = popen(entrada, "r"))){
		exit(1);
	}

	while(fgets(buff, sizeof(buff), in)!=NULL){
		printf("%s", buff);
	}
	pclose(in);

}


int main(void)
{

	while(1){
		shell();

	}
	return 0;
}