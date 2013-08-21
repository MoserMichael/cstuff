#define _GNU_SOURCE 
#include <pthread.h>
#include <sched.h>

#include "testthread.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int TEST_THREAD_init (TEST_THREAD *thread, int num_threads,  TEST_FUNC_INIT test_init, TEST_FUNC test_func, TEST_FUNC cleanup, void *test_ctx )
{
    thread->num_threads = num_threads;
    thread->ctx = test_ctx;

    thread->tfunc_init = test_init;
    thread->tfunc_run  = test_func;
    thread->tfunc_cleanup = cleanup;

    if ( CYCLIC_BARRIER_init( &thread->start, num_threads + 1 ) || 
	 CYCLIC_BARRIER_init( &thread->finish, num_threads + 1 ) ||
         CYCLIC_BARRIER_init( &thread->cleanup, num_threads + 1 ) )
      return -1;
	 
    return 0;
}

//---

static void * test_thread( void * arg)
{
  TEST_THREAD *thread = (TEST_THREAD *) arg; 
  void * thread_ctx = 0;

  if (thread->tfunc_init)
    thread_ctx = thread->tfunc_init( thread->ctx );

  CYCLIC_BARRIER_await( &thread->start );

  thread->tfunc_run( thread->ctx, thread_ctx );

  CYCLIC_BARRIER_await( &thread->finish );

  if (thread->tfunc_cleanup)
    thread->tfunc_cleanup( thread->ctx, thread_ctx );

  CYCLIC_BARRIER_await( &thread->cleanup );

  return 0;
}


static int get_num_cores()
{
  return  (int) sysconf( _SC_NPROCESSORS_ONLN );
}

int TEST_THREAD_run( TEST_THREAD *thread )
{
  pthread_t pth;
  pthread_attr_t attr;
  cpu_set_t *cpusetp;
  int rt, i, num_cores;
  size_t cpusetsize;

  num_cores = get_num_cores();
  
  for( i = 0; i < thread->num_threads; i++ ) {
     pthread_attr_init( &attr );
     pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

     cpusetp = CPU_ALLOC( num_cores );
     cpusetsize = CPU_ALLOC_SIZE( num_cores );

     CPU_ZERO_S( cpusetsize, cpusetp);
     CPU_SET_S( i, cpusetsize, cpusetp);

     pthread_attr_setaffinity_np( &attr, cpusetsize, cpusetp );

     if ((rt = pthread_create( &pth, &attr, test_thread, thread ) )  != 0) {
        printf("Could not create thread !!!\n");
	exit(1);
     }
     pthread_attr_destroy( &attr );
  }     

  CYCLIC_BARRIER_await( &thread->start );

  CYCLIC_BARRIER_await( &thread->finish );

  CYCLIC_BARRIER_await( &thread->cleanup );
  return 0;
}
