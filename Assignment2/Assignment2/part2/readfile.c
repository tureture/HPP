#include <stdio.h>
#include <stdlib.h>

/*
Assignment 2 HPP, part2
Ture Hassler & Jacob Malmenstedt
*/


int main() {

    // initialize variables and file pointer
    FILE *file;
    int i;
    double d;
    char c;
    float f;

    file = fopen("little_bin_file", "r"); // open file 

    // read stuff
    fread(&i, sizeof(int), 1, file);
    fread(&d, sizeof(double), 1, file);
    fread(&c, sizeof(char), 1, file);
    fread(&f, sizeof(float), 1, file);

    fclose(file); // close file

    // print stuff
    printf("%d \n %f \n %c \n %f \n", i, d, c, f);

  return 0;
}

