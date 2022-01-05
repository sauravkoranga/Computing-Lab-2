
//---------------------SAURAV KORANGA------------20CS60R65-----------A1 - SOCKET PROGRAMMING-------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) 
{
	
	int PORT = atoi(argv[1]);

	if(PORT <= 1024 || PORT >= 65535){
			printf("Please enter Port number between 1024 and 65535!!!\n");
			exit(1);
	}

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;	// count_lines stores number of lines in input file
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char message[100] = "SERVER: HAPPY NEW YEAR!!!";
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	printf("Socket = %d created Succesfully!!!\n", server_fd);
	
	// Forcefully attaching socket to the port 4444
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 4444
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}

	printf("Socket Bind phase Succesfull!!!\n");
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 

	printf("Listen phase Succesfull!!!\n");

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}

	printf("Accept phase Succesfull!!!\n");

	char string[100] = {0};
	while(1){

		valread = read(new_socket, buffer, 1024);
		sscanf(buffer, "%s", string);

		//...............close connection message from client..................

		if(strcmp(string, "close") == 0){
			printf("\nConnection closed from server!\n");
			send(new_socket, "closing connection from server!", 50, 0);
			break;
		}

		else{

			printf("\nMessage from client: %s", buffer);
			printf("Sending ACK!\n");
			send(new_socket, "SERVER: This message is ACK from the server!", 50, 0);

		}
		memset(buffer, 0, sizeof(buffer));
	}

	return 0;
}