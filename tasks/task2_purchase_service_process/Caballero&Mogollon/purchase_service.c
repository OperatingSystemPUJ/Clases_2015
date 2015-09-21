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
#include <sqlite3.h>
#include "split2.c"

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
#define MY_PORT   8890
#define MAXBUF    1024

////////////////////////////////////////////////////////////////////////////////
// Enumerations
////////////////////////////////////////////////////////////////////////////////
enum codes 
{
    OK,
    NOT_ACCEPTABLE,
    INTERNAL_SERVER_ERROR
};

////////////////////////////////////////////////////////////////////////////////
// Functions (Helpers)
////////////////////////////////////////////////////////////////////////////////
static inline char *get_response_code(enum codes code)
{
    char *strings[] = { "202", "406", "500"};

    return strings[code];
}

int insert_request(int customer_id, 
                   char *transaction_id,
                   char *card_number, 
                   int amount)
{
    sqlite3 *conn;
    sqlite3_stmt    *res;
    int error = 0;
    int rec_count = 0;
    const char *errMSG;
    const char *tail;
    sqlite3_stmt *stmt; 

    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));

    error = sqlite3_open("purchase_service.db", &conn);
    if (error) 
    {
        syslog (LOG_ERR, "[Validator Service]: Error can not open database...");
        return -1;
    }

    sqlite3_prepare_v2(conn, 
                       "insert into requests (customer_id, transaction_id, \
                                              card_number, amount) \
                        values (?1, ?2, ?3, ?4);", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, customer_id);
    sqlite3_bind_text(stmt, 2, transaction_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, card_number, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, amount);

    //printf("%s\n",stmt);
    int rc = sqlite3_step(stmt); 
    if (rc != SQLITE_DONE) 
    {
        syslog (LOG_ERR, 
                "[Validator Service]: Error inserting data %s\n...", 
                                                    sqlite3_errmsg(conn));
        printf("Error insert\n");
        return -1;
    }
    syslog (LOG_INFO, "[Validator Service] new request inserted");
    printf("inserto bien\n");

    sqlite3_finalize(stmt);
    sqlite3_close(conn);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// client mail
////////////////////////////////////////////////////////////////////////////////
void correo(int valor)
{
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(4040);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    if (valor==1)
        strcpy(buffer,"Transaccion exitosa");
    else
        strcpy(buffer,"Saldo insuficiente");
    
    send(clientSocket, buffer,strlen(buffer),0);  


    memset(buffer, '\0', 1024);
    /*---- Read the message from the server into the buffer ----*/
    recv(clientSocket, buffer, 1024, 0);

    /*---- Print the received message ----*/
    printf("Mensaje Enviado: \n");   

}


