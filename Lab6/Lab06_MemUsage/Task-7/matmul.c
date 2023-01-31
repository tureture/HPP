#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 
static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
int rand_int(int N)
{
  int val = -1;
  while( val < 0 || val >= N )
    {
      val = (int)(N * (double)rand()/RAND_MAX);
    }
  return val;
}

void allocate_mem(int*** arr, int n)
{
  int i;
  *arr = (int**)malloc(n*sizeof(int*));
  for(i=0; i<n; i++)
    (*arr)[i] = (int*)malloc(n*sizeof(int));
}

void free_mem(int** arr, int n)
{
  int i;
  for(i=0; i<n; i++)
    free(arr[i]);
  free(arr);
}

/* kij */
void mul_kij(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (k=0; k<n; k++) {
    for (i=0; i<n; i++) {
      int x = a[i][k];
      for (j=0; j<n; j++)
	c[i][j] += x * b[k][j];   
    }
  }
}

/* kji */
void mul_kji(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (k=0; k<n; k++) {
    for (j=0; j<n; j++) {
      int x = a[j][k];
      for (i=0; i<n; i++)
	c[j][i] += x * b[k][i];   
    }
  }
}


/* ijk */
void mul_ijk(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (i=0; i<n; i++)  {
    for (j=0; j<n; j++) {
      int sum = 0;
      for (k=0; k<n; k++) 
	sum += a[i][k] * b[k][j];
      c[i][j] = sum;
    }
  }
}

/* jik */
void mul_jik(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (j=0; j<n; j++) {
    for (i=0; i<n; i++) {
      int sum = 0;
      for (k=0; k<n; k++)
	sum += a[i][k] * b[k][j];
      c[i][j] = sum;
    }
  }
}

/* ikj */
void mul_ikj(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (i=0; i<n; i++)  {
    for (k=0; k<n; k++) {
      int sum = 0;
      for (j=0; j<n; j++) 
	sum += a[i][j] * b[j][k];
      c[i][k] = sum;
    }
  }
}

/* jki */
void mul_jki(int n, int **a, int **b, int **c)
{
  int i, j, k;
  for (j=0; j<n; j++) {
    for (k=0; k<n; k++) {
      int x = a[k][j];
      for (i=0; i<n; i++)
	c[k][i] += x * b[j][i];   
    }
  }
}

/* kij + row and col cache blocking  */
void mul_kij_cache(int n, int bSize, int **a, int **b, int **c)
{
  int nn = n / bSize; // only works if evenly divisible?
  int i, j, k, kb, jb, ib, ks, js, is, x;

 // first part loops over blocks
  for (kb=0; kb<nn; kb++) {
    ks = kb * bSize;
    for (ib=0; ib<nn; ib++) {
      is = ib * bSize;
      for (jb=0; jb<nn; jb++) {
        js = jb * bSize;

        // second part does multiplication of block
        for (k=ks; k<ks+bSize; k++) {
          for (i=is; i<is+bSize; i++) {
            x = a[i][k];
            for (j=js; j<js+bSize; j++)
	            c[i][j] += x * b[k][j];
            }     
        }  
      }  
    }
  }
}

void mul_kij_row_cache(int n, int bSize, int **a, int **b, int **c)
{
  int nn = n / bSize; // only works if evenly divisible?
  int i, j, k, kb, jb, ib, ks, js, is, x;

 // first part loops over blocks
  for (kb=0; kb<nn; kb++) {
    ks = kb * bSize;
    for (ib=0; ib<nn; ib++) {
      is = ib * bSize;

        // second part does multiplication of block
        for (k=ks; k<ks+bSize; k++) {
          for (i=is; i<is+bSize; i++) {
            x = a[i][k];
            for (j=0; j<n; j++)
	            c[i][j] += x * b[k][j];
            }     
        }  
      }  
    }
  }


