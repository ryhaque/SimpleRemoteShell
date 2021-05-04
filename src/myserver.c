/*  This is the server side of the code.
    The server receives client commands, processes it by popen() and reading it.
    The server than responds accordingly.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define BUFFERSIZE 4096
#define REQSIZE 1024
#define CONT_SZ 20

/*This is a global structure used for sending the data to client i.e. server response */
struct server_response{
	char data[BUFFERSIZE];
    //char contlen[CONT_SZ];
};

/*This function processes the command by running popen() and then trasnfers the data read
  to the client by writing to the client socket descriptor. */
void process_response(int comm_fd, char* com, struct server_response* srvresponse){
    
    FILE* pf;
    if((pf = popen(com,"r"))==NULL){
        warn("%s",com);
        return;
    }

    int chars_read = 1;
    
    while(chars_read>0){
        
        memset(srvresponse->data, '\0', sizeof(srvresponse->data));
        // Setup our pipe for reading and execute our command.
        chars_read = fread(srvresponse->data, sizeof(char),BUFFERSIZE-1,pf);

        //divisible size/buff or remaining bytes aka buffer isn't filled
        if(chars_read == 0 || chars_read < BUFFERSIZE-1){ //remaining bytes
            //append terminating character to indicate end of data trasnfer
            strcat(srvresponse->data, "+");
            write(comm_fd, srvresponse->data, strlen(srvresponse->data));
            //write(1, srvresponse->data, strlen(srvresponse->data));
        }else{
            //send data
            strcat(srvresponse->data, "-");
            write(comm_fd, srvresponse->data, strlen(srvresponse->data));
            //write(1, srvresponse->data, strlen(srvresponse->data));
        }    
        
    }
    
    if ( pclose(pf) != 0){
        //warn("%s",com);
        fprintf(stderr,"Error: Failed to close command stream\n"); 
        return;
    }
        
    return;
    
}
 
int main(int argc,char **argv){
    int n;
    char str[REQSIZE];
    int listen_fd, comm_fd;
    int port = 22000; //default
    struct sockaddr_in servaddr;
    
    //parse through CLA
    if(argc!=2){
        perror("Invalid arguments"); 
		exit(EXIT_FAILURE); 
    }
    if(atoi(argv[1]) !=0){
		port = atoi(argv[1]);
	}
    //printf("port:%d\n",port);
    // Creating socket file descriptor 
	if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	}
    //configure the socket
    bzero( &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    //bind the address
    if( (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0){
        err(1,"bind()");
        EXIT_FAILURE;
    }
    //listen for incoming connection
    if( (listen(listen_fd, 10)) < 0){
        err(1,"listen()");
        EXIT_FAILURE;
    }
    struct server_response srvresponse;

    while (1){
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
        //printf("after accept\n");
    
        while(1){
            bzero(str, REQSIZE);
            //reading client request
            n = read(comm_fd,str,REQSIZE);
            if (n <= 0) {
                close(comm_fd);
                break;
            }
           
            /*Process the client request and respond*/

            process_response(comm_fd,str, &srvresponse);
            
            
        }
    }
}

