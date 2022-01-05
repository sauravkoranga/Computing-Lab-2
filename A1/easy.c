#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>


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

int main(int argc, char const *argv[]){
	
	int count_lines = 0, k;
	/*file is the pointer to text file passed as first argument*/
	FILE* file = fopen(argv[1],"r+");

	char chr;

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

    char string[100];
    fgets(string, sizeof string, stdin);

    printf("string = %s---length = %ld\n", string, strlen(string));
    if(check_expression(string, argv[1], count_lines))
    	printf("Already present in file!\n");
    else
    	printf("Not in file!\n");
    
	printf("program end!!!\n");

	fclose(file);
	return 0;
}