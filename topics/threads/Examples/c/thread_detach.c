#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
 
void *slave_entry(void *arg)
{
    int i;
    pthread_t self;
 
    self = pthread_self();
    pthread_detach(self);
 
    for (i = 0; i < 3; i++) {
        printf("slave thread: %d\n", i);
        sleep(1);
    }
    return NULL;
}
 
int main()
{
    int error;
    pthread_t slave_tid;
 
    pthread_create(&slave_tid, NULL, slave_entry, NULL);
    sleep(1);
 
    error = pthread_join(slave_tid, NULL);
    if (error != 0)
        printf("pthread_join failed!\n");
 
    printf("main thread exit\n");
    pthread_exit(NULL);
    return 0;
}