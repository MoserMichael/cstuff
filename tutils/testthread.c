#define _GNU_SOURCE 
#include <pthread.h>
#include <sched.h>

#include "testthread.h"
#include "tstart.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int TEST_THREAD_init (TEST_THREAD *thread, int num_threads,  TEST_FUNC_INIT test_init, TEST_FUNC test_func, TEST_FUNC cleanup, void *test_ctx )
{
    thread->num_threads = num_threads;
    thread->ctx = test_ctx;

    thread->tfunc_init = test_init;
    thread->tfunc_run  = test_func;
    thread->tfunc_cleanup = cleanup;

    if ( CYCLIC_BARRIER_init( &thread->start, num_threads + 1 ) )
    {
      printf("Failed to init test thread (1)\n");
      return -1;
    }

    if ( CYCLIC_BARRIER_init( &thread->finish, num_threads + 1 ) )
    {
      printf("Failed to init test thread (2)\n");
      return -1;
    }

    if ( CYCLIC_BARRIER_init( &thread->cleanup, num_threads + 1 ) )
    {	 
       printf("Failed to init test thread (3)\n");
     return -1;
    }      
	 
    return 0;
}

//---

typedef struct tagTDATA {
  TEST_THREAD *thread;
  int thread_id;
} TDATA;  


void * test_thread_runner( void * arg)
{
  TDATA *tdata = (TDATA *) arg;
  TEST_THREAD *thread = tdata->thread;
  void * thread_ctx = 0;

  if (thread->tfunc_init)
    thread_ctx = thread->tfunc_init( thread->ctx, tdata->thread_id, thread->num_threads );

  CYCLIC_BARRIER_await( &thread->start );

  thread->tfunc_run( thread->ctx, thread_ctx, tdata->thread_id, thread->num_threads );

  CYCLIC_BARRIER_await( &thread->finish );

  if (thread->tfunc_cleanup) {
    thread->tfunc_cleanup( thread->ctx, thread_ctx, tdata->thread_id, thread->num_threads );
    CYCLIC_BARRIER_await( &thread->cleanup );
  }    

  return 0;
}

#if 0
static int get_num_cores()
{
  return  (int) sysconf( _SC_NPROCESSORS_ONLN );
}
#endif

int TEST_THREAD_run( TEST_THREAD *thread )
{
  pthread_t pth;
  pthread_attr_t attr;
  int rt, i;
#if 0  
  int num_cores;
  cpu_set_t *cpusetp;
  size_t cpusetsize;
#endif  
  TDATA *tdata;

#if 0
  num_cores = get_num_cores();
#endif

  for( i = 0; i < thread->num_threads; i++ ) {
     pthread_attr_init( &attr );

#if 0
     if (num_cores > 1) {
	 cpusetp = CPU_ALLOC( num_cores );
	 cpusetsize = CPU_ALLOC_SIZE( num_cores );

	 CPU_ZERO_S( cpusetsize, cpusetp);
	 CPU_SET_S( i, cpusetsize, cpusetp);
  
	 pthread_attr_setaffinity_np( &attr, cpusetsize, cpusetp );
     }
#endif

     tdata = (TDATA *) malloc( sizeof( TDATA ) );
     tdata->thread = thread;
     tdata->thread_id = i; 

     if ((rt = pthread_create_detached( &pth, &attr, test_thread_runner, tdata ) )  != 0) {
        printf("Could not create thread number %d ret=%d errno=%d!!!\n",  i, rt, errno );
	exit(1);
     }
     pthread_attr_destroy( &attr );
  }     

  CYCLIC_BARRIER_await( &thread->start );

  CYCLIC_BARRIER_await( &thread->finish );

  if (thread->tfunc_cleanup) {
     CYCLIC_BARRIER_await( &thread->cleanup );
  } 

  CYCLIC_BARRIER_free( &thread->start );
  CYCLIC_BARRIER_free( &thread->finish );
  if (thread->tfunc_cleanup) {
    CYCLIC_BARRIER_free( &thread->cleanup );
  }    
  return 0;
}
