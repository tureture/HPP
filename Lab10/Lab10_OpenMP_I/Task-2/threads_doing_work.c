#include <stdio.h>

void do_some_work() {
  printf("Now I am going to do some work...\n");
  long int i;
  long int j = 0;
  for(i = 0; i < 3000000000; i++) {
    j += 3;
  }
  printf("Work done! My result j = %ld\n", j);
}

int main(int argc, char** argv) {

  //read input number from argv
  int n;
  if(argc > 1) {
    n = atoi(argv[1]);
  } else {
    n = 4;
  }


#pragma omp parallel num_threads(n)
  {
    do_some_work();
  }

  return 0;
}

// becomes slower after more than 8 threads
