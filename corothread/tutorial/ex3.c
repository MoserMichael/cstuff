#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUM 90

typedef void (*FIBONACCI_CB) (uint64_t num, void *ctx);

void generate_fibonacci_sequence( size_t num_size, FIBONACCI_CB callback, void *ctx ) 
{
  uint64_t a=0, b=1, n;
  size_t i;

  for(i=0; i < num_size; i++, a = b, b = n) {
    n = a + b;
    callback( n, ctx );
  } 
}

void fibonacci_cb( uint64_t fib_num, void *ctx) 
{
   double *fibb_sum = (double *) ctx;
   *fibb_sum += sqrt( (double) fib_num );
}

int main()
{
  uint64_t num[MAX_NUM];
  double result = 0;
  
  generate_fibonacci_sequence( MAX_NUM, fibonacci_cb, &result );
  printf("Sum of roots of fibonaci numbers: %f\n", result );

  return 0;
}

