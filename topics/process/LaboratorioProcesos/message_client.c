#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
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
    int msqid;
    //int msgflg = IPC_CREAT | 0666;
    int msgflg = 0666;
    key_t key;
    message_buf sbuf;
    size_t buf_length;
    struct msqid_ds info;

    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = 1234;

    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }


    /*
     * We'll send message type 1
     */
     
    sbuf.mtype = 1;
    
    (void) strcpy(sbuf.mtext, "Did you get this?");
    
    buf_length = strlen(sbuf.mtext) + 1 ;

    /*
     * Send a message.
     */
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) 
    {
        printf ("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
    else
        printf("Message sent! [%s]\n", sbuf.mtext);

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) 
    {
        printf ("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
    else
        printf("Message sent! [%s]\n", sbuf.mtext);    

    (void) strcpy(sbuf.mtext, "eni");
    buf_length = strlen(sbuf.mtext) + 1 ;
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) 
    {
        printf ("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }
    else
        printf("Message sent! [%s]\n", sbuf.mtext);

    if (msgctl(msqid, IPC_STAT, &info))
        perror("msgctl error ");
      
    exit(0);
}