
//---------------------SAURAV KORANGA------------20CS60R65-----------A1 - SOCKET PROGRAMMING-------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>





//-------------------------------------------STACK FUNCTIONS-------------------------------------------------





//............Structure of a node of Operand and Operator stacks..............

struct OperandNode {

    float data;
    struct OperandNode* next;
};

struct OperatorNode {

    char data;
    struct OperatorNode* next;
};

// creating a new node in the linked list
struct OperandNode* newOperandNode(float data)
{
    struct OperandNode* OperandNode = (struct OperandNode*)malloc(sizeof(struct OperandNode));
    OperandNode->data = data;
    OperandNode->next = NULL;
    return OperandNode;
}

struct OperatorNode* newOperatorNode(char data)
{
    struct OperatorNode* OperatorNode = (struct OperatorNode*)malloc(sizeof(struct OperatorNode));
    OperatorNode->data = data;
    OperatorNode->next = NULL;
    return OperatorNode;
}

//.............returns 1 if stack is empty...............

int isEmptyOperand(struct OperandNode* root)
{
    return !root;
}

int isEmptyOperator(struct OperatorNode* root)
{
    return !root;
}

//..........push data in top of the stack...............

void pushOperand(struct OperandNode** root, float data)
{
    struct OperandNode* OperandNode = newOperandNode(data);
    OperandNode->next = *root;
    *root = OperandNode;
    printf("%f pushed to operand stack\n", data);
}

void pushOperator(struct OperatorNode** root, char data)
{
    struct OperatorNode* OperatorNode = newOperatorNode(data);
    OperatorNode->next = *root;
    *root = OperatorNode;
    printf("%c pushed to operator stack\n", data);
}

//.........pop data from top of the stack................

float popOperand(struct OperandNode** root)
{
    if (isEmptyOperand(*root))
        return INT_MIN;
    struct OperandNode* temp = *root;
    *root = (*root)->next;
    float popped = temp->data;
    free(temp);
 
    return popped;
}

char popOperator(struct OperatorNode** root)
{
    if (isEmptyOperator(*root))
        return '?';
    struct OperatorNode* temp = *root;
    *root = (*root)->next;
    char popped = temp->data;
    free(temp);
 
    return popped;
}

//...........returns topmost element of stack............

float topOperand(struct OperandNode* root)
{
    if (isEmptyOperand(root))
        return INT_MIN;
    return root->data;
}

char topOperator(struct OperatorNode* root)
{
    if (isEmptyOperator(root))
        return '?';
    return root->data;
}



//--------------------------------------STACK functions done-----------------------------------------




//.............return precedence of operators..............

int check_precedence(char ch){

	switch (ch){
		case '^': return 3;
		case '/': return 2;
		case '%': return 2;
		case '*': return 2;
		case '+': return 1;
		case '-': return 1;
		case '(': return 0;
		return 0;
	}
}

// a funtion to calculate exponent operator

float power(float x , float y){
	float result = 1;
	while(y--)
	 	result *= x;
	return result;
}

// function to perform mathematical operations
float maths(float x, float y, char ch){
    switch(ch){
      
        case '^': return power(x, y);
        case '/': if(y == 0){printf("ERROR: divide by 0\n"); return -242424;} return x / y;
        case '%': return (float)((int)x % (int)y);
        case '*': return x * y;
        case '+': return x + y;
        case '-': return x - y;
    }
}




//-------------------------------------CHECK WHETHER THE WRITEX EXPRESSION IS VALID--------------------------------




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




//------------------------------------THIS FUNCTION SOLVE THE INFIX EXPRESSION-----------------------------------




float infix_evaluator(char *string){

	if(valid_expression(string)){

		//.........creating 2 stacks..............

		struct OperatorNode* operator = NULL;	// OPERATOR STACK
		struct OperandNode* operand = NULL;	// OPERAND STACK
		int i;
		float value = 0, float_value = 1, x, y;
		char ch;

		for(i = 0; i< strlen(string); i++){

			//printf("string[%d] = %c\n", i, string[i]);

			if(isdigit(string[i])){

				value = 0;
				float_value = 1;

				// the number can contain more than one digit
				while((isdigit(string[i]) || string[i] == '.') && (i <strlen(string))){
					
					if(string[i] == '.')
						float_value = 0.1;

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
					//printf("value = %f\n",value );
					
					i++;

				}
				i--;
				pushOperand(&operand, value);

			}

			else if(string[i] == '(')
				pushOperator(&operator, '(');

			else if(string[i] == ' ')
				continue;

			else if(string[i] == '\n')
				break;

			else if(string[i] == ')'){

				while(topOperator(operator) != '(' && !isEmptyOperator(operator)){

					y = popOperand(&operand);
					x = popOperand(&operand);
					ch = popOperator(&operator);
					pushOperand(&operand, maths(x, y, ch));
				}
				ch = popOperator(&operator); // discard
			}

			// an operator encountered
			else{

				if(i != 0){
					while(!isEmptyOperator(operator) && (check_precedence(topOperator(operator)) >= check_precedence(string[i]))){
						//printf("top operator precedence = %d --- string[i] precedence = %d\n",check_precedence(topOperator(operator)), check_precedence(string[i]));
						y = popOperand(&operand);
						x = popOperand(&operand);
						ch = popOperator(&operator);
						int hola;
						hola = maths(x, y, ch);
						if(hola != -242424)
							pushOperand(&operand, maths(x, y, ch));
						else
							return INT_MAX;
					}
					pushOperator(&operator, string[i]);
				}
				

			}
			printf("string = %s\n", string);
		}

		//printf("\nseen the whole string\n");
		while(!isEmptyOperator(operator)){

			y = popOperand(&operand);
			x = popOperand(&operand);
			ch = popOperator(&operator);
			pushOperand(&operand, maths(x, y, ch));
		}

		return popOperand(&operand);
	}
	else
		return INT_MIN;
	
}





