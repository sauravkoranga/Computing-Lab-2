//-----------SAURAV KORANGA--------20CS60R65--------A3 - SOCKET PROGRAMMING--------

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <ctype.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <sys/select.h>



int valid_expression(char *string){

	//printf("valid expression fn string = %s\n", string);
	int i, prn_count = 0, num_count = 0, op_count = 1;		// paranthesis count
	float value = 0, float_value = 1;

	for(i = 0; string[i]; i++){

		// right paranthesis encountered before left one
		if(prn_count < 0){
			//printf("1\n");
			return 0;
		}

		if(num_count > 1 || op_count > 1){
			//printf("num_count = %d, op_count = %d\n", num_count, op_count);
			//printf("2\n");
			return 0;
		}

		// checking paranthesis
		if(string[i] == '(')
			prn_count++;

		else if(string[i] == ')')
			prn_count--;

		else if(isalpha(string[i])){
			//printf("3\n");
			return 0;
		}

		else if(isdigit(string[i])){
			value = 0;
			float_value = 1;

			// the number can contain more than one digit
			while((isdigit(string[i]) || string[i] == '.') && (i <strlen(string))){
				
				if(string[i] == '.')
					float_value = 0.1;

				else if(isalpha(string[i])){
					//printf("4\n");
					return 0;
				}

				else{
					if(float_value == 1){
						if(value < 0)
							value = value*10 - (string[i]-'0');
						else if(value >= 0 || i != 1)
							value = value*10 + (string[i]-'0');

						// when the first int is negative
						if(string[0] == '-' && i == 1)
							value = -(string[i] - '0');
					}
					else{
						if(value >= 0){
							value += (string[i]-'0')*float_value;
							float_value /= 10;
						}
						else{
							value -= (string[i]-'0')*float_value;
							float_value /= 10;
						}
						
					}
				}
				
				i++;

			}
			i--;
			num_count++;
			op_count--;
			
		}
		else if(string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/' || string[i] == '%' || string[i] == '^' && i != 0){
			op_count++;
			num_count--;
		}

		else if(string[i] == ' ')
			continue;

		else if(string[i] == '\n')
			return 1;
	}

	if(prn_count != 0){
		//printf("5\n");
		return 0;
	}

	return 1;
}



char* check_paranthesis(char *string){

	int i = 0, last = strlen(string);
	last -= 2; 
	char *mystring = string;

	while(mystring[i] == '('){
		mystring[i] = ' ';
		mystring[last] = ' ';
		//printf("mystring = %s\n", mystring);
		if(valid_expression(mystring)){
			string = mystring;
			i++;
			last--;
		}

	}
	return string;
}



int check_expression(char *string, char const *filename, int k){

	FILE *file = fopen(filename, "r");
	char exp[100];
	int i, count = 0;

	string = check_paranthesis(string);

	for(i =0; string[i]; i++){
		if((string[i] != ' '))
			string[count++] = string[i];
	}
	string[count] = '\0';

	//printf("string = %s\n", string);
	while(k--){

		fgets(exp, sizeof exp, file);
		count = 0;
		strcpy(exp, check_paranthesis(exp));

		for(i =0; exp[i]; i++){
			if((exp[i] != ' '))
				exp[count++] = exp[i];
		}
		exp[count] = '\0';
		//printf("exp = %s\n", exp);
		if(strcmp(string, exp) == 0){
			fclose(file);
			return 1;
		}
		memset(exp, '\0', sizeof(exp));

	}
	fclose(file);
	return 0;
}





//------------------------------------------------DRIVER CODE----------------------------------------

