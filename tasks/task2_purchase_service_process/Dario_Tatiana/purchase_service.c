#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <syslog.h>
#include <sys/types.h>



#define Port   2222
#define MAXBUF    1024

enum codes{
    OK,
    NOT_ACCEPTABLE,
    INTERNAL_SERVER_ERROR
};


static inline char *get_response_code(enum codes code){
    char *strings[] = { "300", "404", "707"};
    return strings[code];
}

    insert_request(int id_cliente, 
                   char *id_transac,
                   char *numtarjeta, 
                   int monto){
    sqlite3 *conn;
    sqlite3_stmt    *res;
    int error = 0;
    int rec_count = 0;
    const char *errMSG;
    const char *tail;
    sqlite3_stmt *stmt; 
    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    error = sqlite3_open("purch_serv.db", &conn);
    if (error){
        syslog (LOG_ERR, "[Validator Service]: Error can not open database...");
        return -1;
    }
    sqlite3_prepare_v2(conn, 
                       "insert into requests (id_cliente, id_transac, \
                                              numtarjeta, monto) \
                        values (?1, ?2, ?3, ?4);", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id_cliente);
    sqlite3_bind_text(stmt, 2, id_transac, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, numtarjeta, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, monto);
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
void validator_service(void){

    setlogmask (LOG_UPTO (LOG_INFO | LOG_ERR));
    syslog (LOG_INFO, "Starting Validator Service... pid:[%d]", getpid());
    enum codes code = OK;
    sqlite3 *conn;
    sqlite3_stmt    *res,*res1;
    char *sql_tmp;
    const char *tail;
    int error = 0;    
    int var_monto;
    int var_cash;
    int var_id_cliente;
    int var_id;
    FILE* in;
    extern FILE *popen();
    int rc = sqlite3_open("purch_serv.db", &conn);
    if (rc){
        syslog (LOG_ERR, "[Validator Service]: Error can not open database...");
        exit(0);
    }
    while(1){
        sqlite3_prepare_v2(conn,
            "select monto, id_cliente, id from requests where status=0", 1000, &res, NULL);        
           printf("Selecciona la tarjeta con sus numeros \n");
        if (sqlite3_step(res) == SQLITE_ROW){
            var_monto=sqlite3_column_int(res,0);
            var_id_cliente=sqlite3_column_int(res,1);
            var_id=sqlite3_column_int(res,2);
            printf("id_cliente: %d\n",var_id_cliente );
            sqlite3_prepare_v2(conn,"update requests SET status=1 WHERE id=?1",-1, &res, NULL);
            sqlite3_bind_int(res,1,var_id);
            sqlite3_step(res);
            sqlite3_prepare_v2(conn,
                "select cash from cards where id_cliente=?1", 1000, &res1, NULL);
            
            sqlite3_bind_int(res1,1,var_id_cliente);
            var_cash=sqlite3_column_int(res1,0);
            sqlite3_step(res1);
            if(var_cash>=var_monto){

                
                sqlite3_prepare_v2(conn,
                             "update cards SET cash=?1 WHERE id_cliente=?2",-1, &res1, NULL);
                int saldotarj=var_cash-var_monto;
                sqlite3_bind_int(res1,1,saldotarj);
                sqlite3_bind_int(res1,2,var_id_cliente);
                sqlite3_step(res1);
                
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
    printf("Starting Listener Service\n");
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
    self.sin_port = htons(Port);
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
        int id_cliente = atoi(parts[0]);
        char *id_transac = parts[1];
        char *numtarjeta = parts[2];
        int monto = atoi(parts[3]);
        if ( insert_request(id_cliente, 
                            id_transac, 
                            numtarjeta, 
                            monto) == 0 )
            code = OK; //Back response 300 (Received)
        else
            code = INTERNAL_SERVER_ERROR; //Back response 707 
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