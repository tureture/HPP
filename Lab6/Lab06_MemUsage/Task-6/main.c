#include <stdio.h>
// #include "func.h"


int f(int k);
int get_counter();



int main() {
 

  int n = 20000;
  int i, j;
  int sum = 0;
  // int o;

  for(i = 0; i < n; i++) {
    // o = f(i);
    for(j = 0; j < n; j++) {
      sum += f(i) + 3*j;
      // sum += o + 3*j;
    }
  }
  printf("Result sum: %d\n", sum);
  printf("Function call counter: %d\n", get_counter());
  return 0;
}

static int function_call_counter = 0;

int f(int k) {
  // Increment function call counter
  function_call_counter++;
  // Do some expensive operations so that this function takes some time
  int x = 3;
  if(k > 0 && 88/k > 2)
    x += 11;
  if(k > 0)
    x += 777/k + 888 / k + 999 / k;
  return x;
}

int get_counter() {
  return function_call_counter;
}


/*
No changes:
0.78s user 0.00s system 99% cpu 0.791 total

Moving function call to outer loop
0.00s user 0.00s system 65% cpu 0.004 total

after adding pure directive to func.h
0.00s user 0.00s system 61% cpu 0.005 total

after inserting function into main file 
0.08s user 0.00s system 97% cpu 0.079 total
*/

