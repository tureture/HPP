#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

const long int N1 = 350000000;
const long int N2 = 350000000;

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

void* the_thread_func(void* arg) {
  long int i;
  long int sum = 0;
  double start = get_wall_seconds();
  for(i = 0; i < N2; i++)
    sum += 7;
  /* OK, now we have computed sum. Now copy the result to the location given by arg. */
  long int * resultPtr;
  resultPtr = (long int *)arg;
  *resultPtr = sum;

  double end = get_wall_seconds();
  printf("Time inside thread: %f \n", end-start);
  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  double start_all = get_wall_seconds();

  long int thread_result_value = 0;

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, &thread_result_value);

  printf("This is the main() function after pthread_create()\n");

  long int i;
  long int sum = 0;
  for(i = 0; i < N1; i++)
    sum += 7;

  double end_main_loop = get_wall_seconds();
  printf("Time for end of main loop: %f \n", end_main_loop-start_all);

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);

  printf("sum computed by main() : %ld\n", sum);
  printf("sum computed by thread : %ld\n", thread_result_value);
  long int totalSum = sum + thread_result_value;
  printf("totalSum : %ld\n", totalSum);

  double end_all = get_wall_seconds();
  printf("Total time: %f \n", end_all-start_all);


  return 0;
}

/*
Time after splitting evenly:
Time for end of main loop: 1.081890 
the main() function now calling pthread_join().
Time inside thread: 1.127784 
sum computed by main() : 2450000000
sum computed by thread : 2450000000
totalSum : 4900000000
Total time: 1.128093 

Approx 2x faster
*/