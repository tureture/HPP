#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isPrime(int n);

int main()
{
    int n;
    int nr_primes = 0;
    printf("Nr inputs: ");
    scanf("%i", &n);

    int *list_in= (int *)malloc(n * sizeof(int));
    printf("Enter list of numbers: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%i", &list_in[i]);
    }

    int *list_out = (int *)malloc(5 * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        if (isPrime(list_in[i]) == 1)
        {
            if (nr_primes % 5 == 0)
            {
                list_out = realloc(list_out, (nr_primes + 5) * sizeof(int));
            }

            printf("%i ", list_in[i]);
            list_out[nr_primes] = list_in[i];
            nr_primes++;
        }
    }

    printf("\n Nr of primes: %i", nr_primes);

    


    return 0;
}

int isPrime(int n){
    for(int i=2; i <= sqrt(n); i++){
        if(n%i == 0){
            return 0;
        }
    }
    return 1;
}