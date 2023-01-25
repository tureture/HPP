#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    
    int a = 8;
    double b = 1.3;
    char c = 'z';


    printf("int: %i %p %lu \n", a, &a, sizeof(a));
    printf("double: %f %p %lu \n", b, &b, sizeof(b));
    printf("char: %c %p %lu \n", c, &c, sizeof(c));

    return 0;
}