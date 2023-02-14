#include <stdio.h>
#include <stdlib.h>

void print_array(int *arr, int n);

int main()
{
    int n = 0;
    int x = 0;
    int in;

    int *arr = (int *)malloc(3 * sizeof(int));

    printf("\n Enter list of numbers: ");

    while (scanf("%d", &in) != 0) {

        if (n % 10 == 0) {
           arr = realloc(arr, (n+10) * sizeof(int));
        }
        arr[n] = in;
        n++;

    }


    for(int i=0; i<n; ++i){
        printf("%d ", arr[i]);
        }

    free(arr);

    return 0;
}