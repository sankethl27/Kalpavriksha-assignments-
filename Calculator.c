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
    if(op == '*' || op == '/') return 2;
    if(op == '+' || op == '-') return 1;
    return 0;
}

long long int performOperation(long long int a ,long long int b , char op){
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
    long long int values_stack[MaxLen];
    long long int values_top = -1, operators_top = -1;
    for(int i = 0; exp[i] != '\0'; i++){

        // if(isspace(exp[i])) continue;
        if(isdigit(exp[i])){
            long long int value = 0;

            while(exp[i] != '\0' && isdigit(exp[i])){
                value = value * 10 + 1ll*(exp[i] - '0');
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
            while(operators_top >= 0 && precedence(exp[i]) <= precedence(operators[operators_top])){

                if (values_top < 1) {
                    printError("Error: Invalid expression.");
                }
                
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

        if (values_top < 1) {
            printError("Error: Invalid expression.");
        }

        if (operators[operators_top] == '(') {
            printError("Error: Mismatched parentheses.");
        }
        int b = values_stack[values_top--];
        int a = values_stack[values_top--];
        char opreator = operators[operators_top--];
        values_stack[++values_top] = performOperation(a,b,opreator);  
    }
    // printf("Result : %d" , values_stack[values_top]);
    return values_stack[values_top];
}

void main(){

    while(1){
        char inputString[MaxLen];
        printf("Enter a mathematical expression (or type 'exit' to quit):\n");

        if(!fgets(inputString,MaxLen,stdin)){
            printError("Error reading input");
        }
        
        inputString[strcspn(inputString, "\n")] = '\0';

        if (strlen(inputString) == 0) {
            printf("Please enter a valid expression.\n");
            continue;
        }

        if (strcmp(inputString, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }   

        
        removeWhiteSpaces(inputString);

        long long int result = calculate(inputString);
        
        printf("The result of '%s' is: %lld\n", inputString, result);

        printf("\n");

    }

   
}