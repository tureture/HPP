#include <stdio.h>
#include <stdlib.h>

/*
Assignment 2 HPP
Ture Hassler & Jacob Malmenstedt
*/

void print_array(double *arr, int n);

int main(int argc, char **argv) {

    // check so we get correct number of inputs
    if (argc != 2)
    {
        printf("Usage: ./triang \n");
        return 1;
    }
    
    double n = atoi(argv[1]); // read input from command line
    double product;
  
    for (double j = 0; j < n; j++) { // loop over rows

        double *array = malloc((j + 1) * sizeof(double)); // dynamically allocate memory

        for (double k = 0; k <= j; k++) { // loop over columns
            product = 1;
            for (double i = 1; i <= k; i++) { // do math
                product = product * (j + 1 -i) / i;
                
            }
            array[(int)k] = product; // add to array
        }

        print_array(array, j+1); //print
        free(array); // free memory 
    }

  return 0;
}


void print_array(double *arr, int n){
    int i;
    for(i=0; i<n; ++i) {
        printf("%d \n", (int)arr[i]);
    }
}