#include <iothreads/cthread.h>
#include "vtest/vtest.h"
#include <stdio.h>

#define NUM_STACKS 10000 
#define NUM_FIB 30

void  fibonaci_generator (void *arg)
{
  int a,b,n,i;

  M_UNUSED(arg);

  for(a = b = 1, i = 0; i < NUM_FIB; i++) {
    n = a+b;

    fprintf(stderr,"\tGenerate fibonaci %d\n",  n);
    CTHREAD_set_return_value( (void *) n );
    CTHREAD_yield();
    
    a=b;
    b=n;
  }

}

void CTHREAD_test()
{
  CTHREAD *th;
  void *val;
  int count;
  STACKS stacks;
  
  VASSERT( !  CTHREAD_libinit() );
  
  VASSERT( !  STACKS_init( &stacks, 1 , 5 ) );

  th = CTHREAD_init( &stacks, fibonaci_generator, 0 );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th ) == 0 );

  for(count = 0 ;CTHREAD_state(th) != CTHREAD_STATE_EXIT; count++) {
     VASSERT( CTHREAD_get_return_value( th, &val ) == 0 );
     fprintf(stderr,"\t->received fibonaci %d\n", (int) val );
     CTHREAD_resume( th );
  }

  fprintf(stderr," -- count %d\n", count);
  VASSERT( count == NUM_FIB );
  VASSERT( ! CTHREAD_free( th ) );
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}



