/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <tutils/cbarrier.h>
#include <tutils/tstart.h>
#include <vtest/vtest.h>
#include <unistd.h>
#include <stdio.h>

#define THREAD_NUM  50

static int counter;
//static pthread_mutex_t counter_mutex  = PTHREAD_MUTEX_INITIALIZER;

static void * test_thread( void * arg)
{
  CYCLIC_BARRIER *cb =(CYCLIC_BARRIER *) arg;

  __sync_fetch_and_add( &counter, 1 );
  
#if 0
  pthread_mutex_lock( &counter_mutex );
  
  __sync_synchronize();
  counter++;
 __sync_synchronize();
  pthread_mutex_unlock( &counter_mutex );
#endif

  CYCLIC_BARRIER_await(cb);

  return 0;
}

void TBARRIER_test()
{
  pthread_t th[ THREAD_NUM ];
  CYCLIC_BARRIER cbarrier;
  int i;

  __sync_synchronize();
  counter = 0;
 __sync_synchronize();

  CYCLIC_BARRIER_init( &cbarrier, THREAD_NUM  + 1 );

  for( i=0; i < THREAD_NUM; i++) {
    VASSERT( pthread_create_detached( &th[ i ], 0, test_thread, &cbarrier ) == 0 );
  }

  CYCLIC_BARRIER_await( &cbarrier );

  __sync_synchronize();
 if (counter != THREAD_NUM ) {
      fprintf(stderr,"Counter %d tnum %d\n", counter ,THREAD_NUM);
      VASSERT( counter == THREAD_NUM );
  }      

  VASSERT( CYCLIC_BARRIER_free( &cbarrier ) == 0 );
}

