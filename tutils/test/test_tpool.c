
#include <tutils/tpool.h>
#include <vtest/vtest.h>
#include <malloc.h>
#include <stdio.h>

#define THREAD_NUM 3

static TQUEUE rqueue;

typedef struct tagADDER
{ 
  RUNNABLE base;
  int arg;
  int res;
} ADDER;

void do_adder(ADDER *arg)
{
   arg->res = arg->arg + 1;
}

void handle_response (RUNNABLE *request, void *tcontext, void *poolctx)
{
  (void) tcontext;
  (void) poolctx;

  VASSERT( TQUEUE_push_fail_on_queue_full( &rqueue, request ) == 0 );
}

#define NUM_REQUESTS 10000

void TPOOL_test()
{
  int i;
  int *res_numbers;
  THREADPOOL *pool;
  ADDER *tmp;


  TQUEUE_init(&rqueue, -1 );
  
  pool = THREADPOOL_init_ext( handle_response , NULL, NULL,  100, 10, 100, NULL );
  

  for(i = 0; i < NUM_REQUESTS; i++ ) {
     tmp = (ADDER *) malloc( sizeof(ADDER) );
     RUNNABLE_init( &tmp->base, (RUNNABLE_HANDLER) do_adder, 0 );
     tmp->arg = i;

     THREADPOOL_send_block_on_queue_full( pool, &tmp->base );
  }

   res_numbers  = (int *) malloc(  NUM_REQUESTS * sizeof(int) );
   for(i = 0; i <  NUM_REQUESTS;  i++ ) {
     res_numbers[ i ] = 0;
   }
   


   for(i = 0; i <  NUM_REQUESTS;  i++ ) {
     tmp = (ADDER *) TQUEUE_pop( &rqueue );
     VASSERT( tmp->res > 0 && tmp->res <= NUM_REQUESTS );
     res_numbers[ tmp->res - 1 ] = tmp->res;
     RUNNABLE_free( &tmp->base, NULL, NULL );
   }

   for(i = 0; i <  NUM_REQUESTS;  i++ ) {
     VASSERT( res_numbers[ i ] == (i + 1 ) );
   }
 
   THREADPOOL_close( pool );
}  

