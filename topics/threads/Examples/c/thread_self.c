#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
 
void *worker(void *arg)
{
    printf("In worker thread:\n");
    printf("\tworker = %p\n", (void *)pthread_self());
    return NULL;
}
 
int main()
{
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, worker, NULL);
 
    printf("In main thread:\n");
    printf("\t  main = %p\n", (void *)pthread_self());
    printf("\tworker = %p\n", (void *)worker_tid);
    pthread_join(worker_tid, NULL);
    return 0;
}