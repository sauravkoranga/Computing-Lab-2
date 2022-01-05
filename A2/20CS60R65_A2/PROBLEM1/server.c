//---------------------SAURAV KORANGA------------20CS60R65-----------A2 - SOCKET PROGRAMMING-------------------


#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <sys/select.h>





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
		case '^': return 5;
		case '/': return 4;
		case '%': return 3;
		case '*': return 3;
		case '+': return 2;
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





//----------------------------------------infix to postfix converter------------------------------------------




char * in_post(char *string){

	string[strlen(string)-1] = '\0';
	printf("string = %s\n", string);
	struct OperatorNode* operator = NULL;	// OPERATOR STACK
	char postfix[100] = {0}, ch, str[10], *pfix;
	float value = 0, float_value = 1;

	for(int i = 0; string[i]; i++){

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
			sprintf(str, "%.0f", value);
			strcat(postfix, str);
			postfix[strlen(postfix)] = ' ';
		}

		else if(string[i] == '(')
			pushOperator(&operator, '(');

		else if(string[i] == ')'){
			while(topOperator(operator) != '('){
				ch = popOperator(&operator);
				postfix[strlen(postfix)] = ch;
				postfix[strlen(postfix)] = ' ';
			}
			ch = popOperator(&operator);		// discard
		}
		else{

			if(isEmptyOperator(operator) || topOperator(operator) == '(' || (check_precedence(topOperator(operator)) <= check_precedence(string[i])))
				pushOperator(&operator, string[i]);

			else{
				while(check_precedence(topOperator(operator)) >= check_precedence(string[i]) && !isEmptyOperator(operator)){
					ch = popOperator(&operator);
					//printf("ch = %c\n",ch );

					if(ch == '(' || ch == ')')
						break;
					else{
						//printf("postfix = %s\n", postfix);
						postfix[strlen(postfix)] = ch;
						//printf("postfix = %s\n", postfix);
						postfix[strlen(postfix)] = ' ';
					}
				}
				pushOperator(&operator, string[i]);
			}
		}
		//printf("string ---- %s\n", string);
	}
	while(!isEmptyOperator(operator)){
		ch = popOperator(&operator);
		//printf("len = %ld\n", strlen(postfix));
		postfix[strlen(postfix)] = ch;
		postfix[strlen(postfix)] = ' ';
	}
	pfix = postfix;
	return pfix;
}




//--------------------------------------------------evaluate postfix----------------------------------------------




float eval_post(char *post_exp){

	post_exp = check_paranthesis(post_exp);
	struct OperandNode* opd = NULL;	// OPERAND STACK
	int i;
	float value = 0, float_value = 1, x, y;
	char ch;
	for(i = 0; post_exp[i]; i += 2){

		if(isdigit(post_exp[i])){

			value = 0;
			float_value = 1;

			// the number can contain more than one digit
			while((isdigit(post_exp[i]) || post_exp[i] == '.') && (i <strlen(post_exp))){

				if(post_exp[i] == '.')
					float_value = 0.1;

				else{
					if(float_value == 1){
						if(value < 0)
							value = value*10 - (post_exp[i]-'0');
						else if(value >= 0 || i != 1)
							value = value*10 + (post_exp[i]-'0');

						// when the first int is negative
						if(post_exp[0] == '-' && i == 1)
							value = -(post_exp[i] - '0');
					}
					else{
						if(value >= 0){
							value += (post_exp[i]-'0')*float_value;
							float_value /= 10;
						}
						else{
							value -= (post_exp[i]-'0')*float_value;
							float_value /= 10;
						}
						
					}
				}
				//printf("value = %f\n",value );
				
				i++;

			}
			i--;
			pushOperand(&opd, value);
		}
		else{
			y = popOperand(&opd);
			x = popOperand(&opd);
			ch = post_exp[i];
			pushOperand(&opd, maths(x, y, ch));
		}
	}
	return popOperand(&opd);
}




//-------------------------------------------DRIVER CODE----------------------------------------------




int main(int argc, char const *argv[]) 
{
	
	int PORT = atoi(argv[1]);

	if(PORT <= 1024 || PORT >= 65535){
		printf("Please enter Port number between 1024 and 65535!!!\n");
		exit(1);
	}

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1, childpid;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char string[100] = {0};
	char message[100] = "SERVER: HAPPY NEW YEAR!!!";
	int count=0, i;
	time_t begin, end;
	float solution;
	char string2[1000] = {0}, string3[1000] = {0}, str1[1000] = {0}, str2[1000];
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	printf("Socket = %d created Succesfully!!!\n", server_fd);
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	
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

	FILE *file = fopen("server_records.txt", "w");

	while(1){

		if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}
		begin = time(NULL);

		printf("Accept phase Succesfull!!!\n");
		printf("Connection accepted from %s : %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

		if((childpid = fork()) == 0){
			close(server_fd);

			while(1){

				valread = read(new_socket, buffer, 1024);
				sscanf(buffer, "%s", string);

				if(strcmp(string, "close") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
					memset(buffer, 0, sizeof(buffer));
					break;
				}
				else{
					count = 0;
					for( i =0; buffer[i]; i++){
						if((buffer[i] != ' '))
							buffer[count++] = buffer[i];
					}
					buffer[count] = '\0';

					if(valid_expression(buffer)){

						strcat(string2, "POSTFIX: ");
						printf("string = %s\n", buffer);
						strcat(str1, in_post(buffer));		// postfix exp
						strcat(string2, str1);
						strcat(string3, string2);
						strcat(string3, "	RESULT: ");
						solution = eval_post(str1);
						if(solution == -242424)
							strcpy(str2, "Divide by zero");
						else
							sprintf(str2, "%.3f", solution);
						strcat(string3, str2);

						// forwarding to client
						send(new_socket, string3, strlen(string3), 0);

						printf("postfix = %s\n", string2);
						printf("solution = %.3f\n", solution);
						

						// to write in file
						strcpy(string2, "CLIENT: ");
						strcat(string2, inet_ntoa(address.sin_addr));
						memset(str2, 0, sizeof(str2));
						sprintf(str2, "%d", ntohs(address.sin_port));
						strcat(string2, str2);
						strcat(string2, "	INFIX: ");
						strcat(string2, buffer);
						strcat(string2, "	");
						strcat(string2, string3);
						strcat(string2, "	TIME_ELAPSED: ");
						end = time(NULL);
						double time_spent = end - begin;
						memset(str2, 0, sizeof(str2));
						sprintf(str2, "%.2lf\n", time_spent);
						strcat(string2, str2);
						fputs(string2, file);
						fflush(file);
					}
					else{
						printf("Invalid expression!\n");
						send(new_socket, "INVALID EXPRESSION\n", 50, 0);
					}
					
				}
				memset(buffer, 0, sizeof(buffer));
				memset(string2, 0, sizeof(string2));
				memset(string3, 0, sizeof(string3));
				memset(str1, 0, sizeof(str1));
				memset(str2, 0, sizeof(str2));
			}		
		}
		else
			continue;
	}
	fclose(file);
	return 0;
}
