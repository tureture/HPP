#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    printf("Enter 2 numbers: ");
    int abc;
    int xyz;

    scanf("%d", &abc);
    scanf("%d", &xyz);

    if(abs(abc) > abs(xyz)){
        printf("%d \n", abc);
    } else {
        printf("%d \n", xyz);
    } 

}