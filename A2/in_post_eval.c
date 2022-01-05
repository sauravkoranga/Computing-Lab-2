
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
		if(string[i] == '('){
			prn_count++;
			if(string[i+1] == '-' && isdigit(string[i+2]))
				i++;
		}

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



//-----------------------------evaluate postfix--------------------------



float eval_post(char *post_exp){

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



int main(){

	char string[100] = {0}, string2[100] = {0};
	int count=0, i;
	fgets(string, sizeof(string), stdin);
	for(i =0; string[i]; i++){
		if((string[i] != ' '))
			string[count++] = string[i];
	}
	string[count] = '\0';
	if(valid_expression(string)){
		printf("string = %s\n", string);
	}
	else{
		printf("Invalid expression!\n");
		return 0;
	}
	strcpy(string2,in_post(string));
	printf("postfix = %s\n", string2);

	printf("solution = %.0f\n", eval_post(string2));
	return 0;
}