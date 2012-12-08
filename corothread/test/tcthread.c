#include <corothread/cthread.h>
#include "vtest/vtest.h"
#include <stdio.h>
#include <butils/logg.h>


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
  fprintf(stderr,"\tFinished fib loop\n");
}

void CTHREAD_test_fib()
{
  CTHREAD *th;
  VALUES *vals;
  int val,count;
  STACKS stacks;
 
  MLOG_init( MLOG_LEVEL_TRACE, MLOG_ACTION_CONSOLE, 0);
 
  VASSERT( !  CTHREAD_libinit() );
  
//#ifdef __CYGWIN__
VASSERT( ! STACKS_init_on_demand( &stacks, -1, 5 ) );
//#else
//  VASSERT( ! STACKS_init( &stacks, 1 , 5 ) );
//#endif  


  th = CTHREAD_init( &stacks, fibonaci_generator );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th, &vals, 0 ) == 0 );

  for(count = 0 ;CTHREAD_state(th) != CTHREAD_STATE_EXIT; count++) {
     VASSERT( VALUES_scan( vals, "%d", &val ) == 0 );
     fprintf(stderr,"\t->received fibonaci %d (count %d)\n", val, count );
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

  VASSERT( CTHREAD_start( th, 0, "%p", t ) != 0 );

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

  VASSERT( CTHREAD_start( th, 0, "%p", &param ) != 0 );
  
  VASSERT( param.level == 0 );
  VASSERT( param.has_second_level );

  VASSERT( ! CTHREAD_free( th ) );
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}

// -----------------------------------------------------------------
int test_simple2(int val)
{
  VALUES *msg;
  int inc;
  MLOG_TRACE( "> test_simple yield %d", val );
  VASSERT_RET( CTHREAD_yield( &msg, "%d", val ) == 0, -1 );
 
  VALUES_scan( msg, "%d", &inc );
  MLOG_TRACE( "> test_simple got %d", inc );
 
  val += inc;
 
  return val;
}

void  test_simple(VALUES *arg)
{
  int val;
#if 0  
  int inc;
  VALUES *msg;
#endif  
 
  VALUES_scan( arg, "%d", &val );
  MLOG_TRACE( "> test_simple got %d", val );
  val += 3;
   
#if 0
  MLOG_TRACE( "> test_simple yield %d", val );
  VASSERT( CTHREAD_yield( &msg, "%d", val ) == 0 );
 
  VALUES_scan( msg, "%d", &inc );
  MLOG_TRACE( "> test_simple got %d", inc );
 
  val += inc;
#else
  val = test_simple2( val );
#endif
 
  MLOG_TRACE( "> test_simple yield %d", val );
  VASSERT( CTHREAD_yield( 0 , "%d", val ) == 0 );

   
  MLOG_TRACE( "> test_simple exit" );

}

void CTHREAD_test_simple()
{
  VALUES  *rvals;
  STACKS stacks;
  CTHREAD *th;
  int res;
 
  MLOG_init( MLOG_LEVEL_TRACE, MLOG_ACTION_CONSOLE, 0);
 
  VASSERT( ! CTHREAD_libinit() );
  
  VASSERT( ! STACKS_init( &stacks, 1 , 5 ) );
//VASSERT( ! STACKS_init_on_demand( &stacks, -1, 5 ) );

  th = CTHREAD_init( &stacks, test_simple );
  VASSERT( th != 0 );

  VASSERT( CTHREAD_start( th, &rvals, "%d", 42 ) == 0 );
  
  VALUES_scan( rvals, "%d", &res );
  VASSERT( res == 45 );
  
  VASSERT( CTHREAD_resume( th, &rvals, "%d", 3 ) == 0 );  
   
  VALUES_scan( rvals, "%d", &res );

  VASSERT( res == 48 );
  
  VASSERT( CTHREAD_resume( th, 0, 0 ) != 0 );  
 
  VASSERT( CTHREAD_state( th ) == CTHREAD_STATE_EXIT );

  VASSERT( CTHREAD_free( th ) == 0 );
} 
