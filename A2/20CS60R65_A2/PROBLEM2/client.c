//---------------------SAURAV KORANGA------------20CS60R65-----------A2 - SOCKET PROGRAMMING-------------------

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char const *argv[]) 
{ 
	int PORT = atoi(argv[2]);

	if(PORT <= 1024 || PORT >= 65535){
			printf("Please enter Port number between 1024 and 65535!!!\n");
			exit(1);
	}

	int sock,client_id;
	struct hostent *server;
	char buffer[1000] = {0}, string[100];
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\nSorry!!! Error while creating SOCKET!\n"); 
		return -1; 
	}

	printf("Socket = %d created Succesfully!!!\n", sock);


	memset(&serv_addr, 0, sizeof(serv_addr));

	server=gethostbyname((char*)argv[1]);
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; 
	bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(PORT);

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nCould not establish connection!!!\n");
		return -1;
	}
	printf("Connection Establishment Succesfull!!!\n");

	char message[1000] = {0};

	//..................................this loop will run till close is entered by the client.....................

	while(1){

		printf("\nCLIENT: Enter the message you want to send to server or type <close> to close connection!!\n");
		fgets(buffer, sizeof buffer, stdin);
		sscanf(buffer, "%s", string);

		//...............closing connection from client side..................

		if(strcmp(string,"close") == 0){
			send(sock , buffer, strlen(buffer), 0);
			close(sock);
			printf("\nCLIENT: Connection closed!\n");
			break;
		}

		//.........................otherwise......................

		else{

			send(sock , buffer, strlen(buffer), 0);
			printf("CLIENT: %s message sent!\n\n", buffer);
			
			if(recv(sock, message, 1000, 0) < 0)
				printf("Error while receiving!!!\n");
			else
				printf("Server: %s\n",message);
		}
		memset(buffer, 0, sizeof(buffer)); //to erase old content
		memset(message, 0, sizeof(message));
	}

	return 0;
}
