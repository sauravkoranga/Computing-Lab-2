//-----------SAURAV KORANGA---------20CS60R65--------A4 - SOCKET PROGRAMMING------

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

	int sock,client_id, val = 0, value;
	struct hostent *server;
	char buffer[1000] = {0}, string[1000] = {0};
	struct sockaddr_in serv_addr;
	char string1[1000] = {0}, string2[1000] = {0};

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

	char message[1000] = {0}, filename[1000] = {0}, data[1000] = {0};

	//.......................this loop will run till close is entered by the client..........

	while(1){

		printf("\nCLIENT: Enter the message you want to send or type <QUIT> to close connection!!\n");
		fgets(buffer, sizeof buffer, stdin);
		sscanf(buffer, "%s", string);

		//...............closing connection from client side..................

		if(strcmp(string,"QUIT") == 0){
			send(sock , buffer, strlen(buffer), 0);
			printf("\nCLIENT: Connection closed!\n");
			close(sock);
			break;
		}

		//--------------------------RETR------------------------------

		else if(strcmp(string,"RETR") == 0){
			strcat(filename, &buffer[5]);
			filename[strlen(filename)-1] = '\0';
			FILE *file;
			if(!((filename[strlen(filename)-1] == 't' && filename[strlen(filename)-2] == 'x' 
				&& filename[strlen(filename)-3] == 't') || (filename[strlen(filename)-1] == 'c' 
				&& filename[strlen(filename)-2] == '.'))){
				
				printf("Please enter file type either <.c> or <.txt>\n");
				goto label;
			}
			if(file = fopen(filename, "r")){
				printf("File: %s already present!!\n", filename);
				fclose(file);
			}
			else{
				send(sock , buffer, strlen(buffer), 0);
				printf("CLIENT: %s message sent!\n\n", buffer);
				value = recv(sock, message, 1000, 0);
				if(value < 0){
					printf("Error while receiving!!!\n");
					goto label1;
				}
				else if(value == 0){
					printf("SERVER CLOSED CONNECTION!!!\n");
					exit(0);
				}
				else{
					if(strcmp(message, "File not in directory!!") == 0)
						printf("FILE: %s not in the server directory!\n", filename);
					else{
						file = fopen(filename, "a");
						memset(message, 0, sizeof(message));
						while(1){
							if(recv(sock, message, 1000, 0) < 0)
								printf("Error while receiving!!!\n");
							else{
								if(strcmp(message, "FILE_TRANSFER_COMPLETE") == 0){
									printf("File: %s Succesfully recieved!!!\n", filename);
									break;
								}
								else{
									//printf("message = %s\n", message);
									fputs(message, file);
									fflush(file);
									memset(message, 0, sizeof(message));
									send(sock, "OK", 10, 0);
								}
							}
						}
						fclose(file);
					}
				}
			}
			label:
			val = val;

		}

		//---------------------------CODEJUD---------------------

		else if(strcmp(string, "CODEJUD") == 0){

			memset(string, 0, sizeof(string));
			sscanf(buffer, "%s %s %s", string, filename, string2);
			//printf("string2 = %s\n", string2);
			if(string2[0] == '\0'){
				printf("Wrong CODEJUD command!!\n");
				goto label1;
			}
			if(strcmp(string2, "c")==0){
				//printf("%s\n", &filename[strlen(filename)-1]);
				if(strcmp(string2, &filename[strlen(filename)-1])!=0){
					printf("ERROR: Extension should be cpp\n");
					goto label1;
				}
			}
			else{
				//printf("%s\n", &filename[strlen(filename)-3]);
				if(strcmp(string2, &filename[strlen(filename)-3])!=0){
					printf("ERROR: Extension should be c\n");
					goto label1;
				}
			}
			FILE *file;
			if(!((filename[strlen(filename)-1] == 'p' && filename[strlen(filename)-2] == 'p' 
				&& filename[strlen(filename)-3] == 'c') || (filename[strlen(filename)-1] == 'c' 
				&& filename[strlen(filename)-2] == '.'))){
				
				printf("Please enter file type either <.c> or <.cpp>\n");
				goto label1;
			}
			send(sock , buffer, strlen(buffer), 0);		// IF file present at server than dont send
			value = recv(sock, message, 1000, 0);
			if(value < 0){
				printf("Error while receiving!!!\n");
				goto label1;
			}
			else if(value == 0){
				printf("SERVER CLOSED CONNECTION!!!\n");
				exit(0);
			}
			printf("message = %s\n", message);
			if(strcmp(message, "DONT_SEND") == 0){
				printf("FILE: %s already present at server directory!\n", filename);
				printf("CAN NOT CONTINUE!!\n");
				goto label1;
			}
			else{
				memset(message, 0, sizeof(message));
				if(file = fopen(filename, "r")){
				    send(sock, "Sending file", 50, 0);
				    printf("Sending file: %s to server\n", filename);
				    memset(data, 0, sizeof(data));
				    while(fgets(data, sizeof(data), file)){
				        send(sock, data, strlen(data), 0);
				        //printf("data = %s\n", data);
				        memset(data, 0, sizeof(data));
				        recv(sock, message, 1000, 0);
				        //printf("message = %s\n", message);
				        memset(message, 0, sizeof(message));
				    }
				    send(sock, "FILE_TRANSFER_COMPLETE", 100, 0);
				    printf("File sent successfully!!!\n");

				    fclose(file);
				}
				else{
				    printf("File: %s not in directory!!\n", filename);
				    send(sock, "File not in directory!!", 50, 0);
				    goto label1;
				}
			}

			//-----------------compilation-----------------------
			recv(sock, message, sizeof(message), 0);
			if(strcmp(message, "COMPILATION_SUCCESSFUL")==0)
				printf("SERVER: %s\n", message);
			else{
				printf("SERVER: %s\n", message);
				goto label1;
			}
			memset(message, 0, sizeof(message));
			send(sock, "C-OK!", 50, 0);
			//------input.txt and testcase.txt----------

			printf("checking input.txt and testcase.txt\n");
			value = recv(sock, message, 1000, 0);
			if(value < 0){
				printf("Error while receiving!!!\n");
				goto label1;
			}
			//printf("message = %s\n", message);
			if(strcmp(message, "TESTCASE.txt PRESENT")==0){
				printf("SERVER: %s\n", message);
			}
			else{
				printf("SERVER: %s\n", message);
				goto label1;
			}
			send(sock, "OK!", 50, 0);
			memset(message, 0, sizeof(message));
			

			//------------------execution------------------------

			memset(message, 0, sizeof(message));
			recv(sock, message, sizeof(message), 0);
			if(strcmp(message, "TLE")==0){
				printf("SERVER: TIME LIMIT EXCEED\n");
				goto label1;
			}
			else if(strcmp(message, "EXECUTION_SUCCESSFUL")==0)
				printf("SERVER: %s\n", message);
			else{
				printf("SERVER: RTE - %s\n", message);
				goto label1;
			}
			memset(message, 0, sizeof(message));

			//------------------matching------------------------
			recv(sock, message, sizeof(message), 0);
			printf("SERVER: %s\n", message);	// accepted or wrong answer
		}




		//------------------------------------STOR--------------------------------------

		else if(strcmp(string, "STOR") == 0){
			memset(string, 0, sizeof(string));
			strcat(filename, &buffer[5]);
			filename[strlen(filename)-1] = '\0';
			FILE *file;
			if(!((filename[strlen(filename)-1] == 't' && filename[strlen(filename)-2] == 'x' 
				&& filename[strlen(filename)-3] == 't') || (filename[strlen(filename)-1] == 'c' 
				&& filename[strlen(filename)-2] == '.'))){
				
				printf("Please enter file type either <.c> or <.txt>\n");
				goto label1;
			}
			send(sock , buffer, strlen(buffer), 0);		// IF file present at server than dont send

			value = recv(sock, message, 1000, 0);
			if(value < 0){
				printf("Error while receiving!!!\n");
				goto label1;
			}
			else if(value == 0){
				printf("SERVER CLOSED CONNECTION!!!\n");
				exit(0);
			}
			else{
				if(strcmp(message, "DONT_SEND") == 0){
					printf("FILE: %s already present at server directory!\n", filename);
				}
				else{
					if(file = fopen(filename, "r")){
					    send(sock, "Sending file", 50, 0);
					    printf("Sending file: %s to server\n", filename);
					    memset(data, 0, sizeof(data));
					    while(fgets(data, sizeof(data), file)){
					        send(sock, data, strlen(data), 0);
					        //printf("data = %s\n", data);
					        memset(data, 0, sizeof(data));
					        recv(sock, message, 1000, 0);
					        //printf("message = %s\n", message);
					        memset(message, 0, sizeof(message));
					    }
					    send(sock, "FILE_TRANSFER_COMPLETE", 100, 0);
					    printf("File sent successfully!!!\n");

					    fclose(file);
					}
					else{
					    printf("File: %s not in directory!!\n", filename);
					    send(sock, "File not in directory!!", 50, 0);
					}
				}

			}
			label1:
			val = val;
	    }
		//.........................otherwise......................

		else{

			send(sock , buffer, strlen(buffer), 0);
			printf("CLIENT: %s message sent!\n\n", buffer);
			
			value = recv(sock, message, 1000, 0);
			if(value < 0){
				printf("Error while receiving!!!\n");
				goto label1;
			}
			else if(value == 0){
				printf("SERVER CLOSED CONNECTION!!!\n");
				exit(0);
			}
			else
				printf("Server: %s\n",message);
		}
		memset(buffer, 0, sizeof(buffer)); //to erase old content
		memset(message, 0, sizeof(message));
		memset(filename, 0, sizeof(filename));
		memset(data, 0, sizeof(data));
		memset(string, 0, sizeof(string));
		memset(string2, 0, sizeof(string2));
	}

	return 0;
}
