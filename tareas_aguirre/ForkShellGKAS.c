

/* Giann Karlo Aguirre Samboní */
/* gcc -o ForkShellGKAS ForkShellGKAS.c */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char cmd[1024];
    char *args[64];
    int pid, state;

    while (1) 
    {    
        printf("gkas> ");

        if (gets(cmd) == NULL) {
            printf("\n");
            exit(0);
        }

        /*
         * Split the string into arguments.
         */
        splitarg(cmd, args);

        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            execvp(*args, args);
            perror(*args);
            exit(1);
        }


        while (wait(&state) != pid);
    }
}

/*
 * splitarg--split the command in cmd into
 *         individual arguments.
 */
splitarg(char* cmd, char** args)

{
    while (*cmd != NULL) 
    {
        /*
         * Strip whitespace.  Use nulls, so
         * that the previous argument is terminated
         * automatically.
         */
        while ((*cmd == ' ') || (*cmd == '\t'))
            *cmd++ = NULL;

        /*
         * Save the argument.
         */
        *args++ = cmd;
        //printf("%s\n", cmd);

        /*
         * Skip over the argument.
         */
        while ((*cmd != NULL) && (*cmd != ' ') && (*cmd != '\t'))
            cmd++;
    }

    *args = NULL;
}
