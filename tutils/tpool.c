#include "tpool.h"
#include <butils/errorp.h>
#include <pthread.h>
#include <tstart.h>

//---

void RUNNABLE_init(RUNNABLE *runnable, RUNNABLE_HANDLER handler, RUNNABLE_HANDLER free_request  )
{
   runnable->handle_request = handler;
   runnable->free_request = free_request;  
}


void RUNNABLE_free(RUNNABLE *runnable )
{
  if (runnable->free_request) {
    runnable->free_request( runnable );
  }
  free( runnable );
}

//---

static void * worker_thread( void * arg)
{
  THREADPOOL *pool = (THREADPOOL *) arg;
  RUNNABLE *req;

  while ( (req = TQUEUE_pop( &pool->request_queue ) ) != 0 ) {
     
     req->handle_request( req );
     if (pool->process_result != 0) {
        pool->process_result( req ); 
     }
  }

  CYCLIC_BARRIER_await( &pool->all_finished );

  return 0;
}


THREADPOOL *THREADPOOL_init( RUNNABLE_HANDLER process_result, int queue_size, int num_threads, int stack_size_kb )
{
  THREADPOOL *pool;
  pthread_t pth;
  pthread_attr_t attr;
  int i, rt;

  pool = (THREADPOOL *) malloc( sizeof(THREADPOOL) );
  if (!pool) {
    return 0;
  }

  pool->process_result = process_result;
  pool->num_threads = num_threads;

  TQUEUE_init( &pool->request_queue, queue_size );

  pthread_attr_init( &attr );
  if (stack_size_kb > 0) {
    pthread_attr_setstacksize( &attr, stack_size_kb * 1024 );
  }    

  for( i = 0; i < num_threads; i++) {

     if ((rt = pthread_create_detached( &pth, &attr, worker_thread, pool ) )  != 0) {
        break;
     }
  }
  if ( i < num_threads) {
     errorp(rt, "Can't create thread # %d", i);
     CYCLIC_BARRIER_init( &pool->all_finished, i + 1 );
     THREADPOOL_close( pool );
     return 0;
  }

  CYCLIC_BARRIER_init( &pool->all_finished, num_threads + 1 );

  return pool;
}

void THREADPOOL_close( THREADPOOL *pool )
{
  int i;

  for( i=0; i < pool->num_threads; i++) {
    TQUEUE_push_exit_message( &pool->request_queue);
  }
  CYCLIC_BARRIER_await( &pool->all_finished );
  free(pool);
}

int THREADPOOL_send_block_on_queue_full( THREADPOOL *pool, RUNNABLE *request)
{
  return TQUEUE_push_block_on_queue_full( &pool->request_queue, request);
}

int THREADPOOL_send_fail_on_queue_full( THREADPOOL *pool, RUNNABLE *request)
{
  return TQUEUE_push_fail_on_queue_full( &pool->request_queue, request);
}





   

