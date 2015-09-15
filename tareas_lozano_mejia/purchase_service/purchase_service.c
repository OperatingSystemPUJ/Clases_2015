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
enum codes{
    OK,
    NOT_ACCEPTABLE,
    INTERNAL_SERVER_ERROR
};



////////////////////////////////////////////////////////////////////////////////
// Functions (Helpers)
////////////////////////////////////////////////////////////////////////////////
static inline char *get_response_code(enum codes code){
    char *strings[] = { "202", "406", "500"};
    return strings[code];
}



int insert_request(int customer_id, 
                   char *transaction_id,
                   char *card_number, 
                   int amount){
    sqlite3 *conn;
    sqlite3_stmt    *res;
    int error = 0;
    int rec_count = 0;
    const char *errMSG;
    const char *tail;
    sqlite3_stmt *stmt; 
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    error = sqlite3_open("purchase_service.db", &conn);
    if (error){
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
    int rc = sqlite3_step(stmt); 
    if (rc != SQLITE_DONE){
        syslog (LOG_ERR,
                "[Validator Service]: Error inserting data %s\n...", 
                                                    sqlite3_errmsg(conn));

        return -1;
    }
    syslog (LOG_INFO, "[Validator Service] new request inserted");
    sqlite3_finalize(stmt);
    sqlite3_close(conn);
    return 0;
}

///////////////////////////////////////////
/////////////Split/////////////////////////
int strsplit (const char *str, char *parts[], const char *delimiter){
  char *pch;
  int i = 0;
  char *tmp = strdup(str);
  pch = strtok(tmp, delimiter);
  parts[i++] = strdup(pch);
  while (pch){
    pch = strtok(NULL, delimiter);
    if (NULL == pch) break;
    parts[i++] = strdup(pch);
  }
  free(tmp);
  free(pch);
  return i;
}
////////////////////////////////////////////////////////////////////////////////
// Validator Service
////////////////////////////////////////////////////////////////////////////////
void validator_service(void){

    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Validator Service... pid:[%d]", getpid());
    enum codes code = OK;
    // <<<Here your code>>>
    sqlite3 *conn;
    sqlite3_stmt    *res,*res1;
    char *sql_tmp;
    const char *tail;
    int error = 0;    
    int var_card_number;
    int var_amount;
    int var_cash;
    int var_customer_id;
    FILE* in;
    extern FILE *popen();
    //Query to get all the unfinished transactions
    error = sqlite3_open("purchase_service.db", &conn);
    if (error){
        syslog (LOG_ERR, "[Validator Service]: Error can not open database...");
        exit(0);
    }
    while(1){
        sqlite3_prepare_v2(conn,
            "select card_number, amount, customer_id from requests where status=0", 1000, &res, NULL);        
        //Check all the unfinished transactions
        printf("HACE SELECCION DE DATOS DE LAS TARJETAS \n");
        if (sqlite3_step(res) == SQLITE_ROW){
            //get the information from the db of the cards
             //char** card_number=sqlite3_column_text(res, 0);
            //Vars  update
            var_card_number=sqlite3_column_int(res,0);
            var_amount=sqlite3_column_int(res,1);
            var_customer_id=sqlite3_column_int(res,2);
            printf("customer_id: %d\n",var_customer_id );
            // update the status to ok
            sqlite3_prepare_v2(conn,"update requests SET status=1 WHERE customer_id=?1",-1, &res, NULL);
            sqlite3_bind_int(res,1,var_customer_id);
            sqlite3_prepare_v2(conn,
                "select cash from cards where card_number=?1", 1000, &res1, &tail);
            //bind vars
            sqlite3_bind_int(res1,1,var_card_number);
            var_cash=sqlite3_column_int(res1,1);
            //If the cash in the person's account is greater than the amount then...
            printf("HAY FILAS \n");
            if(var_cash>=var_amount){

                //update the cash in the account
                ///int cash=sqlite3_column_int(res, 1);
                
                sqlite3_prepare_v2(conn,
                             "update cards SET cash=?1 WHERE card_number=?2",-1, &res1, NULL);
                int saldot=var_cash-var_amount;
                sqlite3_bind_int(res1,1,saldot);
                sqlite3_bind_int(res1,2,var_card_number);
                if(!(in = popen("echo \"Transaccion exitosa\" | mailx -v -r \"pruebasisoper@gmail.com\" -s \"Banco de los Sistemas Operativos\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"pruebasisoper@gmail.com\" -S smtp-auth-password=\"sisoper123\" -S ssl-verify=ignore sebaslh12@gmail.com", "r"))){
                        exit(1);
                    }
                else{
                    if(!(in = popen("echo \"Usted no posee saldo para realizar la compra\" | mailx -v -r \"pruebasisoper@gmail.com\" -s \"Banco de los Sistemas Operativos\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"pruebasisoper@gmail.com\" -S smtp-auth-password=\"sisoper123\" -S ssl-verify=ignore sebaslh12@gmail.com", "r"))){
                        exit(1);
                    }
                }

            }
            else{
                code=NOT_ACCEPTABLE;
            }            
        }
        sleep(10);
    }
    pclose(in);
    sqlite3_close(conn);
    syslog (LOG_INFO, "[Validator Service]: Started...");
}
////////////////////////////////////////////////////////////////////////////////
// Listener Service
////////////////////////////////////////////////////////////////////////////////
void listener_service(void){
    enum codes code = OK;
    int sockfd;
    struct sockaddr_in self;
    char buffer[MAXBUF];
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Listener Service... pid:[%d]", getpid());
    // Create streaming socket
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        syslog (LOG_ERR, "[Listener Service]: Error in create Socket...");
        exit(errno);
    }
    // Initialize address/port structure
    bzero(&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(MY_PORT);
    self.sin_addr.s_addr = INADDR_ANY;
    // Assign a port number to the socket
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 ){
        syslog (LOG_ERR, "[Listener Service]: Error in Socket-bind...");
        exit(errno);
    }
    // Make it a listening socket
    if ( listen(sockfd, 20) != 0 ){
        syslog (LOG_ERR, "[Listener Service]: Error in Socket-listen...");
        exit(errno);
    }

    syslog (LOG_INFO, "[Listener Service]: Started...");
    //Forever
    while (1){  
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
        char **parts = calloc(10, sizeof(char *));
        size_t size =strsplit(buffer,parts,"|");
        //Validate message structure
        //Insert into sqlite
        int customer_id = atoi(parts[0]);
        char *transaction_id = parts[1];
        char *card_number = parts[2];
        int amount = atoi(parts[3]);
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
int main(int Count, char *Strings[]){
    enum codes code = OK;
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    if (fork() == 0){
        //Child
        validator_service();
    }
    else{
        //Parent
        listener_service();
    }
    return 0; 
}