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

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
#define MY_PORT   4040
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
//Estructura para enviar el mensaje
typedef struct msgbuf {
    long    mtype;
    char    mtext[MAXBUF];
} message_buf;
 ////Funcion split (separar el string)
int strsplit (const char *str, char *parts[], const char *delimiter) 
{
  char *pch;
  int i = 0;
  char *tmp = strdup(str);
  pch = strtok(tmp, delimiter);

  parts[i++] = strdup(pch);

  while (pch) 
  {
    pch = strtok(NULL, delimiter);
    if (NULL == pch) break;
    parts[i++] = strdup(pch);
  }

  free(tmp);
  free(pch);
  return i;
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
                       "insert into requests (customer_id, transaction_id, card_number, amount) values (?1, ?2, ?3, ?4);", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, customer_id);
    sqlite3_bind_text(stmt, 2, transaction_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, card_number, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, amount);

    int rc = sqlite3_step(stmt); 
    if (rc != SQLITE_DONE) 
    {
        syslog (LOG_ERR, 
                "[Validator Service]: Error inserting data %s\n...", sqlite3_errmsg(conn));
        return -1;
    }
    syslog (LOG_INFO, "[Validator Service] new request inserted");

    sqlite3_finalize(stmt);
    sqlite3_close(conn);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Validator Service
////////////////////////////////////////////////////////////////////////////////
void validator_service(void)
{
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Validator Service... pid:[%d]", getpid());
    //Se llama al codigo OK!
    enum codes code= OK;
    // <<<Here your code>>>
    sqlite3 *base_de_datos;
    sqlite3_stmt *resultado1;
    sqlite3_stmt *resultado2;
    int error=0;
    int columna_amount;
    int columna_saldo;
    int columna_customer_id;
    int columna_id;
    int columna_status;
    int archivo_db = sqlite3_open("purchase_service.db", &base_de_datos);
    if (archivo_db){
        syslog (LOG_ERR, "[Validator Service]: Error can not open database...");
        exit(0);
    }

    while (1){

        //Consulta base de datos
        sqlite3_prepare_v2(base_de_datos, "select id,customer_id,amount,status from requests where status=0", 1000, &resultado1, NULL); 

        
        //Actualizacion status
        if (sqlite3_step(resultado1) == SQLITE_ROW){
            
            columna_id=sqlite3_column_int(resultado1,0);
            columna_customer_id=sqlite3_column_int(resultado1,1);
            columna_amount=sqlite3_column_int(resultado1,2);
            columna_status=sqlite3_column_int(resultado1,3);
            printf("customer_id: %d\n",columna_customer_id );
            // Actualizar status a ok
            sqlite3_prepare_v2(base_de_datos,"update requests SET status=1 WHERE id=?1",-1, &resultado1, NULL);
            sqlite3_bind_int(resultado1,1,columna_id);
            sqlite3_step(resultado1);
            sqlite3_prepare_v2(base_de_datos, "select saldo from cuentas where customer_id=?1", 1000, &resultado2, NULL);
            //bind vars
            sqlite3_bind_int(resultado2,1,columna_customer_id);
            columna_saldo=sqlite3_column_int(resultado2,0);
            sqlite3_step(resultado2);
            //If the cash in the person's account is greater than the amount then...
            int tmp;
            tmp=columna_saldo-columna_amount;
            if (tmp>=0){
                sqlite3_prepare_v2(base_de_datos, "update cuentas SET saldo=?1 WHERE customer_id =?2", -1, &resultado2, NULL);
                sqlite3_bind_int(resultado2, 1, tmp);
                sqlite3_bind_int(resultado2, 2, columna_customer_id);  
                sqlite3_step(resultado2);
            }
            else{
                code=NOT_ACCEPTABLE;
            }


            




    }

    
    sqlite3_close(base_de_datos);
    syslog (LOG_INFO, "[Validator Service]: Started...");


}
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
                "[Listener Service]: Socket %s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        recv(clientfd, buffer, MAXBUF, 0);
        syslog (LOG_INFO, "[Listener Service]: Data receive [%s]\n", buffer);
        char **partes= calloc(10,sizeof(char *));
        size_t tamano= strsplit(buffer,partes,"|"); //Divide hasta un |
        //Validate message structure
        //Insert into sqlite
        int customer_id = atoi(partes[0]);
        char *transaction_id = partes[1];
        char *card_number = partes[2];
        int amount = atoi(partes[3]);
        if ( insert_request(customer_id, 
                            transaction_id, 
                            card_number, 
                            amount) == 0 )
            code = OK; //Back response 202 (Received)
        else
            code = INTERNAL_SERVER_ERROR; //Back response 500 
                                          // (Internal server error)

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
int main(int argc, char *argv[])
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