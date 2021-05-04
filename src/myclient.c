/*  This is the client side of the program.
    The client readline() from stdin, and then generates a unix command to send to server
    The client that receives the server response accordingly and displays it to stdout.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <err.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFERSIZE 4096
#define REQSIZE 1024
#define CONT_SZ 20

int main(int argc,char **argv){
    int sockfd;
    //char sendline[100];
    char* sendline;
    char command[REQSIZE];
    char recvline[BUFFERSIZE];
    struct sockaddr_in servaddr;
    int port = 22000; //default
	char* ip;
    //char ip[50];

    //Parse through CLA
    if(argc!=3){
        perror("Invalid arguments"); 
		exit(EXIT_FAILURE); 
    }
	ip = argv[1];
	if(atoi(argv[2]) !=0){
		port = atoi(argv[2]);
	}
	//printf("ip:%s\nport:%d\n",ip, port);

    // Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	}
    //configure the socket
    bzero(&servaddr,sizeof servaddr);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);

    if((inet_pton(AF_INET,ip,&(servaddr.sin_addr)))<=0){
        err(1,"inet_pton()");
        return EXIT_FAILURE;
    }

    //connect
    if( ( connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) ) < 0){
        err(1,"connect");
        return EXIT_FAILURE;
    }
 
    while(1){
        
        bzero(recvline, BUFFERSIZE);
        bzero(command, REQSIZE);
        
        /*readline from terminal*/
        sendline = readline("client $ ");
        //printf("sendline: %s\n",sendline);
        
        if(strcmp(sendline, "exit") == 0) {
            close(sockfd);
            break;
        }
        /*Generate the Unix command response to send to server*/
        if((sprintf(command, "%s 2>&1", sendline))<0){
            err(1,"error in command");
            exit(EXIT_FAILURE);
        }
        //sprintf(command, "%s 2>&1", sendline);
        free(sendline);
        /*Send the command*/
        write(sockfd,command,strlen(command));
        /*Recieve the server response and display to Client stdout*/
        int sz = BUFFERSIZE;
        while(sz == BUFFERSIZE){

            memset(recvline, '\0', sizeof(recvline));
            sz = read(sockfd,recvline,BUFFERSIZE);
            /*
            Note the + indicated the terminating character
            */
            if( strstr(&recvline[sz-1], "+") != NULL){
                if(sz>1){
                    //write(1,"hey\n", strlen("hey\n"));
                    write(1,recvline,sz-2);
                }else{
                    write(1,recvline,sz-1);
                }
                //write(1,"{+}\n",strlen("{+}\n"));
                //break;
                
            }else{
                //write(1,"{-}\n",strlen("{-}\n"));
                write(1,recvline,sz-1);
            }
            

        }
    
    }
 
}
