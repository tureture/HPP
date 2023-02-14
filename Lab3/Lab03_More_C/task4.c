#include <stdio.h>
#include <stdlib.h>


int comparator_double(const void* p1, const void* p2) {
    if (*(double*)p1 > *(double*)p2) return -1;
    if (*(double*)p1 < *(double*)p2) return 1;
    return 0;
}




int main(){
    double arrDouble[] = {9.3, -2.3, 1.2, -0.4, 2, 9.2, 1, 2.1, 0, -9.2};
    int arrlen=10;

    int (*comp)(const void*, const void*);

    comp = comparator_double;
    qsort (arrDouble, arrlen, sizeof(double), comp);

    for (int i=0; i<arrlen; i++) {
        printf("%f ", arrDouble[i]);
    }



    return 0;
}