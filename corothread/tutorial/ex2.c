#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUM 90


void make_fibonacci_sequence( uint64_t *fib, size_t num_fib ) 
{
  uint64_t a=0, b=1, n;
  size_t i;

  for(i=0; i < num_fib; i++, a = b, b = n) {
    n = a + b;
    fib[ i ] = n;
  } 
}

double sum_of_roots( uint64_t *num, size_t num_size ) 
{
  size_t i;
  double sum_roots = 0;

  for( i = 0; i < num_size; i++) {
    sum_roots += sqrt( (double) num[i] );
  }
  return sum_roots;
}

int main()
{
  uint64_t num[MAX_NUM];
  
  make_fibonacci_sequence( num, MAX_NUM );
  printf("Sum of roots of fibonaci numbers: %f\n", sum_of_roots( num, MAX_NUM ) );

  return 0;
}

