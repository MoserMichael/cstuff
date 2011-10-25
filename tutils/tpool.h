#ifndef __TPOOL_H__
#define __TPOOL_H__

#include <tutils/tqueue.h>
#include <tutils/cbarrier.h>

struct tagRUNNABLE;
struct tagTHREADPOOL;

//---

typedef void (*RUNNABLE_HANDLER) (struct tagRUNNABLE *request);


/**
  @defgroup RUNNABLE

  @brief an interface to run a unit of work.

  Acts as a work request for thread pool
  Includes a callback that is invoked in order to process the request.

  Very similar to java concept of java.lang.Runnable.

  @{ 
 */
typedef struct tagRUNNABLE {
  RUNNABLE_HANDLER handle_request;
  RUNNABLE_HANDLER free_request;
} RUNNABLE;

/**
 @brief constructs a RUNNABLE instance
 */
void RUNNABLE_init(RUNNABLE *runnable, RUNNABLE_HANDLER handler, RUNNABLE_HANDLER free_request );

/**
 @brief free a RUNNABLE instance
 */
void RUNNABLE_free(RUNNABLE *runnable );

/**
 @}
 */

// ---

/**

 @defgroup THREADPOOL
 
 @brief a thread pool with fixed number of worker threads.

 The pool has a request queue: New requests are enqueued into it, the worker threads dequeue a work request and invoke the 'run' method of work request.

 One creation of thread pool the following parameters are specified
   - number of worker threads in thread pool.
   - maximum number of entries in request queue (or -1 if number of requests is unlimited).

 It is very important to set a reasonable size limit on the request queue;
 An unlimited queue can often lead to out of memory conditions: If work requests
 arrive at a rate faster than what can be processed, a bounded request buffer guards against a situation where the system is run at a load rate that is higher then it's capacity.
 
 In this situation the request queue acts as a bounded buffer; it should 
 be able to account for temporary fluctuations of the load; it can accomodate temporary peaks (i.e. short periods of time when the load is higher then the peak load), on condition that the request load later falls back to normal.

 You might find this class similar to java class java.util.concurrent.ThreadPoolExicutor when used with a thread pool size.

 @{
 */
typedef struct tagTHREADPOOL {
  int num_threads;
  TQUEUE request_queue;
  CYCLIC_BARRIER all_finished;
  RUNNABLE_HANDLER process_result;

} THREADPOOL;

/**
 @brief constructs a thread pool and starts it.

 The method 
   - creates request queue 
   - runs fixed number of worker threads 

 The worker thread 
   - fetches a work request from the request queue
   - invokes the handle_request callback stpred in the work request 
   - if process_result callback has been registered: invokes process_result
     callback (in order to have common policy of processing the results).

 @param process_result  a callback that receives work request after invoking it's run method. Value can be 0 (in this case it is not invoked)
 @param queue_size limit on the request queue (for details see THREADPOOL )
 @param num_threads number of worker threads
 @param stack_size_kb Size of thread pool stack in kilobytes (-1 if default stack size)

 */
THREADPOOL *THREADPOOL_init( RUNNABLE_HANDLER process_result, int queue_size, int num_threads, int stack_size_kb );

/*
 @brief shuts down the thread pool and frees it.

 Shuts down the thread pool.
 - Requests shut down of worker threads. Please not that all requests queued at the time of this call will be discarded.
 - Blocks untill the last worker thread has finished.

 */
void	    THREADPOOL_close( THREADPOOL *pool );

/**
 @brief posts a work request to the pool; blocks if request queue limit is reached

 A request is always queued. If request queue limit is reached then this method blocks  until the size of the queue falls back, In this case the request is enqueued and this method returns.

Returns 0 on success, -1 on failure (can't allocate memory for request)
 */
int	    THREADPOOL_send_block_on_queue_full( THREADPOOL *pool, RUNNABLE *request);

/**
 @brief posts a work request to the pool; blocks if request queue limit is reached

 A request is queued if the request queue did not reach its size limit. 
 If request queue limit is reached then this method returns an error.

Returns 0 on success, -1 on failure 
 */

int	    THREADPOOL_send_fail_on_queue_full( THREADPOOL *pool, RUNNABLE *request);

/**
  @}
 */
 

#endif


