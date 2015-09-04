#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
//int execlp(const char *file, const char *arg, ...);
int main(){
        char command[256];
		char buff[512];      
		char salir[5];  
    	FILE *in;
		extern FILE *popen();			
	while(1){ // loop until return
		printf("Comando >"); fflush(stdout);	
		//it captures all the size	
		fgets(command, sizeof(command),stdin);						
		//this is for the exit case, if it's exit so the next conditional will be true
		salir[0]=command[0];
		salir[1]=command[1];
		salir[2]=command[2];
		salir[3]=command[3];		
		if(!strcmp(salir,"exit")){		
			return 0;
		} else {
			pid_t returnedValue = fork();
			if(returnedValue < 0){
				perror("error forking");
				return -1;
			} else if (returnedValue == 0){
				//Execution of the command
				//execlp(command, command, NULL);
				if(!(in=popen(command,"r"))){
					return -1;
				}
				while(fgets(buff, sizeof(buff), in)!=NULL){
					printf("%s", buff);
				}	
				return -1;
			} else {
				if(waitpid(returnedValue, 0, 0) < 0){
					perror("error waiting for child");
					return -1;
				}
			}
		}
	}
	pclose(in);
}	