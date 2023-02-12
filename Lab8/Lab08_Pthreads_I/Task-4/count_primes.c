#include <stdio.h>
#include <math.h>
#include <sys/time.h>

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

int main() {
    int M = 3000000;
    int p;
    double start = get_wall_seconds();

    int input[2];
    input[0] = 1;
    input[1] = M;

    p = count_primes(&input);

    double end = get_wall_seconds();
    
    printf("There are %d prime numbers between 1 and %d\n", p, M);
    printf("Time: %f \n", end-start);
    return 0;
}