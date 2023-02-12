/**********************************************************************
 * This program calculates pi using C
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t m;
double sum = 0;

void * calculations(void *arg) { 
  double *input = (double *)arg;
  double lowerB = input[0];
  double upperB = input[1];
  double dx = input[2];

  double x;
  double localsum = 0;
  
  for (int i = (int)lowerB; i < (int)upperB; i++) { 
    x = dx*(i - 0.5);
    localsum += dx*4.0/(1.0 + x*x);
  }

  printf("Thread: localsum = %f \n", localsum);

  pthread_mutex_lock(&m);
  sum += localsum;
  pthread_mutex_unlock(&m);

  free(input);
  
  return NULL;
}

int main(int argc, char *argv[]) {

  const int NR_THREADS = 10;
  pthread_t thread[NR_THREADS];
  const int intervals = 500000000;
  double dx;

  pthread_mutex_init(&m, NULL);

  dx  = 1.0/intervals;

  for(int t=0; t<NR_THREADS; t++) {
    printf("Main: creating thread %d\n", t);

    double * input = malloc(3 * sizeof(double));
    input[0] = t * intervals / NR_THREADS;
    input[1] = (t + 1) * intervals / NR_THREADS;
    input[2] = dx;

    pthread_create(&thread[t], NULL, calculations, (void *)input); 
  }

  for(int t=0; t<NR_THREADS; t++) {
    pthread_join(thread[t], NULL);
  }

  pthread_mutex_destroy(&m);

  printf("PI is approx. %.16f\n", sum);

  return 0;
}


/*
Probably as accurate? If not more since you can use more iterations?
*/







