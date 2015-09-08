/*     #######             #####         
 *  #############       ###########      
 * ####       ####    ####       ####    
 *###           ###  ###           ###   
 * ####              ###           ###   
 *   #####           ###           ###   
 *     #####         ###           ###   
 *       #####       ###  ##       ###   
 *         #####     ###  ####     ###   
 *           #####   ###    ####   ###   
 *###           ###  ###      #### ###   
 * ####       ####    ####      #####    
 *  #############       ###############
 *     #######             #####    ####
 *     Creado por: Santiago Quintero     
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Alternative code if system comman is not an option.
char** split_string(char* string)
{

}
*/


int run_command(char* command)
{
    /* Used for fork() and wait() */
    pid_t pid, wait_pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        /* Fork failed */
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        /* Child */
        if (system(command) == -1)
            perror("SQshell");
        exit(EXIT_FAILURE);
    } else {
        /* Parent */
        do {
            wait_pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int main(int argc, char *argv[])
{
    /* Declarations */
    char* line;
    ssize_t buffer_size;
    int status;

    /* Main loop */
    do {
        line = NULL;
        buffer_size = 0;
        printf("SQ> ");
        /* User input */
        getline(&line, &buffer_size, stdin);
        /* Command excecution and status return */
        if (strcmp(line, "exit\n") == 0)
            status = 0;
        else
            status = run_command(line);
        /* Memory liberation */
        free(line);
    } while (status);

    return 0;
}
