#include <stdio.h>
#include <stdlib.h>

#define N 64

typedef struct {
  long int v[N];
} StructType;

void SetToZero(StructType* p) {
  int i;
  for(i = 0; i < N; i++)
    p->v[i] = 0;
}

long int GetSum(StructType* p) {
  int i;
  long int sum = 0;
  for(i = 0; i < N; i+=1)
    sum += p->v[i];
  return sum;
}

void ModifyWithStep8(StructType* p, long int a) {
  p->v[0]  += a;
  p->v[8]  += a;
  p->v[16] += a;
  p->v[24] += a;
  p->v[32] += a;
  p->v[40] += a;
  p->v[48] += a;
  p->v[56] += a;
}

void ModifyLow(StructType* p, long int a) {
  p->v[0]  += a;
  p->v[1]  += a;
  p->v[2]  += a;
  p->v[3]  += a;
  p->v[4]  += a;
  p->v[5]  += a;
  p->v[6]  += a;
  p->v[7]  += a;
}

int main() {
  int n = 200;
  StructType* arrOfStructs;
  int nBytesToAllocate = n*sizeof(StructType);
  printf("nBytesToAllocate = %d\n", nBytesToAllocate);
  arrOfStructs = (StructType*)malloc(nBytesToAllocate);
  int m = 500000;
  long int a;
  a = 1 + (((long int)rand()*3)/RAND_MAX);
  printf("a = %ld\n", a);
  int i, j;
  for(j = 0; j < n; j++)
    SetToZero(&arrOfStructs[j]);
#if 1
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      ModifyWithStep8(&arrOfStructs[j], a);
#else
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      ModifyLow(&arrOfStructs[j], a);
#endif
  long int sum = 0;
  for(j = 0; j < n; j++)
    sum += GetSum(&arrOfStructs[j]);
  printf("sum = %ld\n", sum);
  free(arrOfStructs);
  return 0;
}

/*
With no changes:
ModifyLow: 0.13s user 0.00s system 98% cpu 0.140 total
Step8: 0.24s user 0.00s system 99% cpu 0.243 total

With n / 2
ModifyLow: 0.13s user 0.00s system 98% cpu 0.138 total
Step8: 0.23s user 0.00s system 98% cpu 0.237 total

With n / 8
ModifyLow: 0.13s user 0.00s system 98% cpu 0.132 total
Step8: 0.23s user 0.00s system 99% cpu 0.240 total

With n/20, (n=10)
ModifyLow: 0.06s user 0.00s system 97% cpu 0.066 total, why is modifylow so much faster?
Step8: 0.23s user 0.00s system 99% cpu 0.238 total

With n/40, 2560 bytes
Modifylow: 0.08s user 0.00s system 97% cpu 0.079 total
Step8: 0.07s user 0.00s system 97% cpu 0.070 total, now step8 

With n*2
Step8: 0.60s user 0.00s system 99% cpu 0.603 total

With n*10
ModifyLow: 0.18s user 0.00s system 98% cpu 0.186 total
Step8: 0.59s user 0.01s system 99% cpu 0.601 total

With n=1
ModifyLow: 0.00s user 0.00s system 61% cpu 0.005 total
Step8: 0.00s user 0.00s system 58% cpu 0.005 total

Google says M2 chip has either 196+128 KB per core or 128+64 KB per core
for performance and efficency cores respectively. 

Biggest change seems to be between n*2 and n for Step8, should correspond
to change from around 102kb to 204kb 


*/
