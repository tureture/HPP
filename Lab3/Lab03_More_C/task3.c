#include <stdio.h>
#include <stdlib.h>


void print_int_1(int x) {
  printf("Here is the number: %d\n", x);
}

void print_int_2(int x) {
  printf("Wow, %d is really an impressive number!\n", x);
}


int main(){
    int n = 5;
    double a = 32.78;

    void (*funPointer)(int);

    funPointer = print_int_1;
    funPointer(n);

    funPointer = print_int_2;
    funPointer(n);

    return 0;
}