#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 126
#define BUFFSIZE 4096 
#define SA struct sockaddr 

void setup(char inputBuffer[], char* args[], int* background) //Ham xu li (cat) chuoi lenh
{
    const char s[4] = " \t\n";
    char* token;
    token = strtok(inputBuffer, s);
    int i = 0;
    while (token != NULL)
    {
        args[i] = token;
        i++;
        //printf("%s\n", token);
        token = strtok(NULL, s);
    }
    args[i] = NULL;
}


// Function designed for command execution between client and server. 
void func(int sockfd)
{
    char buffer[BUFFSIZE];
    int received = -1;
    char data[MAX];
    memset(data, 0, MAX);

    while (1) {
        data[0] = '\0';

	// make sure we recieved actual text
        if ((received = recv(sockfd, buffer, BUFFSIZE, 0)) < 0) {

	  //fail
	  
	  exit(1);
        }
	

        buffer[received] = '\0';

        strcat(data, buffer);
        if (strcmp(data, "exit") == 0)
	  {
	    // printf("FAIL2");
	    // this will force the code to exit
            exit(0);
	  }

        puts(data);
        char* args[100];
        setup(data, args, 0);
        int pipefd[2], lenght;

        if (pipe(pipefd))
	   exit(1);
	//pipe process to call command from client

        pid_t pid = fork();
    
        char path[MAX];
        if (pid == 0)
        {
            close(1); // close the original stdout
            dup2(pipefd[1], 1); // duplicate pipfd[1] to stdout
            close(pipefd[0]); // close the readonly side of the pipe
            close(pipefd[1]); // close the original write side of the pipe
            execvp(args[0], args); // finally execute the command
        }
        else
            if (pid > 0)
            {
                close(pipefd[1]);
                memset(path, 0, MAX);
                while (lenght = read(pipefd[0], path, MAX - 1)) {
		  
		 
                    fflush(NULL);
		    //Write the output to file
		    FILE *f = fopen("out.txt","w");
		    if(f == NULL)
		      {
		        printf("can't open file!");
			exit(1);
		      }
	 
		    fclose(f);
                    memset(path, 0, MAX);
                }
                close(pipefd[0]);
            
            }
            else
            {
                printf("Error !\n");
                exit(0);//
            }
    }
    
}

// Driver function
// Code referenced from geeksforgeeks.org
int main(int argc, char *argv[])
{
  //Take in port from command line
  int PORT = 0;
  if(argc == 2)
    {
      PORT = atoi(argv[1]);
    }
  else
    {
      printf("Error, invalid port");
      exit(1);
    }
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    
    // socket create and verification 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding new socket 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server sits and listens
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for runnning command recieved from client 
    func(connfd);
    char ret[MAX];
    // Read output from command from the file we wrote to in the function
    FILE *f3;
    f3=fopen("out.txt","rt");
    int x=0;
    while(ret[x++]=fgetc(f3) != EOF)
      ;
      
    fclose(f3);
    //  put output from file to the buffer for client to read
    if(send(connfd,ret,strlen(ret),0) != strlen(ret) ){
      exit(0);
    }
    // After chatting close the socket 
    close(sockfd);
}
