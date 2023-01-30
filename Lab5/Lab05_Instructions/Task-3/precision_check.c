#include <stdio.h>
#include <stdlib.h>
# include <math.h>

int main(int argc, char **argv)
{
   int i;
   double n1 = 3;
   double n2 = 3;
   double eps = 0.001;

    // check when reaching inf, after 3.0000000000e+306 152 * 100, 2^1023 motsvarar typ 10^308
   for (int i=0; i<154; i++)
   {
    n1 *= 100;
    printf(" %.10e %i \n", n1, i); 
   }

    // check when getting NaN
    n2 = sqrt(-1);
    printf(" %.10e \n", n2);
    n2 += 100;
    printf(" %.10e \n", n2);
    // Stays Nan after doing operations

    // check when eps + 1 = 1
    for (int i=0; i<100; i++)
    {
        if (eps + 1 == 1)
        {
            printf(" %.10e %i \n", eps, i);
            break;
        }
        eps /= 10;
    }

    // circa 10^-16 precision? Matches with table
    






   return 0;
}
