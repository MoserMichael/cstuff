#include "cthread.h"
#include <pthread.h>
#include <stdio.h>

#ifdef NO_TLS
static pthread_key_t tls_key;
#else
__thread CTHREAD *tls_thread;
#endif

static uint32_t next_tid;

int CTHREAD_libinit()
{
#ifdef NO_TLS
  pthread_key_create( &tls_key, 0);
#endif
  return 0;
}

void cthread_init( CTHREAD *arg )
{
  arg->proc( arg->ctx );
  
  arg->state = CTHREAD_STATE_EXIT; 

  STACKS_release( arg->stack_entry );

#if 0
  setcontext( &arg->context_caller );
#endif
}

typedef void (*MK_CTX_FUNC) (void);

CTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc, void *ctx )
{
  void *stack;
  CTHREAD *ret;

  ret = (CTHREAD *) malloc( sizeof(CTHREAD) );
  if (!ret) {
    return 0;
  }

  stack = STACKS_get( stacks, &ret->stack_entry );
  if (!stack) {
    return 0;
  }

  ret->proc = proc;
  ret->ctx = ctx;
  ret->thread_to_caller_value = 0;
  ret->caller_to_thread_value = 0;

  ret->prev_thread = 0;

  ret->thread_id = -1;   
  ret->state = CTHREAD_STATE_INIT;


  if (getcontext( &ret->context_coroutine )) {
    if (ret->state == CTHREAD_STATE_INIT) {
      STACKS_release(ret->stack_entry);
      free(ret);
    }  
    return 0;
  }

  ret->context_coroutine.uc_stack.ss_sp = ret;
  ret->context_coroutine.uc_stack.ss_size = STACKS_get_stack_size( stacks );


  return ret;
}



static int do_start( CTHREAD *thread )
{
  thread->state = CTHREAD_STATE_RUNNING;
  
  thread->thread_id = ++next_tid;

#ifdef NO_TLS
  thread->prev_thread = pthread_getspecific( tls_key );
  pthread_setspecific( tls_key, thread );
#else
  thread->prev_thread = tls_thread;
  tls_thread = thread;
#endif

#if 0
  thread->context_coroutine.uc_link = 0;
#else 
  thread->context_coroutine.uc_link = &thread->context_caller;
#endif
  makecontext( & thread->context_coroutine, (MK_CTX_FUNC) cthread_init, 1, thread);
  
  setcontext( &thread->context_coroutine );
  
  // should not get here.
  return -1;
}


int CTHREAD_start( CTHREAD *thread )
{
  if (thread->state != CTHREAD_STATE_INIT) {
    return -1;
  }
  getcontext( &thread->context_caller );
  if (thread->state == CTHREAD_STATE_RUNNING) {
    // got here when the running thread called yield for the first time, or exited without calling yield.
    thread->state = CTHREAD_STATE_SUSPENDED;
    return 0;
  }
  if (thread->state == CTHREAD_STATE_EXIT) {
    // got here from thread that has exited.
    return 0;
  }
  return do_start( thread );
}

int CTHREAD_join( CTHREAD *thread )
{  
   if( thread->state == CTHREAD_STATE_INIT ||
       thread->state == CTHREAD_STATE_RUNNING) {
     return -1;
   }
   while(thread->state != CTHREAD_STATE_EXIT) {
     CTHREAD_resume( thread );
   }
   return 0;
}


int CTHREAD_resume( CTHREAD *thread)
{
  if (thread->state != CTHREAD_STATE_SUSPENDED) {
    return -1;
  }

  getcontext( &thread->context_caller );
  if (thread->state == CTHREAD_STATE_RUNNING) {
    // got here when the running thread called yield for the first time, or exited without calling yield.
    thread->state = CTHREAD_STATE_SUSPENDED;
    return 0;
  }
  if (thread->state == CTHREAD_STATE_EXIT) {
    // got here from thread that has exited.
    return 0;
  }
#ifdef NO_TLS
  pthread_setspecific( tls_key, thread );
#else
  tls_thread = thread;
#endif
  return setcontext( &thread->context_coroutine );
}


int CTHREAD_yield()
{

  CTHREAD *thread;
  
#ifdef NO_TLS  
  thread = pthread_getspecific( tls_key );
#else
  thread = tls_thread;
#endif

  if (!thread) {
    return -1;
  }

  if (thread->state != CTHREAD_STATE_RUNNING) {
    return -1;
  }

  getcontext( &thread->context_coroutine );
  if (thread->state != CTHREAD_STATE_RUNNING) {
    // got here when this thread was resumed.
    thread->state = CTHREAD_STATE_RUNNING;
    return 0;
  }
  
#ifdef NO_TLS  
  pthread_setspecific( tls_key, thread->prev_thread );
#else
  tls_thread = thread->prev_thread;
#endif
  return setcontext( &thread->context_caller );
}


int CTHREAD_free( CTHREAD *thread )
{
  if (thread->state != CTHREAD_STATE_EXIT) {
    return -1;
  }
  free(thread);
  return 0;
}

uint32_t CTHREAD_get_tid()
{
  CTHREAD *thread;
  
#ifdef NO_TLS  
  thread = (CTHREAD *) pthread_getspecific( tls_key );
#else
  thread = tls_thread;
#endif

  if (!thread) {
    return (uint32_t) -1;
  }
  return thread->thread_id;
}

int CTHREAD_set_caller2thread(CTHREAD *thread, void *rvalue)
{
  if (thread->state == CTHREAD_STATE_SUSPENDED || thread->state == CTHREAD_STATE_EXIT) {
    thread->caller_to_thread_value = rvalue;
    return 0;
  }
  return -1;
}
int CTHREAD_get_caller2thread(void **rvalue)
{
  CTHREAD *thread;
  
#ifdef NO_TLS  
  thread = (CTHREAD *) pthread_getspecific( tls_key );
#else
  thread = tls_thread;
#endif


  if (!thread) {
    return -1;
  }
  if (thread->state != CTHREAD_STATE_RUNNING) {
    return -1;
  }

  *rvalue = thread->caller_to_thread_value;
  return 0;
}

int CTHREAD_set_thread2caller(void *rvalue)
{
  CTHREAD *thread;
  
#ifdef NO_TLS  
  thread = (CTHREAD *) pthread_getspecific( tls_key );
#else
  thread = tls_thread;
#endif

  if (thread->state == CTHREAD_STATE_RUNNING) {
    thread->thread_to_caller_value = rvalue;
    return 0;
  }
  return -1;
}

int CTHREAD_get_thread2caller(CTHREAD *thread, void **rvalue)
{ 
  if (!thread) {
    return -1;
  }
  if (thread->state == CTHREAD_STATE_SUSPENDED || thread->state == CTHREAD_STATE_EXIT) {
    *rvalue = thread->thread_to_caller_value;
    return 0;
  }
  return -1;
}

