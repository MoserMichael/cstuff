#include <corothread/cthread.h>
#include "vtest/vtest.h"
#include <stdio.h>

#define NUM_STACKS 10000 
#define NUM_FIB 30

// -----------------------------------------------------------------
void  fibonaci_generator (void *arg)
{
  int a,b,n,i;

  M_UNUSED(arg);

  for(a = b = 1, i = 0; i < NUM_FIB; i++) {
    n = a+b;

    fprintf(stderr,"\tGenerate fibonaci %d\n",  n);
    CTHREAD_set_thread2caller( (void *) n );
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
     VASSERT( CTHREAD_get_thread2caller( th, &val ) == 0 );
     fprintf(stderr,"\t->received fibonaci %d\n", (int) val );
     CTHREAD_resume( th );
  }

  fprintf(stderr," -- count %d\n", count);
  VASSERT( count == NUM_FIB );
  
  VASSERT( ! CTHREAD_free( th ) );
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}


// -----------------------------------------------------------------
typedef struct tagtparam {
  int level;
  int has_second_level;
  STACKS *stacks;
} tparam;
 

void  second_level(void *arg)
{
  tparam *t = (tparam *) arg;

  t->level++;
  t->has_second_level = 1;
  t->level--;
}


void  first_level(void *arg)
{
  tparam *t = (tparam *) arg;
  CTHREAD *th;

  t->level++;

  th = CTHREAD_init( t->stacks, second_level, arg );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th ) == 0 );

  VASSERT( ! CTHREAD_free( th ) );
  
  t->level--;
}

 void CTHREAD_test_nested()
{
  CTHREAD *th;
  STACKS stacks;
  tparam param;
  
  VASSERT( !  CTHREAD_libinit() );
  
  VASSERT( !  STACKS_init( &stacks, 2 , 5 ) );

  param.level = 0;
  param.has_second_level = 0;
  param.stacks = &stacks;

  th = CTHREAD_init( &stacks, first_level, &param );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th ) == 0 );

  VASSERT( ! CTHREAD_free( th ) );
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}
