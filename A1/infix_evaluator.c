#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>

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
/* 
int main()
{
    struct Node* root = NULL;
 
    push(&root, 10);
    push(&root, 20);
    push(&root, 30);
 
    printf("%d popped from stack\n", pop(&root));
 
    printf("Top element is %d\n", top(root));
 
    return 0;
}*/

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
        case '/': return x / y;
        case '%': return (float)((int)x % (int)y);
        case '*': return x * y;
        case '+': return x + y;
        case '-': return x - y;
    }
}

float infix_evaluator(char *string){

	//.........creating 2 stacks..............

	struct OperatorNode* operator = NULL;	// OPERATOR STACK
	struct OperandNode* operand = NULL;	// OPERAND STACK
	int i;
	float value = 0, float_value = 1, x, y;
	char ch;

	for(i = 0; i< strlen(string); i++){

		printf("string[%d] = %c\n", i, string[i]);

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
				printf("value = %f\n",value );
				
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
					pushOperand(&operand, maths(x, y, ch));
				}
				pushOperator(&operator, string[i]);
			}
			

		}
	}

	printf("\nseen the whole string\n");
	while(!isEmptyOperator(operator)){

		y = popOperand(&operand);
		x = popOperand(&operand);
		ch = popOperator(&operator);
		pushOperand(&operand, maths(x, y, ch));
	}

	return popOperand(&operand);
}

int main(int argc, char const *argv[]){
	
	int count_lines = 0, k;
	/*file is the pointer to text file passed as first argument*/
	FILE* file = fopen(argv[1],"r+");

	char chr, string[100] = {0};

	chr = fgetc(file);
	while (chr != EOF){
	//Count whenever new line is encountered

        if (chr == '\n'){

            count_lines = count_lines + 1;

        }

        //take next character from file.

        chr = fgetc(file);

    }
    printf("Number of lines: %d\n", count_lines);

    scanf("%d", &k);

    rewind(file);
    while(k--){

    	if(fgets(string, sizeof string, file));
    }
    printf("line k content: %s length = %ld\n", string, strlen(string));
    if(strlen(string) == 1)
    	printf("Empty string!!!\n");
    else
    	printf("solution = %.3f\n", infix_evaluator(string));
	printf("program end!!!\n");

	fclose(file);
	return 0;
}