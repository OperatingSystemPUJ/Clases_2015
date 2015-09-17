/*
* This example uses a “classic” deadlock between two threads. 
* Each thread locks and then unlocks two mutexes. 
* The first thread acquires the mutexes in the order “mutex_1, mutex_2”, 
* while the second thread acquires them in the order “mutex_2, mutex_1”.
*/

#include <assert.h>
#include <pthread.h>
#include <stdio.h>

static void * simple_thread(void *);

pthread_mutex_t mutex_1= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2= PTHREAD_MUTEX_INITIALIZER;


int main()
{
    pthread_t tid = 0;
    int error = 0;

    // create a thread
    error = pthread_create(&tid, 0, &simple_thread, 0);
    assert(error == 0);

    error = pthread_mutex_lock(&mutex_1); // acquire mutex_1
    assert(error == 0);

    sleep(4);

    error = pthread_mutex_lock(&mutex_2); // acquire mutex_2
    assert(error == 0);

    sleep(5);


    error = pthread_mutex_unlock(&mutex_2); // release mutex_2
    assert(error == 0);

    error = pthread_mutex_unlock(&mutex_1); // release mutex_1
    assert(error == 0);

    error = pthread_join(tid, NULL);
    assert(error == 0);

    return 0;
}

static void * simple_thread(void * dummy)
{
    int error = pthread_mutex_lock(&mutex_2); // acquire mutex_2
    assert(error == 0);

    sleep(5);

    error = pthread_mutex_lock(&mutex_1); // acquire mutex_1
    assert(error == 0);

    error = pthread_mutex_unlock(&mutex_1); // release mutex_1
    assert(error == 0);

    sleep(3);

    error = pthread_mutex_unlock(&mutex_2); // release mutex_2
    assert(error == 0);

    return NULL;
}