#include <stdio.h>
#include <stdlib.h>


int main(){
    int n = 5;
    int matrix[n][n];

    for (int j=0; j<n; j++){
        for (int i=0; i<n; i++){
            if (i==j){
                matrix[i][j] = 0;
            }
            else if (i>j){
                matrix[i][j] = 1;
            }
            else if (i<j){
                matrix[i][j] = -1;
            }

        }
    }
    
    for (int j=0; j<n; j++){
        for (int i=0; i<n; i++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}