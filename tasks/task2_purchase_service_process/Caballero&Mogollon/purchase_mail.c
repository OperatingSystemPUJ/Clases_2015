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


void main(void)
{
    enum codes code = OK;
    int sockfd;
    struct sockaddr_in self;
    char buffer[MAXBUF];

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
       if (size==4)
       {
            /*printf("SI ES DE 4\n");
            printf("%d\n", atoi(parts[0]));
            printf("%s\n", (parts[1]));
            printf("%s\n", (parts[2]));
            printf("%d\n", atoi(parts[3]));*/

                 //Insert into sqlite
            int res=insert_request(atoi(parts[0]), 
                                   parts[1], 
                                   parts[2], 
                                   atoi(parts[3]));
            if (res==0)
                code = OK; //Back response 202 (Received)
            else
                code = INTERNAL_SERVER_ERROR; //Back response 500 
                                              // (Internal server error)
       }
        else
            code=NOT_ACCEPTABLE; //Back response 406

        char *str_code = get_response_code(code);

        send(clientfd, str_code, strlen(str_code), 0);

        //Close data connection
        close(clientfd);   
      
    }
    // Clean up (should never get here!)
    close(sockfd);
}