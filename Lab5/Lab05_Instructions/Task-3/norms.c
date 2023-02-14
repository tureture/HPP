#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   int i;
   float tiny = 1e-35;

   printf("%e\n", tiny);

   /* Inside the loop we do some computations modifying the "tiny"
      variable in such a way that it is (almost) restored to the same
      value again in each loop iteration. The value will probably
      change slightly due to rounding errors. */
   for (i=0; i<5000000; i++)
   {
      tiny += tiny/2.0;
      tiny *= 0.666667;
      tiny += tiny/2.0;
      tiny *= 0.666667;
      tiny += tiny/2.0;
      tiny *= 0.666666;
   }
   printf("%e\n", tiny);

   return 0;
}

/*
On my computer no difference withour compiler flags
0.23s user 0.00s system 99% cpu 0.237 total
0.23s user 0.00s system 99% cpu 0.237 total

With -O2:
nroms: 0.13s user 0.00s system 98% cpu 0.133 total
denorms:  0.13s user 0.00s system 98% cpu 0.133 total

With -O3:
norms: 0.13s user 0.00s system 98% cpu 0.135 total
denorms: 0.13s user 0.00s system 98% cpu 0.135 total

With -Ofast:
denorms: 0.10s user 0.00s system 96% cpu 0.106 total
norms: 0.10s user 0.00s system 97% cpu 0.105 total

Should perhaps check on linux computers?
*/