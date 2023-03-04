#include <stdio.h>
#include <omp.h>

const long int NA = 16000000000;
const long int NB = 8000000000;

long int thread_func_A() {
  long int i;
  long int sum = 1;
  for(i = 0; i < NA; i++)
    sum *= 7;
  return sum;
}

long int thread_func_B() {
  long int i;
  long int sum = 1;
  for(i = 0; i < NB; i++)
    sum *= 7;
  return sum;
}

int main() {
  int start = omp_get_wtime();
  printf("This is the main() function starting.\n");

  long int result_A;
  long int result_B;

#pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    if(id == 0)
      result_A = thread_func_A();
    else
      result_B = thread_func_B();
  }

  printf("This is the main() function after the parallel block.\n");

  printf("result_A : %ld\n", result_A);
  printf("result_B : %ld\n", result_B);
  long int totalSum = result_A + result_B;
  printf("totalSum : %ld\n", totalSum);
  int end = omp_get_wtime();
  printf("Time: %d\n", end - start);
  return 0;
}

// not exactly twice as fast, but pretty close with even workload. Compared to one thread doing all the work.
// When one thread is finished it is not using the cpu while waiting for the other thread to finish.