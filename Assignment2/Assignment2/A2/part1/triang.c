#include <stdio.h>
#include <stdlib.h>

/*
Assignment 2 HPP
Ture Hassler & Jacob Malmenstedt
*/

void print_array(double *arr, int n);

int main(int argc, char **argv) {
    
    double n = atoi(argv[1]); // read input from command line
    double product;


    // Since we only need to print we only allocate n and overwrite them
    double *array = malloc(n * sizeof(double)); 

    
    for (double j = 0; j < n; j++) { // loop over rows
        for (double k = 0; k <= j; k++) { // loop over columns
            product = 1;
            for (double i = 1; i <= k; i++) { // do math
                product = product * (j + 1 -i) / i;
                
            }
            array[(int)k] = product; // add to array
        }

        //print
        print_array(array, j+1);
        if (j != n-1) printf("\n"); // dont print newline after last row quickfix
    }

    free(array); // free memory

  return 0;
}

void print_array(double *arr, int n){
    int i;
    for(i=0; i<n; ++i) {
        printf("%d ", (int)arr[i]);
    }
}