#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSZ     128

/*
 * Declare the message structure.
 */

typedef struct msgbuf 
{
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;


main()
{
    int running = 1;
    int msqid;
    key_t key;
    message_buf  rbuf;

    /*
     * Get the message queue id for the
     * "name" 1234.
     */
    key = 1234;

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) 
    {
        perror("msgget");
        exit(1);
    }

    /*
     * Receive an answer of message type 1.
     */
    while(running)
    {
        int res = msgrcv(msqid, (void *)&rbuf, MSGSZ, 0, 0);
        if (res < 0) 
        {
            perror("msgrcv");
            exit(1);
        }

        printf("You wrote: %s\n", rbuf.mtext);
        if (strncmp(rbuf.mtext, "end", 3) == 0) 
        {
            running = 0;
        }
        else
            running = 1;       
    }    

    exit(0);
}
