#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxLen 100
int Error = 0;

int IsSpace(char Character)
{
    return Character == ' ' || Character == '\t' || Character == '\n';
}

void RemoveWhiteSpaces(char *Expression)
{
    char *Left = Expression;
    char *Right = Expression;
    while (*Right != '\0')
    {
        if (!IsSpace(*Right))
        {
            *Left++ = *Right;
        }
        Right++;
    }
    *Left = '\0';
}

int IsDigit(char Character)
{
    return Character >= '0' && Character <= '9';
}

int StringCompare(const char *str1, const char *str2)
{
    while (*str1 != '\0' && *str2 != '\0')
    {
        if (*str1 != *str2)
        {
            return (unsigned char)(*str1) - (unsigned char)(*str2);
        }
        str1++;
        str2++;
    }
    return (unsigned char)(*str1) - (unsigned char)(*str2);
}

int StringLength(const char *String)
{
    int Length = 0;
    while (String[Length] != '\0')
    {
        Length++;
    }
    return Length;
}

int Precedence(char Operator)
{   
    int Value = 0;
    if (Operator == '*' || Operator == '/')
        Value = 2;
    if (Operator == '+' || Operator == '-')
        Value = 1;
    return Value;
}

int PerformOperation(int OperandA, int OperandB, char Operator)
{   
    int Result = 0;
    switch (Operator)
    {
    case '/':
        if (OperandB == 0)
        {
            printf("Error: Division by zero.\n");
            Error = 1;
            break;
        }   
        Result = OperandA / OperandB;
        break;  

    case '*':
        Result = OperandA * OperandB;
        break;

    case '+':
        Result = OperandA + OperandB;
        break;

    case '-':
        Result = OperandA - OperandB;
        break;

    default:
        printf("Error: Invalid operator.\n");
        Error = 1;
    }
    return Result;
}

int Calculate(char *Expression)
{
    char Operators[MaxLen];
    int ValuesStack[MaxLen];
    int ValuesTop = -1, OperatorsTop = -1;

    for (int Index = 0; Expression[Index] != '\0'; Index++)
    {   
        if (Error) break;
        if (IsDigit(Expression[Index]))
        {
            int Value = 0;
            while (Expression[Index] != '\0' && IsDigit(Expression[Index]))
            {
                Value = Value * 10 + (Expression[Index] - '0');
                Index++;
            }
            Index--;
            ValuesStack[++ValuesTop] = Value;
        }
        else if (Expression[Index] == '(')
        {
            Operators[++OperatorsTop] = Expression[Index];
        }
        else if (Expression[Index] == ')')
        {
            while (OperatorsTop >= 0 && Operators[OperatorsTop] != '(')
            {
                int OperandB = ValuesStack[ValuesTop--];
                int OperandA = ValuesStack[ValuesTop--];
                char Operator = Operators[OperatorsTop--];
                ValuesStack[++ValuesTop] = PerformOperation(OperandA, OperandB, Operator);
            }
            OperatorsTop--;
        }
        else if (Expression[Index] == '+' || Expression[Index] == '-' || Expression[Index] == '*' || Expression[Index] == '/')
        {
            while (OperatorsTop >= 0 && Precedence(Expression[Index]) <= Precedence(Operators[OperatorsTop]))
            {
                if (ValuesTop < 1)
                {
                    printf("Error: Invalid expression.\n");
                    Error = 1;
                }

                int OperandB = ValuesStack[ValuesTop--];
                int OperandA = ValuesStack[ValuesTop--];
                char Operator = Operators[OperatorsTop--];
                ValuesStack[++ValuesTop] = PerformOperation(OperandA, OperandB, Operator);
            }
            Operators[++OperatorsTop] = Expression[Index];
        }
        else
        {   
            Error = 1;
            printf("Invalid Expression.\n");
        }

        
    }
    while (OperatorsTop >= 0)
    {   
        if (Error) break;
        if (ValuesTop < 1)
        {
            printf("Error: Invalid expression.\n");
            Error = 1; 
        }

        if (Operators[OperatorsTop] == '(')
        {
            printf("Error: Mismatched parentheses.\n");
            Error = 1;
        }   

        int OperandB = ValuesStack[ValuesTop--];
        int OperandA = ValuesStack[ValuesTop--];
        char Operator = Operators[OperatorsTop--];
        ValuesStack[++ValuesTop] = PerformOperation(OperandA, OperandB, Operator);
    }
    return ValuesStack[ValuesTop];
}

void main()
{
    while (1)
    {   
        Error = 0;
        char InputString[MaxLen];
        printf("Enter a mathematical expression (or type 'exit' to quit):\n");
        if (!fgets(InputString, MaxLen, stdin))
        {
            printf("Error reading input\n");
            continue;
        }
        InputString[StringLength(InputString) - 1] = '\0';  // Remove newline

        if (StringLength(InputString) == 0)
        {
            printf("Please enter a valid expression.\n");
            continue;
        }

        if (StringCompare(InputString, "exit") == 0) 
        {
            printf("Exiting...\n");
            break;
        }

        RemoveWhiteSpaces(InputString);
        int Result = Calculate(InputString);
        if (!Error)  
        {
            printf("The result of '%s' is: %d\n", InputString, Result);
        }
      
        printf("\n");
    }
} 