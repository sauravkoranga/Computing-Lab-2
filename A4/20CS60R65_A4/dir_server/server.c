//-----------SAURAV KORANGA--------20CS60R65--------A4 - SOCKET PROGRAMMING--------

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



//----------------------------------------function to match the testcase and output file--------------------

int match(FILE *file1, FILE *file2){
	char string[1000] = {0}, string1[1000] = {0};
	int count = 0, i;
	while(fgets(string, sizeof(string), file1) && fgets(string1, sizeof(string1), file2)){
		count = 0;
		for(i =0; string[i]; i++){
			if((string[i] != '\r') && (string[i] != '\n'))
				string[count++] = string[i];
		}
		string[count] = '\0';
		count = 0;
		for(i =0; string1[i]; i++){
			if((string1[i] != '\r') && (string1[i] != '\n'))
				string1[count++] = string1[i];
		}
		string1[count] = '\0';
		if(strcmp(string, string1) != 0){
			printf("string = %s, string1 = %s\n", string, string1);
			return 0;
		}
	}
	return 1;
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
    char string1[1000] = {0}, string2[1000] = {0}, string3[1000] = {0}, string4[1000] = {0}, string5[1000] = {0};
    struct dirent *de; // Pointer for directory entry
    time_t start, end;
    double time_spent;
    int check_input, cmd;

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

                    // ----------------------CODEJUD--------------------

                    else if(strcmp(string, "CODEJUD") == 0){
                    	printf("CODEJUD command recieved from client: %d\n", ntohs(address.sin_port));
                    	//strcat(filename, &buffer[5]);
                    	sscanf(buffer, "%s %s %s", string, filename, string2);
                        //filename[strlen(filename)-1] = '\0';
                        printf("filename = %s, type = %s\n", filename, string2);
                        //--------------------1.recieving file--------------------
                        FILE *file;
                        if(file = fopen(filename, "r")){
                            printf("File: %s already present!!\n", filename);
                            send(sd, "DONT_SEND", 50, 0);
                            fclose(file);
                            goto label1;
                        }
                        else{
                            send(sd, "SEND", 50, 0);
                            printf("message = SEND\n");
                            if(recv(sd, message, 1000, 0) < 0){
                                printf("Error while receiving!!!\n");
                                goto label1;
                            }
                            else{
                                if(strcmp(message, "File not in directory!!") == 0){
                                    printf("FILE: %s not in the client directory!\n", filename);
                                    goto label1;
                                }             
                                else{
                                	printf("CREATING FILE\n");
                                    file = fopen(filename, "a");
                                    memset(message, 0, sizeof(message));
                                    while(1){
                                    	printf("while loop!!\n");
                                        if(recv(sd, message, 1000, 0) < 0)
                                            printf("Error while receiving!!!\n");
                                        else{
                                        	printf("else condition!\n");
                                            if(strcmp(message, "FILE_TRANSFER_COMPLETE") == 0){
                                                printf("File: %s Successfully recieved!!!\n", filename);
                                                break;
                                            }
                                            else{
                                                printf("message = %s\n", message);
                                                fputs(message, file);
                                                fflush(file);
                                                memset(message, 0, sizeof(message));
                                                send(sd, "OK", 10, 0);
                                                printf("OK SENT!\n");
                                            }
                                        }
                                    }
                                    fclose(file);
                                }
                            }
                        }

                        //----------------2.compile phase-----------------------

                        printf("compiling file: %s\n", filename);
                        if(strcmp(string2, "cpp") == 0)
                        	strcat(string1, "g++ ");
                        else
                        	strcat(string1, "gcc ");
                        strcat(string1, filename);
                        strcat(string1, " -o ");
                        if(strcmp(string2, "cpp") == 0)
                        	filename[strlen(filename)-4] = '\0';
                        else
                        	filename[strlen(filename)-2] = '\0';
                        strcat(string1, filename);
                        char *str = filename;
                          
                	    // Convert string to const char * as system requires 
                        // parameter of type const char * 
                        const char *command = (const char*)string1; 
                        printf("Compiling file using: %s\n", command);
                        //COMPILE
                        system(command);
      					if(file = fopen(filename, "r")){
      						send(sd, "COMPILATION_SUCCESSFUL", 50, 0);
      						fclose(file);
      					}
      					else{
      						send(sd, "COMPILATION_UNSUCCESSFUL", 50, 0);
      						if(strcmp(string2, "cpp") == 0)
      							sprintf(string4, "%s.cpp", str);
      						else
      							sprintf(string4, "%s.c", str);
      						remove(string4);
      						goto label1;
      					}
      					printf("COMPILATION DONE!!!\n");
      					memset(message, 0, sizeof(message));
      					recv(sd, message, 1000, 0);
      					printf("Recieve after compilation: %s\n", message);


      					//-------checking for input_filename.txt and testcase_filename.txt------

      					memset(message, 0, sizeof(message));
      					check_input = 0;// input file present
      					memset(string4, 0, sizeof(string4));
      					memset(string5, 0, sizeof(string5));
      					sprintf(string5, "input_%s.txt", str);
      					sprintf(string4, "testcase_%s.txt", str);
      					printf("string4 = %s, string5 = %s\n", string4, string5);
      					if(file = fopen(string4, "r")){
      						send(sd, "TESTCASE.txt PRESENT", 50, 0);
      						printf("SEND\n");
      						recv(sd, message, 1000, 0);
      						printf("testcase.txt present!\n");
      						fclose(file);
      					}
      					else{
      						send(sd, "TESTCASE.txt NOT PRESENT", 50, 0);
      						memset(string4, 0, sizeof(string4));
      						if(strcmp(string2, "cpp") == 0)
      							sprintf(string4, "%s.cpp", str);
      						else
      							sprintf(string4, "%s.c", str);
      						remove(string4);
      						goto label1;
      					}

      					if(file = fopen(string5, "r")){
      						printf("input.txt present!!\n");
      						fclose(file);
      					}
      					else{
      						check_input = 1; // input file not present
      					}

                		//----------------3.execution and matching phase-----------------------

                        memset(string1, 0, sizeof(string1));
                        memset(string3, 0, sizeof(string3));
	                    printf("Running file.\n");
	                    if(check_input == 0){	//---------input file present--------

	                    	printf("THIS CODE REQUIRES INPUT\n");
	                    	sprintf(string1, "input_%s.txt", str);
		                    file = fopen(string1, "r");
		                    
		                    while(fgets(string3, sizeof(string3), file)){

		                    	memset(string1, 0, sizeof(string1));
		                    	FILE * file_in = fopen("in.txt", "w");
		                    	printf("string3 = %s\n", string3);
		                    	fputs(string3, file_in);
		                    	memset(string3, 0, sizeof(string3));
		                    	fclose(file_in);
		                    	sprintf(string1, "timeout 1 ./%s 0< in.txt", str);
			      				//RUN
			      				start = time(NULL);
			      				printf("start time = %ld\n", start);
			      				cmd = system(string1);
			                    if(cmd == 0){//36608
			                    	printf("EXECUTION SUCCESSFUL!\n");
			                    }
			                    else{
			                    	//system(string1);
			                    	end = time(NULL);
			                    	printf("end time = %ld\n", end);
									time_spent = end - start;
			                    	//tc = (difftime(end,start)/CLOCKS_PER_SEC);
			                    	if(time_spent >= 1){
			                    		send(sd, "TLE", 10, 0);
			                    		goto label1;
			                    	}
			                    	else{
			                      		send(sd, "RUN_TIME_ERROR", 50, 0);
			                      		goto label1;
			                    	}
			                    }
			                    memset(string1, 0, sizeof(string1));
			                    sprintf(string1, "timeout 1 ./%s 0< in.txt 1>> output_%s.txt", str, str);
			                    cmd = system(string1);
			                    printf("time_spent = %f\n", time_spent);
		                    }
	                    }
	                    else{
	                    	printf("THIS CODE DOESN'T REQUIRE INPUT\n");
	                    	sprintf(string1, "timeout 1 ./%s ", str);
	                    	//RUN
			      			start = time(NULL);
			      			printf("start time = %ld\n", start);
			      			cmd = system(string1);
			                if(cmd == 0){//36608
			                	printf("EXECUTION SUCCESSFUL!\n");
			                }
			                else{
			                	//system(string1);
			                	end = time(NULL);
			                	printf("end time = %ld\n", end);
								time_spent = end - start;
			                	//tc = (difftime(end,start)/CLOCKS_PER_SEC);
			                	if(time_spent >= 1){
			                		send(sd, "TLE", 10, 0);
			                		goto label1;
			                	}
			                	else{
			                  		send(sd, "RUN_TIME_ERROR", 50, 0);
			                  		goto label1;
			                	}
			                }
			                memset(string1, 0, sizeof(string1));
			                sprintf(string1, "timeout 10 ./%s 0< in.txt 1>> output_%s.txt", str, str);
			                cmd = system(string1);
			                printf("time_spent = %f\n", time_spent);	
	                    }
	                    
	                    send(sd, "EXECUTION_SUCCESSFUL", 50, 0);
	                    printf("MATCHING START!!\n");
	                    FILE *file1, *file2;
		                memset(data, 0, sizeof(data));
		               	sprintf(data, "testcase_%s.txt", str);
		                file1 = fopen(data, "r");
		                memset(data, 0, sizeof(data));
		                sprintf(data, "output_%s.txt", str);
		                file2 = fopen(data, "r");
		                if (file1 == NULL || file2 == NULL){
							printf("Error : Files not open\n");
							goto label1;
						}
	                    if(match(file1, file2) == 1)
		                	send(sd, "ACCEPTED", 50, 0);
		                else
		           			send(sd, "WRONG_ANSWER", 50, 0);
		           		printf("CODEJUD END!!\n");
		           		if(check_input == 0){
		                    fclose(file);
		           		}
		           		fclose(file1);
	                    fclose(file2);
	                    remove(filename);
	                    char *str1 = filename;
	                    memset(string4, 0, sizeof(string4));
	                    sprintf(string4, "output_%s.txt", str1);
	                    remove(string4);
	                    strcat(filename, ".");
	                    strcat(filename, string2);
	                    remove(filename);
	                    remove("in.txt");
                    }



                    // -----------------------------STOR---------------------------

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
                                    printf("FILE: %s not in the client directory!\n", filename);
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
                    label1:
                    memset(buffer, 0, sizeof(buffer)); //to erase old content
                    memset(message, 0, sizeof(message));
                    memset(filename, 0, sizeof(filename));
                    memset(string, 0, sizeof(string));
                    memset(string1, 0, sizeof(string1));
                    memset(string2, 0, sizeof(string2));
                    memset(string3, 0, sizeof(string3));
                    memset(string4, 0, sizeof(string4));
                    memset(string5, 0, sizeof(string5));
                    
                } 
            }   
        } 
    }    
    return 0;   
}   

