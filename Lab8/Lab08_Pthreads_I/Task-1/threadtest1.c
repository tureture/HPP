#include <stdio.h>
#include <pthread.h>

void* the_thread_func(void* arg) {
  /* Do something here? */
  
  printf("threadfunc starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 10000000; i++)
    sum += 1e-5;
  printf("Result of work in threadfunc: sum = %f\n", sum);
  return NULL;
}

void* the_thread_func_B(void* arg) {
  /* Do something here? */
  
  printf("threadfunc B starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 10000000; i++)
    sum += 1e-5;
  printf("Result of work in threadfunc B: sum = %f\n", sum);
  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  pthread_t thread_B;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, NULL);
  pthread_create(&thread_B,  NULL, the_thread_func_B, NULL);

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
  printf("main() starting doing some work.\n");
  long int i;
  double sum;
  for(i = 0; i < 100000000; i++)
    sum += 1e-7;
  printf("Result of work in main(): sum = %f\n", sum);

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);
  pthread_join(thread_B, NULL);

  return 0;
}
