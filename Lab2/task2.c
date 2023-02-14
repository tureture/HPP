#include <stdio.h>

int main(){
    printf("Enter 2 numbers: ");
    int abc;
    int xyz;

    scanf("%d", &abc);
    scanf("%d", &xyz);

    for(int i=0; i<abc; i++){
        for(int j=0; j<xyz; j++){
            if(i==0 || i==abc-1 || j==0 || j==xyz-1){
                printf("*");
            } else {
                printf("-");
            }

        }
        printf("\n");
    }


}