#ifndef __TEST_THREAD_H_
#define __TEST_THREAD_H_

#include "cbarrier.h"

typedef void  * (* TEST_FUNC_INIT) (void  *ctx);
typedef void    (* TEST_FUNC) (void *ctx, void *thread_ctx );


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

int TEST_THREAD_init (TEST_THREAD *thread, int num_threads,  TEST_FUNC_INIT test_init, TEST_FUNC test_func, TEST_FUNC cleanup, void *test_ctx );

int TEST_THREAD_run( TEST_THREAD *thread );

#endif


