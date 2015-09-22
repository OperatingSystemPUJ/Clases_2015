#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include "split2.c"

#define MY_PORT   8891
#define MAXBUF    1024

enum codes 
{
    OK,
    NOT_ACCEPTABLE,
    INTERNAL_SERVER_ERROR
};

static inline char *get_response_code(enum codes code)
{
    char *strings[] = { "202", "406", "500"};

    return strings[code];
}



void main(void)
{
    enum codes code = OK;
    int sockfd;
    struct sockaddr_in self;
    char buffer[MAXBUF];
    FILE *in;
    extern FILE *popen();
    char buff[2048];

    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Listener Mail Service... pid:[%d]", getpid());

    // Create streaming socket
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        syslog (LOG_ERR, "[Listener Mail Service]: Error in create Socket...");
        exit(errno);
    }

    // Initialize address/port structure
    bzero(&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(MY_PORT);
    self.sin_addr.s_addr = INADDR_ANY;

    // Assign a port number to the socket
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
    {
        syslog (LOG_ERR, "[Listener Mail Service]: Error in Socket-bind...");
        exit(errno);
    }

    // Make it a listening socket
    if ( listen(sockfd, 20) != 0 )
    {
        syslog (LOG_ERR, "[Listener Mail Service]: Error in Socket-listen...");
        exit(errno);
    }

    syslog (LOG_INFO, "[Listener Mail Service]: Started...");

    //Forever
    while (1)
    {  
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen=sizeof(client_addr);

        // accept a connection (creating a data pipe)
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        syslog (LOG_INFO, 
                "[Listener Mail Service]: Socket %s:%d connected\n", 
                                            inet_ntoa(client_addr.sin_addr), 
                                            ntohs(client_addr.sin_port));

        recv(clientfd, buffer, MAXBUF, 0);
        syslog (LOG_INFO, "[Listener Mail Service]: Data receive [%s]\n", buffer);

        //Validate message structure
       //printf("ESTO ES LO QUE LEE\n%s\n",buffer );
        char **parts = calloc(4, sizeof(char *));
        size_t size = strsplit(buffer, parts, "|");
        sprintf(buff,"echo \" %s \" | mailx -v -r \"pruebasisoper@gmail.com\" -s \"Banco de los Sistemas Operativos\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"pruebasisoper@gmail.com\" -S smtp-auth-password=\"sisoper123\" -S ssl-verify=ignore jfm1094@gmail.com",parts[0]);
        if(!(in = popen(buff, "r"))){
                    exit(1);
                }

        char *str_code = get_response_code(code);

        send(clientfd, str_code, strlen(str_code), 0);

        //Close data connection
        close(clientfd);
        sleep(10); 
         
      
    }
    // Clean up (should never get here!)
    close(sockfd);
    pclose(in);
}

