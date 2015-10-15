#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *buf;

	buf = malloc(1<<31); //the value of the expression 1 << 31 (the integer 1 right-shifted 31 times) 
	                     //is 429497295, or 4GB (gigabytes). Very few machines have this kind of memory

	fgets(buf, 1024, stdin);
	printf("Result: %s\n", buf);

	return 1;
}
