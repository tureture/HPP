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

int main() {
    long int M = 3000000;
    double start = get_wall_seconds();

    pthread_t thread1;
    pthread_t thread2;

    long int input1[3]; 
    long int input2[3];
    input1[0] = 1;
    input1[1] = M/2;

    input2[0] = M/2 + 1;
    input2[1] = M;



    pthread_create(&thread1, NULL, count_primes, &input1);
    pthread_create(&thread2, NULL, count_primes, &input2);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    long int tot = input1[2] + input2[2];

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