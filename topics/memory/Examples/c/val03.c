// Reading/writing off the end of malloc’d blocks
// valgrind --tool=memcheck ./val03

#include <stdio.h>
#include <stdlib.h> 

int main(void)
{
    char *p = malloc(1);
    *p = 'a'; 

    char c = *(p+1); 

    printf("\n [%c]\n",c); 

    free(p);
    return 0;
}