#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    printf("Enter num: ");
    int num1;


    scanf("%d", &num1);

    int s = sqrt(num1);
    if (s*s == num1){
        printf("Square \n");
    } else {
        printf("Not square \n");
    }

    
    return 0;
}