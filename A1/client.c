
//---------------------SAURAV KORANGA------------20CS60R65-----------A1 - SOCKET PROGRAMMING-------------------


#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) 
{ 
	int PORT = atoi(argv[1]);

	if(PORT <= 1024 || PORT >= 65535){
			printf("Please enter Port number between 1024 and 65535!!!\n");
			exit(1);
	}

	int sock = 0, valread;
	char write_eval[10000], string[100];
	struct sockaddr_in serv_addr; 
	//char *hello = "Hello from client";
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\nSorry!!! Error while creating SOCKET!\n"); 
		return -1; 
	} 

	printf("Socket created Succesfully!!!\n");

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{
		printf("\nSorry!!! Address is not supported or may be Invalid!\n");
		return -1; 
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nCould not establish connection!!!\n");
		return -1; 
	}

	printf("Connection Establishment Succesfull!!!\n");

	//..................................this loop will run till close is entered by the client.....................

	while(1){

		printf("\nCLIENT: Enter input or type <close> to close connection!!\n");
		fgets(write_eval, sizeof write_eval, stdin);
		sscanf(write_eval, "%s", string);

		//...............closing connection from client side..................

		if(strcmp(string,"close") == 0){
			send(sock , write_eval, strlen(write_eval), 0);
			printf("\nCLIENT: Connection closed!\n");
			valread = read( sock, buffer, 1024);
			printf("%s\n",buffer);
			break;
		}

		//.........................otherwise......................

		else{
			send(sock , write_eval, strlen(write_eval), 0);
			printf("CLIENT: %s message sent!\n", string);
			valread = read( sock, buffer, 1024);
			printf("%s\n",buffer);
		}
		memset(buffer, 0, sizeof(buffer)); //to erase old content
	}

	return 0; 
} 
