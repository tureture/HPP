#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
    int n;  
    char product[50];
    char filename[50];
    double price;

    strcpy(filename, argv[1]);
    FILE *fp = fopen(filename, "r");



    fscanf(fp, "%i", &n);

    for (int i = 0; i < n; i++){
        fscanf(fp, "%s %lf", product, &price);
        printf("%s %lf \n", product, price);
    }



    return 0;
}

