#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    int n;  
    char filename[50];

    typedef struct product
    {
    char name[50];
    double price;
    } 
    product_t;

    strcpy(filename, argv[1]);
    FILE *fp = fopen(filename, "r");

    fscanf(fp, "%i", &n);
    product_t arr_of_prod[n];

    for (int i = 0; i < n; i++){
        fscanf(fp, "%s %lf", arr_of_prod[i].name, &arr_of_prod[i].price);
        printf("%s %lf \n", arr_of_prod[i].name, arr_of_prod[i].price);
    }



    return 0;
}