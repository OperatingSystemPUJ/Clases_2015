#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

// no more than 254
#define NTHREADS 254

char* network;

int ping(char *ipaddr) {
    char *command = NULL;
    char buffer[1024];
    FILE *fp;
    int stat = 0;
    //asprintf (&command, "%s %s -q 2>&1", "fping", ipaddr);
    asprintf (&command, "%s %s -c 1", "ping", ipaddr);
    //printf("%s\n", command);

    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to execute fping command\n");
        free(command);
        return -1;
    }

    while(fread(buffer, sizeof(char), 1024, fp)) {
        if (strstr(buffer, "1 received"))
        {
            free(command);
            return 0;
        }
    }
    stat = pclose(fp);
    free(command);
    return 1;
}

/*  Check if an ip address is valid */
int isValidIpAddress(char *ipaddr) {
    struct sockaddr_in sa;
    char* test = NULL;
    asprintf(&test, "%s.%d", ipaddr, 1);
    int result = 0;
    result = inet_pton(AF_INET, test, &(sa.sin_addr));

    free(test);
    return result != 0;
}

void ping_helper(int sub_ip) {
    char* ipaddr = NULL;
    int status;
    asprintf(&ipaddr, "%s.%d", network, sub_ip);
    status = ping(ipaddr);
    if (status == 0) {
        printf("Could ping %s successfully, status %d\n", ipaddr, status);
    } else {
        printf("%s not reachable, status %d\n", ipaddr, status);
    }
    free(ipaddr);
}

void* ping_thread(void* tid) {
    int i, step, first, last;
    step = 254 / NTHREADS + 1; // 
    if (254 % NTHREADS == 0) {
        --step; // Compensate for NTHREADS = 1, 2, 127 and 254
    }
    first = *((int*) tid) * step + 1; // Counting varios ips per thread, x.x.x.0 doesnt count
    last = first + step;
    last = (last > 255) ? 255 : last; // the last ip can not be greater than 255
    /* i = [first, last) so at the end we check from x.x.x.1 to x.x.x.254 */
    for (i = first; i < last; ++i) {
        ping_helper(i);
    }
    pthread_exit(NULL);
}


int main(int argc, char **argv) {
    int status = 0;
    int i, start, tids[NTHREADS];
    pthread_t threads[NTHREADS];
    pthread_attr_t attr;

    if(argc != 2) {
        printf("Example Usage: %s 192.168.1\n", argv[0]);
        return 1;
    } else if(!isValidIpAddress(argv[1])) {
        printf("%s is an invalid Address, eg. 192.168.1 \n", argv[1]);
        return 1;
    }

    asprintf(&network, "%s", argv[1]);
    printf("Calculating...\n");
    /* Pthreads setup: explicitly create threads in a
    joinable state (for portability).  Pass each thread its loop offset */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i=0; i<NTHREADS; i++) {
        tids[i] = i;
        pthread_create(&threads[i], &attr, ping_thread, (void *) &tids[i]);
    }

    /* Wait for all threads to complete */ 
    for (i=0; i<NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Done.\n");

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    free(network);
    return status;
}