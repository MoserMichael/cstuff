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

  // retrieve argument passed to coroutine - how many iterations to run.
  VALUES_scan( values, "%d", &max_num );


  for(a = 0, b = 1, i = 0; i < max_num; i++) {
    n = a+b;
  
    // now that a new fibonacci value has been computed, it can be passed to the caller;
    // the next function switches stacks from coroutine back to the caller.  
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

  // initialise the coroutine thread
  th = CTHREAD_init( stacks, make_fibonacci_sequence );
  assert( th != 0 );

  // start coroutine. pass the value of max_num to the coroutine.
  assert( CTHREAD_start( th, &values, "%d", max_num ) == 0 );

  // on return from CTHREAD_start, the return value from the coroutine is now stored in the values object.

  for( i = 0 ; i < max_num && CTHREAD_state(th) != CTHREAD_STATE_EXIT; i++) {
     
     VALUES_scan( values, "%qu", &fib );

     sum_roots += sqrt( (double) fib );

     // resume the coroutine thread, switch stack to coroutine thread.  
     CTHREAD_resume( th, &values, 0 );

     // on return from CTHREAD_resume, the return value from the coroutine is now stored in the values object.

  }

  // free coroutine object.
  assert( ! CTHREAD_free( th ) );
  return sum_roots;
}


int main()
{
  STACKS stacks;
  
  assert( !  CTHREAD_libinit() );
  
  // initialise the pool of stacks - here one stack that takes five memory pages is allocated.
  assert( !  STACKS_init( &stacks, 1 , 5 ) );

  printf("Sum of roots of fibonaci numbers: %f\n", sum_of_roots( MAX_NUM, &stacks ) );

  // release pool of stacks
  assert( ! STACKS_destroy( &stacks ) ); 

  return 0;
}
