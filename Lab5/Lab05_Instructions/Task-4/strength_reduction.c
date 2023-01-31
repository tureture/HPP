#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   int i;
   unsigned int b = 2, c = 3, d = 1, a = 1;
   float x = 0.1, y = 0.5, z = 0.33;

   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);

   for (i=0; i<50000000; i++)
   {
      // c = d*2;
      c = d << 1;
      // b = c*15;
      b = c * (unsigned int)(15);
      // a = b/16;
      a = b >> 4;

      d = b/a; // changed all to unsigned int

      z = 0.33;
      y = 2*z;
      // x = y / 1.33;
      x = y * 0.75;
      // z = x / 1.33;
      z = x * 0.75;
   }
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);
   return 0;
}

/*
Time before: 0.394s total
Time after:  0.289s total

*/