int main(int argc, char const *argv[]) {
    
    int PORT = atoi(argv[1]);

    if(PORT <= 1024 && PORT >= 65535){
        printf("Please enter Port number between 1024 and 65535!!!\n");
        exit(1);
    }
    int server_fd, sd, valread, new_socket, client_socket[100], max_clients = 100, activity, max_sd;       
    
    struct sockaddr_in address;
    int opt = 1, childpid, count = 0, i;
    int addrlen = sizeof(address);
    char buffer[1024] = {0}, message[100] = {0}, string[1000] = {0}, filename[1000] = {0}, data[1000] = {0};
    
    struct dirent *de; // Pointer for directory entry

    fd_set readfds;     
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }    
    if( (server_fd = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }
    printf("Socket = %d created Succesfully!!!\n", server_fd);
     
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }

    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons(PORT);   
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)   
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

    while(1)   
    {     
        FD_ZERO(&readfds);    
        FD_SET(server_fd, &readfds);   
        max_sd = server_fd;

        for ( i = 0 ; i < max_clients ; i++){
            sd = client_socket[i];    
	        if(sd > 0)   
                FD_SET( sd , &readfds);
            if(sd > max_sd)   
   	            max_sd = sd;   
        }  

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
            printf("select error");

        if (FD_ISSET(server_fd, &readfds))   
        {   
        	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){   
        		perror("accept");   
      		    exit(EXIT_FAILURE);   
      	    }

            printf("Accept phase Succesfull!!!\n");
            printf("Connection socket fd: %d , ip is : %s , port : %d\n" , sd , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            for(i = 0; i < max_clients; i++){
                if( client_socket[i] == 0 ){   
				    client_socket[i] = new_socket;   
				    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }    
        for (i = 0; i < max_clients; i++){   
            sd = client_socket[i];   
                 
            if (FD_ISSET( sd , &readfds)){
                valread = read(sd,buffer,1000);
                sscanf(buffer, "%s", string);

                if (strcmp(string,"QUIT") == 0){ 
             		printf("Disconnected from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
             		memset(buffer, 0, sizeof(buffer));  
                	client_socket[i] = 0;
                	break;
                }
                else{

                	printf("CLIENT: %s\n", buffer);

                	// ---------------------LIST-----------------------

                	if(strcmp(buffer,"LIST\n") == 0){

                		printf("LIST command recieved from client: %d\n", ntohs(address.sin_port));
                		// opendir() returns a pointer of DIR type. 
                		DIR *dr = opendir("."); 

                		if (dr == NULL) // opendir returns NULL if couldn't open directory 
                		{ 
                			printf("Could not open current directory\n" ); 
                			strcat(message, "Could not open current directory\n");
                			send(sd, message, strlen(message), 0);
                		} 
                		// for readdir() 
                		while((de = readdir(dr)) != NULL){
                			printf("%s\n", de->d_name);
                			strcat(message, de->d_name);
                			strcat(message, "\n");
                		} 

                		closedir(dr);
                		printf("Sending list of files to client: %d\n", ntohs(address.sin_port));
                		send(sd, message, strlen(message), 0);
                	}

                	// ---------------------DELE-----------------------

                	else if(strcmp(string, "DELE") == 0){
                		printf("DELE command recieved from client: %d\n", ntohs(address.sin_port));

                		strcat(filename, &buffer[5]);
                		filename[strlen(filename)-1] = '\0';
                		if (remove(filename) == 0){
                			printf("Deleted %s successfully!!\n", filename);
                			strcat(message, "Deletion Succesfull!!!\n");
                			send(sd, message, strlen(message), 0);
                		}
                		else{
                			printf("File %s not in directory!!\n", filename);
                			strcat(message, "File not in directory!! Deletion UNsuccesfull!!!\n");
                			send(sd, message, strlen(message), 0);
                		}
                	}

                	// ---------------------RETR-----------------------

                	else if(strcmp(string, "RETR") == 0){
                		printf("RETR command recieved from client: %d\n", ntohs(address.sin_port));

                		strcat(filename, &buffer[5]);
                		filename[strlen(filename)-1] = '\0';
                		FILE *file;
                        if(file = fopen(filename, "r")){
                            send(sd, "Sending file", 50, 0);
                            printf("Client request file: %s\n", filename);
                            while(fgets(data, sizeof(data), file)){
                                send(sd, data, strlen(data), 0);
                                printf("data = %s\n", data);
                                memset(data, 0, sizeof(data));
                                recv(sd, message, 1000, 0);
                                printf("message = %s\n", message);
                                memset(message, 0, sizeof(message));
                            }
                            send(sd, "FILE_TRANSFER_COMPLETE", 100, 0);
                            printf("File sent successfully!!!\n");

                            fclose(file);
                        }
                        else{
                            printf("File: %s not in directory!!\n", filename);
                            send(sd, "File not in directory!!", 50, 0);
                        }
                    }
                	
                    // ---------------------STOR-----------------------

                    else if(strcmp(string, "STOR") == 0){
                        printf("STOR command recieved from client: %d\n", ntohs(address.sin_port));

                        strcat(filename, &buffer[5]);
                        filename[strlen(filename)-1] = '\0';
                        FILE *file;
                        if(file = fopen(filename, "r")){
                            printf("File: %s already present!!\n", filename);
                            send(sd, "DONT_SEND", 50, 0);
                            fclose(file);
                        }
                        else{
                            send(sd, "SEND", 50, 0);
                            if(recv(sd, message, 1000, 0) < 0)
                                printf("Error while receiving!!!\n");
                            else{
                                if(strcmp(message, "File not in directory!!") == 0)
                                    printf("FILE: %s not in the client directory!\n Enter a valid file name from LIST!\n", filename);
                                else{
                                    file = fopen(filename, "a");
                                    memset(message, 0, sizeof(message));
                                    while(1){
                                        if(recv(sd, message, 1000, 0) < 0)
                                            printf("Error while receiving!!!\n");
                                        else{
                                            if(strcmp(message, "FILE_TRANSFER_COMPLETE") == 0){
                                                printf("File: %s Succesfully recieved!!!\n", filename);
                                                break;
                                            }
                                            else{
                                                printf("message = %s\n", message);
                                                fputs(message, file);
                                                fflush(file);
                                                memset(message, 0, sizeof(message));
                                                send(sd, "OK", 10, 0);
                                            }
                                        }
                                    }
                                    fclose(file);
                                }
                            }
                        }
                
                	}
                    else{
                    	printf("Invalid command!!\n");
                        send(sd, "INVALID COMMAND!!\n", 50, 0);
                    }
                    memset(buffer, 0, sizeof(buffer)); //to erase old content
                    memset(message, 0, sizeof(message));
                    memset(filename, 0, sizeof(filename));
                    memset(string, 0, sizeof(string));
                    
                } 
            }   
        } 
    }    
    return 0;   
}   

