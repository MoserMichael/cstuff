#include <corothread/cthread.h>
#include "vtest/vtest.h"
#include <stdio.h>

#define NUM_STACKS 10000 
#define NUM_FIB 30

// -----------------------------------------------------------------
void  fibonaci_generator (VALUES *arg)
{
  int a,b,n,i;

  M_UNUSED(arg);

  for(a = b = 1, i = 0; i < NUM_FIB; i++) {
    n = a+b;

    fprintf(stderr,"\tGenerate fibonaci %d\n",  n);
    CTHREAD_yield(0, "%d", n);
    
    a=b;
    b=n;
  }

}

void CTHREAD_test()
{
  CTHREAD *th;
  VALUES *vals;
  int val,count;
  STACKS stacks;
  
  VASSERT( !  CTHREAD_libinit() );
  
  VASSERT( !  STACKS_init( &stacks, 1 , 5 ) );

  th = CTHREAD_init( &stacks, fibonaci_generator );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th, &vals, 0 ) == 0 );

  for(count = 0 ;CTHREAD_state(th) != CTHREAD_STATE_EXIT; count++) {
     VASSERT( VALUES_scan( vals, "%d", &val ) == 0 );
     fprintf(stderr,"\t->received fibonaci %d\n", val );
     CTHREAD_resume( th, &vals, 0 );
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
 

void  second_level( VALUES *arg)
{
  tparam *t;
  
  VALUES_scan( arg, "%p", &t );

  t->level++;
  t->has_second_level = 1;
  t->level--;
}


void  first_level(VALUES *arg)
{
  tparam *t;
  CTHREAD *th;

  VALUES_scan( arg, "%p", &t );

  t->level++;

  th = CTHREAD_init( t->stacks, second_level );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th, 0, "%p", t ) == 0 );

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

  th = CTHREAD_init( &stacks, first_level );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th, 0, "%p", &param ) == 0 );
  
  VASSERT( param.level == 0 );
  VASSERT( param.has_second_level );

  VASSERT( ! CTHREAD_free( th ) );
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}
