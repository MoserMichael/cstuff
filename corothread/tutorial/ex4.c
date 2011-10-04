#include <corothread/cthread.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define NUM_STACKS 10000 
#define MAX_NUM 90


// -----------------------------------------------------------------
void  make_fibonacci_sequence(void *arg)
{
  int i;
  void *val;
  uint64_t a,b,n;

  M_UNUSED(arg);

  CTHREAD_get_caller2thread( &val );


  for(a = 0, b = 1, i = 0; i < 90; i++) {
    n = a+b;
    printf("->%llu\n", n );
    CTHREAD_set_thread2caller( &n );
    CTHREAD_yield();
    
    a=b;
    b=n;
  }

}

double sum_of_roots( size_t max_num, STACKS *stacks ) 
{
  CTHREAD *th;
  void *val;
  uint64_t fib;
  int i;
  double sum_roots = 0;

  th = CTHREAD_init( stacks, make_fibonacci_sequence, 0 );
  assert( th != 0 );

  assert( CTHREAD_start( th ) == 0 );

  for( i = 0 ; i < max_num && CTHREAD_state(th) != CTHREAD_STATE_EXIT; i++) {
     
     assert( CTHREAD_get_thread2caller( th, &val ) == 0 );
     fib =  * ((uint64_t *) val);
     printf("<-%llu\n",fib);
     sum_roots += sqrt( (double) fib );
  
     CTHREAD_resume( th );
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
