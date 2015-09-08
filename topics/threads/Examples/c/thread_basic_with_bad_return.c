#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __myarg_t 
{
	int a;
	int b;
} myarg_t;

typedef struct __myret_t 
{
	int x;
	int y;
} myret_t;

void *mythread(void *arg) 
{
	myarg_t *m = (myarg_t *) arg;
	printf("%d %d\n", m->a, m->b);
	myret_t r; // ALLOCATED ON STACK: BAD!
	r.x = 1;
	r.y = 2;
	return (void *) &r;
}

/* 
Note:
In this case, the variable r is allocated on the stack of mythread. However, 
when it returns, the value is automatically deallocated (that’s why the stack is 
so easy to use, after all!), and thus, passing back a pointer to a now 
deallocated variable will lead to all sorts of bad results. Certainly, when you 
print out the values you think you returned, you’ll probably 
(but not necessarily!) be surprised.
*/

int main(int argc, char *argv[]) 
{
	int rc;
	pthread_t p;
	myret_t *m;

	myarg_t args;
	args.a = 10;
	args.b = 20;
	pthread_create(&p, NULL, mythread, &args);
	pthread_join(p, (void **) &m);
	printf("returned %d %d\n", m->x, m->y);
	return 0;
}