int main()
{
  int blockSize = 4096;
  int i, j, n;
  int **a;
  int **b;
  int **c; 
  double time;
  int Nmax = 100; // random numbers in [0, N]

  printf("Enter the dimension of matrices n = ");
  if(scanf("%d", &n) != 1) {
    printf("Error in scanf.\n");
    return -1;
  }

  allocate_mem(&a, n);

  for ( i = 0 ; i < n ; i++ )
    for ( j = 0 ; j < n ; j++ )
      a[i][j] = rand_int(Nmax);

  allocate_mem(&b, n);
 
  for ( i = 0 ; i < n ; i++ )
    for ( j = 0 ; j < n ; j++ )
      b[i][j] = rand_int(Nmax);

  allocate_mem(&c, n);

  time=get_wall_seconds();
  mul_kij(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version kij, time = %f\n",time);


  /*
  time=get_wall_seconds();
  mul_ijk(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version ijk, time = %f\n",time);
  time=get_wall_seconds();
  mul_jik(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version jik, time = %f\n",time);
  time=get_wall_seconds();
  mul_kji(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version kji, time = %f\n",time);
  time=get_wall_seconds();
  mul_ikj(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version ikj, time = %f\n",time);
  time=get_wall_seconds();
  mul_jki(n, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version jki, time = %f\n",time);
  time=get_wall_seconds();
  */

 /*
   time=get_wall_seconds();
  mul_kij_cache(n, blockSize, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version kij cache optimized, time = %f\n",time);

 */

  time=get_wall_seconds();
  mul_kij_row_cache(n, blockSize, a, b, c);
  time=get_wall_seconds()-time;
  printf("Version kij row cache optimized, time = %f\n",time);
  

  /*
    printf("Product of entered matrices:\n");
 
    for ( i = 0 ; i < n ; i++ )
    {
    for ( j = 0 ; j < n ; j++ )
    printf("%d\t", c[i][j]);
 
    printf("\n");
    }
  */

  free_mem(a, n);
  free_mem(b, n);
  free_mem(c, n);

  return 0;
}

/*
kij order is signifcantly faster,

ex: 
Enter the dimension of matrices n = 1000  
Version kij, time = 0.109987
Version ijk, time = 0.813195
Version jik, time = 0.854558

Enter the dimension of matrices n = 1000
Version kij, time = 0.105748
Version ijk, time = 0.818891
Version jik, time = 0.866735
Version kji, time = 0.092247
Version ikj, time = 0.844697
Version jki, time = 0.091762

Enter the dimension of matrices n = 2000
Version kij, time = 0.716751
Version ijk, time = 17.130947
Version jik, time = 17.444234
Version kji, time = 0.735117
Version ikj, time = 17.137000
Version jki, time = 0.735159

Enter the dimension of matrices n = 2500
Version kij, time = 1.510638
Version ijk, time = 11.952213
Version jik, time = 10.626637
Version kji, time = 1.534601
Version ikj, time = 12.224747
Version jki, time = 1.537308

For best performance we want the innermost loop to move through memory contiguously.
This means moving along b in the inner loop is best for array[a][b]
This is the case for kij, kji and jki

kij was best for me. 

says block size but should be nr blocks?

After cache opmization, 
Enter the dimension of matrices n = 4096
Version kij, time = 8.649599
Version kij cache optimized, time = 8.437410
Version kij row cache optimized, time = 6.306923

Enter the dimension of matrices n = 4096
blockSize = 32;
Version kij, time = 8.803598
Version kij cache optimized, time = 46.799572 // bad dont use this
Version kij row cache optimized, time = 6.275225

Enter the dimension of matrices n = 4096
blockSize = 8;
Version kij, time = 8.862440
Version kij row cache optimized, time = 6.176953

blocksize = 256
Version kij, time = 8.852581
Version kij row cache optimized, time = 6.324473

blocksize = 1
Enter the dimension of matrices n = 4096
Version kij, time = 8.668251
Version kij row cache optimized, time = 8.741147

blocksize = 64 
Enter the dimension of matrices n = 4096
Version kij, time = 8.666696
Version kij row cache optimized, time = 6.251311

blocksieze = 4,
Version kij, time = 8.802313
Version kij row cache optimized, time = 5.281085

bsize = 16
Version kij, time = 8.816733
Version kij row cache optimized, time = 6.549175

with bsize=4096
Enter the dimension of matrices n = 4096
Version kij, time = 8.694081
Version kij row cache optimized, time = 8.957738
slower, makes sense. 

Should check with valgrind?

*/
