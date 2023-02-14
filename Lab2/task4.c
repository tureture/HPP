#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    printf("Enter num op num: ");
    int num1;
    int num2;
    int out;
    char op;

    scanf("%d %c %d", &num1, &op, &num2);

    switch (op)
    {
    case '+':
        out = num1 + num2;
        break;

    case '-':
        out = num1 - num2;
        break;
    
    default:
        out = 0;
        break;
    }
    printf("%d \n", out);
    return 0;
}