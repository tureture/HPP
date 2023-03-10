#include <stdio.h>
#include <pthread.h>
# include <stdlib.h>

void* the_thread_func(void* arg) {
  /* Do something here? */

  int * p = (int *) malloc(3*sizeof(int));
  p[0] = 1;
  p[1] = 2;
  p[2] = 3;

  // return p;
  pthread_exit((void *) p);
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  if(pthread_create(&thread, NULL, the_thread_func, NULL) != 0) {
    printf("ERROR: pthread_create failed.\n");
    return -1;
  }

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
  int * pm;

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join(). \n");

    if(pthread_join(thread, (void **) &pm) != 0) {
    printf("ERROR: pthread_join failed.\n");
    return -1;
  }




  printf("pm[0] = %d, pm[1] = %d, pm[2] = %d \n", pm[0], pm[1], pm[2]);

  free(pm);

  return 0;
}
