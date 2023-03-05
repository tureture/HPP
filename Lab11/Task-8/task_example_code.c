#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


/* The function f is doing some computations based on its input and
 storing the result in the place pointed to by resultPtr. */
void f(int N, int i, double* resultPtr) {
    double result = 0;
    for (int k=0; k<i; k++)
        for(int a = 0; a < N; a++)
            for(int b = 0; b < N; b++)
                result += 0.001 * (0.3*a*b + b);
    *resultPtr = result;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Please give 1 arg: N\n");
        return -1;
    }

    int N = atoi(argv[1]);
    int M=100,i;
    double result[M];
    double finalresult=0;
    double time;
    
    time = omp_get_wtime();

    #pragma omp parallel num_threads(4) 
    {
        #pragma omp single 
        {
            for (int i=0;i<M;i++)
            {
                #pragma omp task
                f(N,i,&result[i]);
            }
        }
    }
    time = omp_get_wtime()-time;
 
    for (int i=0;i<M;i++)
        finalresult+=result[i];
    
    printf("Time = %f  finalSum = %f\n", time, finalresult);
    
    return 0;
}

// time before with n=500
// ./a.out 500  4.45s user 0.02s system 99% cpu 4.472 total
// time after with n=500
// ./a.out 500  4.81s user 0.01s system 385% cpu 1.251 total

// could not get mergesort to work. Please compare results 