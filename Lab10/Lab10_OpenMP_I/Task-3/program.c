#include <stdio.h>
#include <omp.h>

void thread_func() {
  printf("This is inside thread_func()! Thread nr: %d \n", omp_get_thread_num());
  printf("Total number of threads inside thread_func(): %d \n", omp_get_num_threads());
}

int main(int argc, char** argv) {

printf("Total number of threads before: %d \n", omp_get_num_threads());

#pragma omp parallel num_threads(4)
  {
    thread_func();
  }

  return 0;
}
