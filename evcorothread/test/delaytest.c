#include <evcorothread/evthread.h>
#include <corothread/stacks.h>

#include "vtest/vtest.h"
#include <sys/time.h>
#include <time.h>

// ------------------------------------------------------------------------------------

static void evthread_proc( EVTHREAD *thread, EVSOCKET *socket, void *user_data) 
{
  struct timeval tv;

  M_UNUSED( socket );
  M_UNUSED( user_data );

  tv.tv_sec = 3;
  tv.tv_usec = 0;

  EVTHREAD_delay( thread, tv );
  EVLOOP_break( thread->loop );
}

void EVTHREAD_timer_test()
{
  EVLOOP *loop;  
  EVTHREAD *thread;  
  int timer_counter;
  STACKS stacks;
  time_t t1,t2;
  

  time(&t1);

  VASSERT( !  CTHREAD_libinit() );
  
  VASSERT( !  STACKS_init( &stacks, 1 , 5 ) );

  VASSERT( loop = EVLOOP_init( &stacks ) );

  timer_counter = 0;

  VASSERT( thread = EVTHREAD_init( loop, evthread_proc, &timer_counter ) );

  VASSERT( ! EVTHREAD_start( thread, 0) );

  EVLOOP_run( loop );

  VASSERT( ! STACKS_destroy( &stacks ) ); 

  time(&t2);

  VASSERT( (t2 - t1) >= 3 );

}


