#include <corothread/cthread.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define NUM_STACKS 10000 
#define MAX_NUM 90


// -----------------------------------------------------------------
void  make_fibonacci_sequence(VALUES *values)
{
  int i, max_num;
  void *val;
  uint64_t a,b,n;

  VALUES_scan( values, "%d", &max_num );


  for(a = 0, b = 1, i = 0; i < max_num; i++) {
    n = a+b;
    
    CTHREAD_yield( 0, "%qu", n );
    
    a=b;
    b=n;
  }

}

double sum_of_roots( size_t max_num, STACKS *stacks ) 
{
  CTHREAD *th;
  VALUES *values;
  uint64_t fib;
  int i;
  double sum_roots = 0;

  th = CTHREAD_init( stacks, make_fibonacci_sequence );
  assert( th != 0 );

  assert( CTHREAD_start( th, &values, "%d", max_num ) == 0 );

  for( i = 0 ; i < max_num && CTHREAD_state(th) != CTHREAD_STATE_EXIT; i++) {
     
     VALUES_scan( values, "%qu", &fib );

     sum_roots += sqrt( (double) fib );
  
     CTHREAD_resume( th, &values, 0 );
  }

  assert( ! CTHREAD_free( th ) );
  return sum_roots;
}


int main()
{
  STACKS stacks;
  
  assert( !  CTHREAD_libinit() );
  
  assert( !  STACKS_init( &stacks, 1 , 5 ) );

  printf("Sum of roots of fibonaci numbers: %f\n", sum_of_roots( MAX_NUM, &stacks ) );

  assert( ! STACKS_destroy( &stacks ) ); 


  return 0;
}
