#include <stdio.h>

unsigned long long binaryExponenetiation(unsigned long long B ,unsigned long long N ,unsigned long long M)
{
    unsigned long long result = 1;
    while(N > 0)
	{	
		if(N & 1){
			result = (result * B) % M;
		}
		B = B * B % M;
		N>>=1;
	}
    return result;
}
int main()
{	
    while(1)
    {
        unsigned long long B;
        unsigned long long N;
        unsigned long long M;
        int choice;
        printf("Enter Base Value : \n");
        scanf("%llu", &B);
        printf("Enter Exponent Value : \n");
        scanf("%llu" ,&N);
        printf("Enter Modulus Value : \n");
        scanf("%llu", &M);
        unsigned long long result =  binaryExponenetiation(B,N,M);
        printf("Result is %llu\n",result);
        printf("Enter 1 to perform another opreation or 0 to exit\n");
        scanf("%d" ,&choice);
        if(!choice)
        {   
            printf("--------Exiting---------\n");
            break;
        }
    }
}
