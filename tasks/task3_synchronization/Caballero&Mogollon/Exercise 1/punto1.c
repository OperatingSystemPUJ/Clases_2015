#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//  Program 1


void *thread(void *vargp)
{
	pthread_exit((void*)42);
}
int main()
{
	int i;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, (void **)&i);
	printf("%d\n",i);
}


/*

//Program 3
void *thread(void *vargp)
{
	int *ptr = (int*)vargp;
	pthread_exit((void*)*ptr);
}
void *thread2(void *vargp)
{
	int *ptr = (int*)vargp;
	*ptr = 0;
	pthread_exit((void*)31);
}
int main()
{
	int i = 42;
	pthread_t tid, tid2;
	pthread_create(&tid, NULL, thread, (void*)&i);
	pthread_create(&tid2, NULL, thread2, (void*)&i);
	pthread_join(tid, (void**)&i);
	pthread_join(tid2, NULL);
	printf("%d\n",i);
}



// Program 4 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *thread(void *vargp)
{
	pthread_detach(pthread_self());
	pthread_exit((void*)42);
}
int main()
{
	int i = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, (void*)&i);
	pthread_join(tid, (void**)&i);
	printf("%d\n",i);
}

*/