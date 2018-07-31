#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h> 
#include <unistd.h>

#define PORT 4444
#define BUFFER_SIZE 1024
#define X 1000

int main (int argc, char**argv){
    
    int client_socket;
    struct sockaddr_in address;
    int address_length = sizeof(struct sockaddr_in);
    int sockfd,status;

     /* creating socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Failed In Socket Creation");
        return -1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    /*Bind socket to the local address and port number*/
    if ((status = bind(sockfd, (struct sockaddr *)&address, sizeof(address))) < 0){
        perror("Failed in Binding");
        return -1;
    }

    /*Define how many connections can be pending*/
    if ((status = listen(sockfd, 3)) < 0){
        perror("Faild in Listen");
        close(sockfd);
        return -1;
    }
    
    /*Accept a new connection from a client*/
    client_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&address_length);
    

    /* need to recieve 1000*1024 messages (1000 messages from each size 1byte-1024byte)
     * BUT send only after each 1000 messages */

    char buffer[BUFFER_SIZE + 1];
    memset(buffer, 0, BUFFER_SIZE+1);
    

    int i,j;
    for (i=1; i<=BUFFER_SIZE; i=i<<1){
        for(j=0; j<X+50; j++){
            
            if ((recv(client_socket, buffer, i, 0))<0){
                perror("Error in recieve");
                close(sockfd);
                return -1;
            }

          
        }
        
        /* the server replies after all X have arrived */
        send(client_socket, &i, sizeof(i), 0);
    }
   close(sockfd);
   return 0; 
}

