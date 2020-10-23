#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#define MAX 80 
#define SA struct sockaddr
// Function for reading output in buffer
void func(int sockfd) 
{
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff));
		//Prompt user for command to send to server
		printf("Enter command: "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff));
		//Read back the buffer from server which will be output from the command we sent
		read(sockfd, buff, sizeof(buff));
		//Write the output into our own file "cout.txt"
		FILE *f2 = fopen("cout.txt","w");
		if(f2 == NULL)
		  {
		    printf("Error opening file\n");
		    exit(1);
		  }
		
		fprintf(f2,"From Server : %s", buff);
		fclose(f2);
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Could not connect to server."); 
			break; 
		} 
	} 
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 
	int PORT = 8080;
	char ipAddr[]="";
	// code to receive IP or Port as user input **NEEDS TO BE FIXED, I THINK ITS MESSING WITH THE BUFFER BEING SENT TO THE SREVER **
	//	printf("Enter server name or IP address: ");
	int n = 0;
	//while((ipAddr[n++] = getchar()) != '\n')
	// ;
	//printf("Enter port: ");
	//scanf("%d", &PORT);
	//printf(&PORT);
	if(PORT == 0)
	  {
	    printf("err");
	    exit(1);
	  }
	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
	close(sockfd); 
} 
