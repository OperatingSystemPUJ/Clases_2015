/*
 * execs.c - Examples of exec command
 *
 * The  exec()  family  of functions replaces the current process image with a new process image.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>

/*
 * exec0 - Simple execl wrapper example Call once, never return
 * 
 * Sintax: 
 * int execl(const char *path, const char *arg, ...);
 * 
 * The  const  char *arg and subsequent ellipses in the execl(), execlp(), and execle() functions can be thought
 * of as arg0, arg1, ..., argn.  Together they describe a list  of  one  or  more  pointers  to  null-terminated
 * strings  that  represent the argument list available to the executed program.
 *
 */
void exec0() 
{
    if (fork() == 0) 
    {
	   execl("/bin/ls", "ls", "-F", NULL);
	   printf("Child: Should never get here!\n");
	   exit(1);
    }
    wait(NULL);
    printf("Command completed\n");
}

/*
 * exec1 - Simple execve example
 *
 * execve() does not return on success, and the text, data, bss, and stack of 
 * the calling process are overwritten by that of the program loaded. 
 *
 * Sintax:
 * int execve(const char *filename, char *const argv[], char *const envp[]);
 * 
 * argv is an array of argument strings passed to the new program.  By convention, the first  of  these  strings
 * should  contain  the  filename associated with the file being executed.  envp is an array of strings, 
 * conventionally of the form key=value, which are passed as environment to the new program.
 *
 * An example of envp could be:
 * const char *envp[] = {"LD_LIBRARY_PATH=/usr/lib:/lib:/usr/local/lib:$LD_LIBRARY_PATH", NULL};
 * 
 */ 
#define MAXARGS 16
extern char **environ;
void exec1() 
{
    char *myargv[MAXARGS];
    
    myargv[0] = "/bin/ls";
    myargv[1] = "-F";
    myargv[2] = NULL;

    if (fork() == 0) 
    {
    	execve(myargv[0], myargv, environ);
    	printf("Child: Should never get here!\n");
    	exit(1);
    }
    wait(NULL);
    printf("Command completed\n");
}

int main(int argc, char *argv[])
{
    int option = 0;
    if (argc > 1)
	option = atoi(argv[1]);
    switch(option) 
    {
        case 0: 
            exec0();
    	   break;
        case 1: 
            exec1();
    	   break;
        default:
    	   printf("Unknown option %d\n", option);
    	   break;
    }
    return 0;
}
