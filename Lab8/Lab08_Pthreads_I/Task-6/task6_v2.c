#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

struct data_struct{
        int lowerB;
        int upperB;
        int nr_primes;
}; 

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
    struct data_struct * input = (struct data_struct * ) arg;
    int lowerB = input->lowerB;
    int upperB = input->upperB;
    int nr_primes = 0;

    for(int i = lowerB; i <= upperB; i++) {
        if (is_prime(i)) {
            nr_primes++;
        }
    }   

    input->nr_primes = nr_primes;
    return NULL;
}

int main(int argc, char *argv[]) {
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    double start = get_wall_seconds();

    // Array to store the thread IDs
    pthread_t threads[N];
    struct data_struct thread_data[N];

    int i, status;
    for (i = 0; i < N; i++) {
        thread_data[i].lowerB = M/N * (i);
        thread_data[i].upperB = M/N * (i+1);

        // Create the thread
        status = pthread_create(&threads[i], NULL, count_primes, (void *) &thread_data[i]);
        if (status != 0) {
            printf("Error creating thread %d\n", i);
        }
    }

    // Wait for all threads to exit
    for (i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    int tot = 0;
    for (i = 0; i < N; i++) {
        tot += thread_data[i].nr_primes;
    }

    double end = get_wall_seconds();
    
    printf("There are %d prime numbers between 1 and %d \n", tot, M);
    printf("Time: %f \n", end-start);
    return 0;
}

/*
1 thread:
There are 283146 prime numbers between 1 and 4000000 
Time: 10.079035 

4 thread:
There are 283146 prime numbers between 1 and 4000000 
Time: 3.357789 

12 threads:
There are 283146 prime numbers between 1 and 4000000 
Time: 1.180724

100 threads:
There are 283146 prime numbers between 1 and 4000000 
Time: 0.411854 

1000 threads:
There are 283146 prime numbers between 1 and 4000000 
Time: 0.422987 

No further speedup with too many threads 
*/