////////////////////////////////////////////////////////////////////////////////
// Validator Service
////////////////////////////////////////////////////////////////////////////////
void validator_service(void)
{
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Validator Service... pid:[%d]", getpid());

    sqlite3 *db;
    sqlite3_stmt *stmt;
    sqlite3_stmt *co; 
    char *err_msg = 0;
    int rc = sqlite3_open("purchase_service.db", &db);
    int var_temporal;
    int error = 0;
    int var_id;
    int var_customer_id;
    int var_amount;
    int var_saldo;
    int var_status;
    int saldo_temporal;
    FILE *in;
    extern FILE *popen();
    char buff[512];
    while(1){

        //////////////////////////////////////////
        // Se busca nuevas requests
        sqlite3_prepare_v2(db,
                "select id,customer_id,amount,status from requests where status=0",
                1000, &stmt, NULL);       
        // se actualiza el status de la nueva request
        if(sqlite3_step(stmt) == SQLITE_ROW){
            var_id=sqlite3_column_int(stmt, 0);
            var_customer_id=sqlite3_column_int(stmt, 1);
            var_amount=sqlite3_column_int(stmt, 2);
            var_status=sqlite3_column_int(stmt, 3);
            printf("status: %d\n",var_status );
            sqlite3_prepare_v2(db, 
                           "update requests \
                           SET status=1\
                           WHERE id =?1", -1, &stmt, NULL);
            sqlite3_bind_int(stmt, 1, var_id); 
            sqlite3_step(stmt);

            // se busca el saldo
            sqlite3_prepare_v2(db,
                    "select saldo from cuentas where customer_id=?1",
                    1000, &stmt, NULL);
            sqlite3_bind_int(stmt, 1, var_customer_id);
            sqlite3_step(stmt);
            var_saldo=sqlite3_column_int(stmt, 0);

            //se compara si puede realizar la transaccion
            saldo_temporal=var_saldo-var_amount;
            if(saldo_temporal>=0){
                sqlite3_prepare_v2(db, 
                           "update cuentas \
                           SET saldo=?1\
                           WHERE customer_id =?2", -1, &stmt, NULL);
                sqlite3_bind_int(stmt, 1, saldo_temporal);
                sqlite3_bind_int(stmt, 2, var_customer_id);  
                sqlite3_step(stmt);
                valor(1);
                if(!(in = popen("echo \"Transaccion exitosa\" | mailx -v -r \"pruebasisoper@gmail.com\" -s \"Banco de los Sistemas Operativos\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"pruebasisoper@gmail.com\" -S smtp-auth-password=\"sisoper123\" -S ssl-verify=ignore jfm1094@gmail.com", "r"))){
                    exit(1);
                }
            }
            else{
                valor(0);
                if(!(in = popen("echo \"Saldo insuficiente\" | mailx -v -r \"pruebasisoper@gmail.com\" -s \"Banco de los Sistemas Operativos\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"pruebasisoper@gmail.com\" -S smtp-auth-password=\"sisoper123\" -S ssl-verify=ignore jfm1094@gmail.com", "r"))){
                    exit(1);
                }
            }


            while(fgets(buff, sizeof(buff), in)!=NULL){
                printf("%s", buff);
            }
            sleep(10);
            
        }
    }
    /*
        Ciclo infinito haciendo selects a la base de datos, preguntanto el status, si el status es cero
        entonces busca eb otra tabla que debemos crear el saldo disponible, se hace el update restando lo que
        quiere y se ebnvia eñ correo.
        El código de envío esta en el message client, y va desde el inicio hasta el segundo else. 
    */
    pclose(in);
    sqlite3_close(db);
    syslog (LOG_INFO, "[Validator Service]: Started...");
}

////////////////////////////////////////////////////////////////////////////////
// Listener Service
////////////////////////////////////////////////////////////////////////////////
void listener_service(void)
{
    enum codes code = OK;
    int sockfd;
    struct sockaddr_in self;
    char buffer[MAXBUF];

    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Listener Service... pid:[%d]", getpid());

    // Create streaming socket
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        syslog (LOG_ERR, "[Listener Service]: Error in create Socket...");
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
        syslog (LOG_ERR, "[Listener Service]: Error in Socket-bind...");
        exit(errno);
    }

    // Make it a listening socket
    if ( listen(sockfd, 20) != 0 )
    {
        syslog (LOG_ERR, "[Listener Service]: Error in Socket-listen...");
        exit(errno);
    }

    syslog (LOG_INFO, "[Listener Service]: Started...");

    //Forever
    while (1)
    {  
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen=sizeof(client_addr);

        // accept a connection (creating a data pipe)
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        syslog (LOG_INFO, 
                "[Listener Service]: Socket %s:%d connected\n", 
                                            inet_ntoa(client_addr.sin_addr), 
                                            ntohs(client_addr.sin_port));

        recv(clientfd, buffer, MAXBUF, 0);
        syslog (LOG_INFO, "[Listener Service]: Data receive [%s]\n", buffer);

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


////////////////////////////////////////////////////////////////////////////////
// Main function (Service)
////////////////////////////////////////////////////////////////////////////////
int main(int Count, char *Strings[])
{
    enum codes code = OK;
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));

    if (fork() == 0) 
    {
        //Child
        validator_service();
    }
    else 
    {
        //Parent
        listener_service();
    }
    return 0;
}