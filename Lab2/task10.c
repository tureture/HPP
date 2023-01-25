#include <stdio.h>
#include <stdlib.h>

void print_array(int *arr, int n);

int main()
{
int *arr;
int n;
printf("Enter the number of elements: ");
scanf("%d", &n);
arr = (int *)malloc(n*sizeof(int));
for(int i=0; i<n; ++i){
    arr[i] = rand() % 100; // random number from 0 to 99
    } 
print_array(arr, n);
return 0; 
}

void print_array(int *arr, int n){
    for (int i=0; i<n; i++){
        printf("%i \n", *(arr + i)); // same as arr[i] just had to try
    }

}