#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int is_prime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    for (i = 2; i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int count_primes(void* arg) {
    int * input = (int * ) arg;
    int lowerB = input[0];
    int upperB = input[1];
    int nr_primes = 0;

    for(int i = lowerB; i <= upperB; i++) {
        if (is_prime(i)) {
            nr_primes++;
        }
    }

    return nr_primes;
}

// take m and nr threads as input
int main(int argc, char** argv) {
    
    //read m and nr threads from argv
    int n, M;

    if(argc > 1) {
        M = atoi(argv[1]);
        n = atoi(argv[2]);
    } else {
        n = 4;
    }
    
    int primes;

    # pragma omp parallel for num_threads(n) schedule(dynamic)
    for (int i = 1; i<M; i++) {
        if (is_prime(i)) {
            primes++;
        }
    }
    
    printf("There are %d prime numbers between 1 and %d\n", primes, M);
    return 0;
}

// best speedup at around 5.7
// Fastest for me when using a large amount of threads. OS handles load balancing

// Now works when using clang as well, just ignores the parallel part