//------------------------------------------------CHECKS PARANTHESIS------------------------------------------




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




//-------------------------------------------CHECK WRITEX EXPRESSION IN FILE----------------------------------




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




//----------------------------------------------------DRIVER CODE---------------------------------------------





int main(int argc, char const *argv[]) 
{

	int PORT = atoi(argv[1]);

	if(PORT <= 1024 || PORT >= 65535){
			printf("Please enter Port number between 1024 and 65535!!!\n");
			exit(1);
	}

	char const *file_name = "input.txt";
	FILE* file = fopen(file_name,"r+");	//r for both write and read operation

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1, k, count_lines = 0, flag = 0;	// count_lines stores number of lines in input file
	int addrlen = sizeof(address);
	char buffer[1024] = {0}, string[100], str2[1000], str3[10], chr;
	char message[100] = "SERVER: HAPPY NEW YEAR!!!";
	float solution;

	chr = fgetc(file);
	while (chr != EOF){
	//Count whenever new line is encountered

        if (chr == '\n'){

            count_lines = count_lines + 1;

        }

        //take next character from file.

        chr = fgetc(file);

    }
    printf("Number of lines in the input file: %d\n", count_lines);
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	printf("Socket created Succesfully!!!\n");
	
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


	//..................................this loop will run till close is entered by the client.....................


	while(1){

		valread = read(new_socket, buffer, 1024);
		sscanf(buffer, "%s", string);

		//...............close connection message from client..................

		if(strcmp(string, "close") == 0){
			printf("\nConnection closed from server!\n");
			send(new_socket, "closing connection from server!", 50, 0);
			break;
		}

		//...........................otherwise...........................

		else{
			printf("%s\n", buffer);

			//------------------------------------------for EVALUATEX---------------------------------------------

			if(strcmp(string, "EVALUATEX") == 0){
				flag = 0;
				sscanf(buffer, "%s %s", string, str3);
				for( int i = 0; i < strlen(str3); i++){
					if(!isdigit(str3[i])){
						strcpy(message, "SERVER: INVALID INPUT!!! K should be an integer(greater than 0)!\n");
						send(new_socket , message , strlen(message), 0);
						goto here;
					}
				}
				sscanf(buffer, "%s %d", string, &k);
				//printf("%d\n",k);

				//....................when k<1, lines in file starting from 1....................

				if(k<1){
					printf("\n K value less than expected!\n");
					strcpy(message, "SERVER: K should be greater than 0, line number starts from 1!\n");
					send(new_socket , message , strlen(message), 0);
				}

				else{
					rewind(file);
					while(k--){
						if(fgets(string, sizeof string, file));

						//..............when k is more than number of lines in the file................

						else{
							flag = 1;
							printf("\n K value is more than number of lines in the file!\n");
							strcpy(message, "SERVER: K should be less than number of lines in the file, i.e. ");
							sprintf(str2, "%d\n", count_lines);
							strcat(message, str2);
							send(new_socket , message , strlen(message) , 0);
							break;
						}
					}
					if(flag == 0){
						printf("line k content: %s\n", string);

						if(strlen(string) == 1)
							strcpy(message, "SERVER: OOOPS! O_O ! Empty string at the current value of k!!!\n");
						else{
							solution =  infix_evaluator(string);
							strcpy(message, "SERVER: Expression at line k:");
							if(solution == INT_MIN){
								sprintf(str2, "%sINVALID EXPRESSION!\n", string);
								strcat(message, str2);
							}
							else if(solution == -242424){
								sprintf(str2, "%sDIVIDE BY ZERO ERROR!\n", string);
								strcat(message, str2);
							}
							else{
								sprintf(str2, "%sSolution:", string);
								strcat(message, str2);
								sprintf(str2, "%.3f\n", solution);
								strcat(message, str2);
							}
						
						}
						send(new_socket, message, strlen(message), 0);
					}

				}
				here: //discard
					k = k;
				
			}

			//---------------------------------------------for WRITEX----------------------------------------

			else if(strcmp(string, "WRITEX") == 0){

				// check whether expression is valid or not...
				if(!valid_expression(&buffer[7])){

					strcpy(message, "SERVER: OOOPS!!! The expression you entered is INVALID!!!\n");
					send(new_socket , message , strlen(message) , 0);
				}
				else{

					// when expression not in the file write it...
					if(!check_expression(&buffer[7], file_name, count_lines)){

						fseek(file, 0, SEEK_END);
						fputs(&buffer[7], file);
						strcpy(message, "SERVER: CONGRATULATIONS! WRITEX Succesfull!!!\n");
						send(new_socket , message , strlen(message) , 0);
						count_lines++;
					}
					else{
						strcpy(message, "SERVER: OOOPS!!! The expression is ALREADY PRESENT in the file!!!\n");
						send(new_socket , message , strlen(message) , 0);
					}
					
				}

			}

			//------------------------------------otherwise send error message-------------------------------

			else{
				printf("\nInvalid input from client!\nExpecting either <EVALUATEX int> OR <WRITEX exp>!\n");
				strcpy(message, "SERVER: Please enter either <EVALUATEX int> OR <WRITEX exp>!\n");
				send(new_socket , message , strlen(message) , 0);
			}
			
		}
		  memset(message, 0, sizeof(message)); //to erase old content
		  memset(buffer, 0, sizeof(buffer));
		
	}

	fclose(file);
	return 0;

} 
