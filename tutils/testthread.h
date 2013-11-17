#ifndef __TEST_THREAD_H_
#define __TEST_THREAD_H_

#include "cbarrier.h"



/**
 @defgroup TEST_THREAD
 @ingroup  THREADPOOL

 @brief runs set of threads; is mainly used for test purposes.

 - runs a number of threads; each subsequent thread is assigned a different CPU affinity;
 - used during testing: if each thread is running on different CPU then problems are more likely to show up.
 - The test protocol.

 1) Initialize the TEST_THREAD object. (call TEST_THREAD_init)
      Here the following things are specified
	- number of test threads
	- the test procedure  ; pointer to context passed to test procedure
	- (optional) per thread initialization of test 
	- (optional) per thread cleanup
     
 2) start the test thread set, each test thread is initialized (call TEST_THREAD_run)
       the function waits until
       	  - start all test threads, each one with different CPU affinity.
	  - if per test initialization function has been given then call it for each test thread.
	  - wait till all threads reach synchronization point, the function returns when all threads have started the test.

 3) wait for all test threads to complete. (call TEST_THREAD_stop)

 @{
 */


/**
 * @brief test initialization callback; is called by each test thread once so that it can do per thread initializations.
 *
 * returns the per thread data of this test.
 *
 * @param  ctx global context passed to test
 * @param thread_idx the sequence number of the thread - enumeration 0..n
 * @param num_thread number of test threads
 */
typedef void  * (* TEST_FUNC_INIT) (void  *ctx, int thread_idx, int num_thread);

/**
 * @brief test run callback; is called by each test thread to perform the test
 *
 * @param  ctx global context passed to test
 * @param  thread_ctx (optional) per thread data
 * @param thread_idx the sequence number of the thread - enumeration 0..n
 * @param num_thread number of test threads
 */
typedef void    (* TEST_FUNC) (void *ctx, void *thread_ctx, int thread_idx, int num_thread );


typedef struct tagTEST_THREAD {
  CYCLIC_BARRIER start; 
  CYCLIC_BARRIER finish;
  CYCLIC_BARRIER cleanup;
   
  TEST_FUNC_INIT tfunc_init;
  TEST_FUNC	 tfunc_run;
  TEST_FUNC	 tfunc_cleanup;

  void *ctx;
  int num_threads;

} TEST_THREAD;

/**
 * @brief initialize the pool of test threads
 */
int TEST_THREAD_init (TEST_THREAD *thread, int num_threads,  TEST_FUNC_INIT test_init, TEST_FUNC test_func, TEST_FUNC cleanup, void *test_ctx );

/**
 * @brief run all threads that are part of the pool of test threads
 */
int TEST_THREAD_run( TEST_THREAD *thread );

#endif


