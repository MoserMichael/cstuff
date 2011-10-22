#include "cthread.h"
#include <pthread.h>
#include <stdio.h>

#ifdef NO_TLS
static pthread_key_t tls_key;
#else
__thread CTHREAD *tls_thread;
#endif

#ifdef NO_TLS
#define GET_TLS()  (CTHREAD *) pthread_getspecific( tls_key );
#define SET_TLS(val)  (CTHREAD *) pthread_setspecific( tls_key, val );
#else 
#define GET_TLS()  tls_thread
#define SET_TLS(val) do { tls_thread = val; } while( 0 );
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
  arg->proc( &arg->caller_to_thread_value );
  
  arg->state = CTHREAD_STATE_EXIT; 

  STACKS_release( arg->stack_entry );

#if 0
  setcontext( &arg->context_caller );
#endif
}

typedef void (*MK_CTX_FUNC) (void);

CTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc )
{
  void *stack;
  CTHREAD *ret;
  int stage = 0;

  ret = (CTHREAD *) malloc( sizeof(CTHREAD) );
  if (!ret) {
    return 0;
  }
  stage = 1;

  if (VALUES_init(&ret->thread_to_caller_value)) {
    goto err;
  } 
  stage = 2;

  if (VALUES_init(&ret->caller_to_thread_value)) {
    goto err;
  } 
  stage = 3;

  stack = STACKS_get( stacks, &ret->stack_entry );
  if (!stack) {
    goto err;
  }
  stage = 4;

  ret->proc = proc;

  ret->prev_thread = 0;

  ret->thread_id = -1;   
  ret->state = CTHREAD_STATE_INIT;

  ret->caller_to_thread_value_set = ret->thread_to_caller_value_set = 0;
 
  if (getcontext( &ret->context_coroutine )) {
    if (ret->state == CTHREAD_STATE_INIT) {
      goto err;
    }
    return 0;
  }

  ret->context_coroutine.uc_stack.ss_sp = ret;
  ret->context_coroutine.uc_stack.ss_size = STACKS_get_stack_size( stacks );


  return ret;

err:
  if (stage > 3) {
    STACKS_release(ret->stack_entry);
  }
  if (stage > 2) {
    VALUES_free(&ret->caller_to_thread_value);
  }
   if (stage > 1) {
    VALUES_free(&ret->thread_to_caller_value);
  }
  if (stage > 0) {
    free(ret);
  }
  return 0;
}

static int do_start( CTHREAD *thread )
{
  thread->state = CTHREAD_STATE_RUNNING;
  
  thread->thread_id = ++next_tid;
  
  thread->prev_thread = GET_TLS();  
  SET_TLS( thread );

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

int CTHREAD_start( CTHREAD *thread, VALUES **rvalue, const char *format , ... )
{
  if (thread->state != CTHREAD_STATE_INIT) {
    return -1;
  }
  getcontext( &thread->context_caller );
  if (thread->state == CTHREAD_STATE_RUNNING) {
    // got here when the running thread called yield for the first time, or exited without calling yield.
    thread->state = CTHREAD_STATE_SUSPENDED;
    if (rvalue) {
     if (thread->thread_to_caller_value_set) {
      *rvalue = &thread->thread_to_caller_value; 
     } else {
       *rvalue = 0;
     }
    }
    return 0;
  }
  if (thread->state == CTHREAD_STATE_EXIT) {
    // got here from thread that has exited.
    if (rvalue) {
     if (thread->thread_to_caller_value_set) {
       *rvalue = &thread->thread_to_caller_value; 
     } else {
       *rvalue = 0;
     }
    }
    return 0;
  }

  thread->caller_to_thread_value_set = 0;
  if (format) {
    va_list vlist;
    
    va_start( vlist, format );
    
    if (VALUES_printv( &thread->caller_to_thread_value, format, vlist ) ) {
      return -1;
    }

    thread->caller_to_thread_value_set = 1;
  } 
  

  return do_start( thread );
}




int CTHREAD_join( CTHREAD *thread, VALUES **rvalue )
{  
   if( thread->state == CTHREAD_STATE_INIT ||
       thread->state == CTHREAD_STATE_RUNNING) {
     return -1;
   }
   while(thread->state != CTHREAD_STATE_EXIT) {
     CTHREAD_resume( thread, rvalue, 0 );
   }
   return 0;
}

int CTHREAD_resume( CTHREAD *thread, VALUES **rvalue, const char *format, ... )
{
  if (thread->state != CTHREAD_STATE_SUSPENDED) {
    return -1;
  }

  getcontext( &thread->context_caller );
  if (thread->state == CTHREAD_STATE_RUNNING) {
    // got here when the running thread called yield for the first time, or exited without calling yield.
    thread->state = CTHREAD_STATE_SUSPENDED;
    if (rvalue) {
     if (thread->thread_to_caller_value_set) {
       *rvalue = &thread->thread_to_caller_value; 
     } else {
       *rvalue = 0;
     }
    }
    return 0;
  }
  if (thread->state == CTHREAD_STATE_EXIT) {
    // got here from thread that has exited.
     if (thread->thread_to_caller_value_set) {
       *rvalue = &thread->thread_to_caller_value; 
     } else {
       *rvalue = 0;
     }
    return 0;
  }

  thread->caller_to_thread_value_set = 0;
  if (format) {
    va_list vlist;
    
    va_start( vlist, format );
    
    if (VALUES_printv( &thread->caller_to_thread_value, format, vlist ) ) {
      return -1;
    }
    thread->caller_to_thread_value_set = 1;
  } 

  SET_TLS( thread );
  
  return setcontext( &thread->context_coroutine );
}


int CTHREAD_yield(VALUES **rvalue, const char *format, ... )
{
  CTHREAD *thread;
  
  thread = GET_TLS();

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
    if (rvalue) {
     if (thread->caller_to_thread_value_set) {
       *rvalue = &thread->caller_to_thread_value; 
     } else {
       *rvalue = 0;
     }
    }
    return 0;
  }
 
  thread->thread_to_caller_value_set = 0;
  if (format) {
    va_list vlist;
    
    va_start( vlist, format );
    
    if (VALUES_printv( &thread->thread_to_caller_value, format, vlist ) ) {
      return -1;
    }
    thread->thread_to_caller_value_set = 1;
  }

  SET_TLS( thread->prev_thread );
 
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
 
  thread = GET_TLS();
  if (!thread) {
    return (uint32_t) -1;
  }
  return thread->thread_id;
}

int CTHREAD_set_return_value( const char *format, ... )
{
  CTHREAD *thread;
  va_list vlist;
  
  thread = GET_TLS();
  if (!thread) {
   return (uint32_t) -1;
  }

  va_start( vlist, format );
  
  thread->thread_to_caller_value_set = 0;
  if (VALUES_printv( &thread->thread_to_caller_value, format, vlist )) {
    thread->thread_to_caller_value_set = 0;
    return -1;
  }

  thread->thread_to_caller_value_set = 1;
  return 0;
}




