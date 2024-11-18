#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MaxLen 100

void printError(const char* message){
    printf("%s\n" ,message);
    exit(EXIT_FAILURE);
}

void removeWhiteSpaces(char* exp){
    char* l = exp;
    char* r = exp;
    while(*r != '\0'){
        if(!isspace(*r)){
            
            *l++ = *r;
        } 
        r++;
    }
    *l = '\0';
}

int precedence(char op){
    if(op == '*' || op == '/') return 1;
    if(op == '+' || op == '-') return 2;
    return 0;
}

int performOperation(int a , int b , char op){
    switch(op){
        case '/': 
            if(b == 0) printError("Error: Division by zero.");
            return a / b;
               
        case '*' : return a * b;    
        case '+' : return a + b;
        case '-' : return a - b;
        default : printError("Error: Invalid operator.");
    }
    return 0;
}

long long int calculate(char* exp){
    char operators[MaxLen];
    int values_stack[MaxLen];
    int values_top = -1, operators_top = -1;
    for(int i = 0; exp[i] != '\0'; i++){
        if(isdigit(exp[i])){
            int value = 0;

            while(exp[i] != '\0' && isdigit(exp[i])){
                value = value * 10 + (exp[i] - '0');
                i++;
            }
            
            i--;
            values_stack[++values_top] = value;
        }
        else if(exp[i] == '('){
            operators[++operators_top] = exp[i];
        }  
        else if(exp[i] == ')'){
            while(operators_top >= 0 && operators[operators_top] != '('){
                int b = values_stack[values_top--];
                int a = values_stack[values_top--];
                char opreator = operators[operators_top--];
                values_stack[++values_top] = performOperation(a,b,opreator);   
            }
            operators_top--;
        }
        else if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' ){
            while(operators_top >= 0 && precedence(exp[i]) < precedence(operators[operators_top])){
                int b = values_stack[values_top--];
                int a = values_stack[values_top--];
                char op = operators[operators_top--];
                values_stack[++values_top] = performOperation(a,b,op);   
            }
            operators[++operators_top] = exp[i];
        }
        else{
            printError("Invalid Expression.");
        }


    }
    while(operators_top >=0){
        if (operators[operators_top] == '(') {
            printError("Error: Mismatched parentheses.");
        }
        int b = values_stack[values_top--];
        int a = values_stack[values_top--];
        char opreator = operators[operators_top--];
        values_stack[++values_top] = performOperation(a,b,opreator);  
    }
    // printf("Result : %d" , values_stack[values_top]);
    return 1ll * values_stack[values_top];
}

void main(){

    while(1){
        char inputString[MaxLen];
        printf("Enter a mathematical expression (or type 'exit' to quit):\n");

        if(!fgets(inputString,MaxLen,stdin)){
            printError("Error reading input");
        }
        
        inputString[strcspn(inputString, "\n")] = '\0';
        if (strcmp(inputString, "exit") == 0) {
        printf("Exiting...\n");
        break;
}
        printf("Input Expression: %s\n", inputString);

        
        removeWhiteSpaces(inputString);

        long long int result = calculate(inputString);
        
        printf("Result : %lld" ,result);

        printf("\n");

    }

   
}