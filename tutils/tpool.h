#ifndef __TPOOL_H__
#define __TPOOL_H__

#include <tutils/tqueue.h>
#include <tutils/cbarrier.h>

struct tagRUNNABLE;
struct tagTHREADPOOL;

//---

typedef void (*RUNNABLE_HANDLER) (struct tagRUNNABLE *request);

typedef struct tagRUNNABLE {
  RUNNABLE_HANDLER handle_request;
} RUNNABLE;

void RUNNABLE_init(RUNNABLE *runnable, RUNNABLE_HANDLER handler);

// ---

typedef struct tagTHREADPOOL {
  int num_threads;
  TQUEUE request_queue;
  CYCLIC_BARRIER all_finished;
  RUNNABLE_HANDLER process_result;

} THREADPOOL;

THREADPOOL *THREADPOOL_init( RUNNABLE_HANDLER process_result, int queue_size, int num_threads, int stack_size_kb );
void	    THREADPOOL_close( THREADPOOL *pool );
int	    THREADPOOL_send_block_on_queue_full( THREADPOOL *pool, RUNNABLE *request);
int	    THREADPOOL_send_fail_on_queue_full( THREADPOOL *pool, RUNNABLE *request);

#endif


