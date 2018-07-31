#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h> 
#include <time.h> 
#include <unistd.h>

#define BUFFER_SIZE 1024
#define X 1000

/* we chose X to be a number such that in the X interation the avarege time will change in last than 1%
 * so we observed that this is the case for X that is about 1000
 * 
 * */
 
int main (int argc, char**argv){
    
    struct addrinfo hints;
    hints.ai_family = AF_INET;          /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP stream sockets */
    struct addrinfo* res = NULL;
    int status,sockfd;
    
    if((status = getaddrinfo(argv[1], "4444", &hints, &res))<0){
        printf("%d\n", status);
        perror("Failed In Address");
        return -1;
    }

    /* creating socket */
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
        perror("Failed In Socket Creation");
        return -1;
    }

    /* connecting socket to the server */
    if((status = connect(sockfd, res->ai_addr, res->ai_addrlen))!=0){
        perror("Failed In The Connection");
        return -1;
    }


    /* creating buffer in size 1024
     each time send X=1000 messages that are 1 byte (from the buffer)
               calculate throughput after message NUM1000 has been recived
               print 
                
               send X=1000 messages that are 2 byte (from the buffer)
               calculate throughput after message NUM1000 has been recived
               print
               
               send X=1000 messages that are 4 byte (from the buffer)
               calculate throughput after message NUM1000 has been recived
               print
               
               ..
               ..
               
               send X=1000 messages that are 1024 byte (=1KB)
               calculate throughput after message NUM1000 has been recived
               print
    */


    char * buf = (char*)malloc(BUFFER_SIZE+1);
    buf[BUFFER_SIZE]='\0';
    memset(buf, 'a', BUFFER_SIZE);
    
    int end;
    int msg_size ,j,i;
    double throughput;
    clock_t begin;
    
    for (msg_size=1; msg_size<=BUFFER_SIZE; msg_size=msg_size<<1){
        
        /*sending 50 msgs for warmingup
         * we tested the code for diffrent amout of warmups from 10 to 100 and saw that the results were the most stable for 50 warmup cycles  
         */
        for(i=0; i<50; i++){
            if ((send(sockfd, buf, msg_size, 0))<0){
                perror("Client Failed In Send");
                return -1;
            }
        }
        for(j=0; j<X; j++){
            begin = clock();
            
            if ((send(sockfd, buf, msg_size, 0))<0){
                perror("Client Failed In Send");
                return -1;
            }
              
        }     

        /* the server replies after all X have arrived */   
         recv(sockfd, &end, sizeof(end), 0);
        
        
        /* client calculates the throughput - total size of bytes that sent, based on the time it all took */
        
        throughput = (msg_size*X) /   ((double)(clock() - begin)) ;
        throughput = throughput / (double)(CLOCKS_PER_SEC);
        printf("%d\t%lf\tsec\n", msg_size, throughput);        
    }
    close(sockfd);
    return 0;
}

