#include <stdio.h>


int main() {
    for(int i=100; i>=0; i-=2){
        printf("%d \n", i);
    }

    int i2 = 100;
    while(i2 >= 0){
        printf("%d \n", i2);
        i2 -= 2;
    }

    int i3 = 43;
    do{
        printf("%d \n", i3);
        i3 -= 2;


    } while(i3 >= 0);


    return 0;

}