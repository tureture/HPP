#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

int is_prime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    for (i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void* count_primes(void* arg) {
    long int * input = (int long * ) arg;
    long int lowerB = input[0];
    long int upperB = input[1];
    long int nr_primes = 0;

    for(int i = lowerB; i <= upperB; i++) {
        if (is_prime(i)) {
            nr_primes++;
        }
    }   

    input[2] = nr_primes;

    return NULL;
}

int main(int argc, char *argv[]) {
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    double start = get_wall_seconds();

    // Array to store the thread IDs
    pthread_t threads[n];

    int i, status;
    for (i = 0; i < N; i++) {
        // Allocate memory for the input
        int *arr = (int *) malloc(3 * sizeof(int));
        arr[0] = M/N * (i);
        arr[1] = M/N * (i+1);


        // Create the thread
        status = pthread_create(&threads[i], NULL, count_primes, (void *) arr);
        if (status != 0) {
            printf("Error creating thread %d\n", i);
        }
    }

    // Wait for all threads to exit
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }


    double end = get_wall_seconds();
    
    printf("There are %ld prime numbers between 1 and %ld \n", tot, M);
    printf("Time: %f \n", end-start);
    return 0;
}

/*
Before:
Time: 6.712922

After:
Time: 4.178897
*/