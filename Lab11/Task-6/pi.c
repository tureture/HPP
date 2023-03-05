/**********************************************************************
 * This program calculates pi using C
 *
 **********************************************************************/
#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {

  int i;
  const int intervals = 500000000;
  double sum, dx, x, localsum;

  dx  = 1.0/intervals;
  sum = 0.0;

  #pragma omp parallel for private(x) reduction(+:sum) schedule(guided) 
  for (i = 1; i <= intervals; i++) { 
    x = dx*(i - 0.5);
    sum += dx*4.0/(1.0 + x*x);
  }

  printf("PI is approx. %.16f\n",  sum);

  return 0;
}

// for some reason schedule(dynamic) is super slow for me.
// Schedule guided was best, and static was second best.

// from 0.46s to 0.1s

// a lot easier compared to pthreads