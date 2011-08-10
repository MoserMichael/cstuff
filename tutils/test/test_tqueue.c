/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <tutils/tqueue.h>
#include <tutils/cbarrier.h>
#include <vtest/vtest.h>
#include <malloc.h>
#include <stdio.h>

#define THREAD_NUM 3

static TQUEUE rqueue;

static CYCLIC_BARRIER cbarrier;

static void * test_thread( void * arg)
{
  int64_t *mysum = (int64_t *) arg;
  void *p;

  *mysum = 0;


  while ( (p = TQUEUE_pop( &rqueue ) ) != 0 ) {

    *mysum += * ((int64_t *) p );
    free(p);
  }

  CYCLIC_BARRIER_await( &cbarrier );

  return 0;
}

void TQUEUE_test()
{
  TQUEUE_init(&rqueue, 3 );
  pthread_t th[ THREAD_NUM ];
  int i;
  int64_t *msg;
  int64_t sum[THREAD_NUM], count;


  CYCLIC_BARRIER_init( &cbarrier, THREAD_NUM  + 1 );

  for( i=0; i < THREAD_NUM; i++) {
    pthread_create( &th[ i ], 0, test_thread, &sum[i] );
  }

  for( i = 1; i <= 1000000; i++ ) { // 10000
     msg = (int64_t *) malloc(sizeof( int64_t ));
     msg[0] = i;

     TQUEUE_push_block_on_queue_full( &rqueue, msg );
  }

  for( i=0; i < THREAD_NUM; i++) {
     TQUEUE_push_block_on_queue_full( &rqueue, 0 );
  }
  CYCLIC_BARRIER_await( &cbarrier );
  
  for( count = i = 0; i < THREAD_NUM; i++) {
    count += sum[ i ] ;
  }

  fprintf(stderr,"%lld", count );
  VASSERT( count == 500000500000LL );  //50005000  
}



