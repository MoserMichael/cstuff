/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <tutils/cbarrier.h>
#include <vtest/vtest.h>
#include <unistd.h>

#define THREAD_NUM  50

static int counter;
static pthread_mutex_t counter_mutex  = PTHREAD_MUTEX_INITIALIZER;

static void * test_thread( void * arg)
{
  CYCLIC_BARRIER *cb =(CYCLIC_BARRIER *) arg;

  sleep(1);

  pthread_mutex_lock( &counter_mutex );
  counter++;
  pthread_mutex_unlock( &counter_mutex );
   
  CYCLIC_BARRIER_await(cb);

  return 0;
}

void TBARRIER_test()
{
  pthread_t th[ THREAD_NUM ];
  CYCLIC_BARRIER cbarrier;
  int i;

  CYCLIC_BARRIER_init( &cbarrier, THREAD_NUM  + 1 );

  for( i=0; i < THREAD_NUM; i++) {
    VASSERT( pthread_create( &th[ i ], 0, test_thread, &cbarrier ) == 0 );
  }

  CYCLIC_BARRIER_await( &cbarrier );

  VASSERT( counter == THREAD_NUM );

  VASSERT( CYCLIC_BARRIER_free( &cbarrier ) == 0 );
}

