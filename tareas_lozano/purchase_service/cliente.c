#include <stdio.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>

#include <stdlib.h>



int main(){

    int clientSocket, port;

    char buffer[1024];

    struct sockaddr_in serverAddr;

    socklen_t addr_size;

    port=4040;

    //---- Create the socket. The three arguments are: ----*/

    // 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket<0){

        printf("Error abriendo el socket...\n");

        exit(1);



    }



    //---- Configure settings of the server address struct ----*/

    // Address family = Internet */

    //serverAddr.sin_family = AF_INET;

    // Set port number, using htons function to use proper byte order */

    serverAddr.sin_port = htons(port);

    // Set IP address to localhost */

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Set all bits of the padding field to 0 */

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  



    // Connect the socket to the server using the address struct ----*/

    addr_size = sizeof serverAddr;

    

    if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0){

        printf("Error no se encontro el servidor...\n");

        exit(1);

    }



    //strcpy(buffer,"20|QWERTY1234|9876789545A|350000\n");

    while(1){

        printf("Ingrese sus datos:\n");

        printf("Recuerde que el formato es CustomerID|TransactionID|CardNumber|Value \n");

        //All the buffer's bits set to zero

        memset(buffer, '\0', 1024);

        fgets(buffer,1023,stdin);

        

        if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0){

            printf("Error no se encontro el servidor...\n");

            exit(1);        

        } 

        //Send to server the msg

        send(clientSocket, buffer,strlen(buffer),0); 

        //Read the message from the server into the buffer

        recv(clientSocket, buffer, 1024, 0);

        // Print the received message ----*/

        if(buffer=="202"){

            printf("Datos Recibidos!\n");   

        }

        else{

            printf("Los datos no cumplian los requisitos");

        }

        return 0;

    }     

}