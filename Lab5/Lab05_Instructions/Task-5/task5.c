#include <stdio.h>
#include <math.h>

static inline void math_int(int n);
static inline void math_float(int n);
static inline void math_sqrt(int n);

int main()
{
   int big_n = 10000000;
   int small_n = 100;
   float x;
   for (int i=0; i<big_n; i++)
   {
        math_int(small_n);
        math_float(small_n);
        math_sqrt(small_n);
   }
   return 0;
}

static inline void math_int(int n){ // do some integer math
    int x;
    for (int i = 0; i < n; i++){
        x = i * i;
    }
}

static inline void math_float(int n){ // do some float math
    float x;
    for (int i = 0; i < n; i++){
        x = x * i;
    }
}

static inline void math_sqrt(int n){ // do some sqrt math
    double x;
    for (int i = 1; i < n; i++){
        x = sqrt(i);
    }
}

/*
Without inline and no flags: 5.18s user 0.03s system 99% cpu 5.210 total
With -O3 and no inline: 0.00s user 0.00s system 63% cpu 0.005 total

With -O2 and inline:  0.00s user 0.00s system 57% cpu 0.005 total
With -O2 and no inline: 0.00s user 0.00s system 56% cpu 0.006 total

DOes the inline make any difference? Or is it just the compiler?
*/