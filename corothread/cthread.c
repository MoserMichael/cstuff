#define _CTHREAD_MAIN_

#include "cthread.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <cutils/dring.h>
#include <butils/logg.h>
#include <errno.h>
#include "internal.h"



static int is_libinit;


#ifdef HAS_WIN32_FIBER
static void *main_thread_fiber;
#endif
 
typedef struct tagCTHREAD {

#ifdef HAS_SETJMP_LONGJMP
  jmp_buf context_coroutine;
  jmp_buf context_caller;
#endif

#ifdef HAS_WIN32_FIBER
  void * pcontext_coroutine;
  void * pcontext_caller;
#endif

#ifdef HAS_UCONTEXT
  ucontext_t context_coroutine;
  ucontext_t context_caller;
#endif
    
  CTHREAD_STATE state;			    // thread state
  int		request_exit;

  uint32_t thread_id;			    // unique id of thread.
 
  int    thread_to_caller_value_set;	    // is yield value set ?
  VALUES thread_to_caller_value;	    // the yield value.
  
  int    caller_to_thread_value_set;	    // is resume value set ?
  VALUES caller_to_thread_value;	    // resume value.

  CTHREAD_PROC proc;			    // thread procedure.
  struct tagCTHREAD *prev_thread;

  STACK_ENTRY *stack_entry;		    // stack entry.
  size_t stack_size;

  DRING  entry;				    // this thread in child list of parent thread.
  DRING  child_threads;			    // all threads started by this thread (killed when thread is killed or exits).

} CTHREAD;


#ifdef NO_TLS
static pthread_key_t tls_key;
#else
static __thread CTHREAD *tls_thread;
#endif

/*
static CTHREAD *ttls_thread;

#define GET_TLS()  ttls_thread
#define SET_TLS(val) do { ttls_thread = val; } while( 0 );
*/

#ifdef NO_TLS
#define GET_TLS()  ((CTHREAD *) pthread_getspecific( tls_key ))
#define SET_TLS(val)  pthread_setspecific( tls_key, val );
#else 
#define GET_TLS()  tls_thread
#define SET_TLS(val) do { tls_thread = val; } while( 0 );
#endif


static uint32_t next_tid;

#ifdef HAS_SETJMP_LONGJMP

#define PROBE_A_STACK_SIZE 512
static jmp_buf buf_probe_a_before, buf_probe_a_after;

#define PROBE_B_STACK_SIZE 4096
static int index_of_stack_in_jmpbuf = -1;
static jmp_buf buf_probe_b_before, buf_probe_b_after;

size_t fill_buff_probe_a()
{
  static jmp_buf tmp_buf;
  int r;
  size_t rbuf;

  rbuf = (size_t) alloca( 8 );

  r = setjmp( tmp_buf );

  if ( r ) {
    rbuf += (size_t) alloca( PROBE_A_STACK_SIZE );
  }  
    
  setjmp( buf_probe_a_after );

  if (! r ) {
    memcpy( (void *) &buf_probe_a_before, (void *) &buf_probe_a_after, sizeof( jmp_buf ) );
    longjmp(tmp_buf, 1);
    fprintf(stderr,"fill_buf_probe_a - longjmp failed\n");
    exit(1);
  }
  return rbuf;
}

size_t fill_buff_probe_b()
{
  static jmp_buf tmp_buf;
  int r;
  size_t rbuf;

  rbuf = (size_t) alloca( 8 );

  r = setjmp( tmp_buf );
  if ( r ) {
    rbuf += (size_t) alloca( PROBE_B_STACK_SIZE );
  }  
    
  setjmp( buf_probe_b_after );
              
  if (! r ) {
    memcpy( (void *) &buf_probe_b_before, (void *) &buf_probe_b_after, sizeof( jmp_buf ) );
    longjmp(tmp_buf, 1);
    fprintf(stderr,"fill_buf_probe_b - longjmp failed\n");
    exit(1);
  } 
  return rbuf;
}


#define difff(x,y) ((x) < (y) ? (y) - (x) : (x) - (y))

int diff_buf(jmp_buf *buf_a, jmp_buf *buf_b, size_t diff)
{
   size_t *tmp_a, *tmp_b;
   int i, rt = -1;
   tmp_a = (size_t *) buf_a;
   tmp_b = (size_t *) buf_b;

   assert( sizeof( size_t) == sizeof( void * ) );

   for( i = 0; i < (int) (sizeof( jmp_buf ) / sizeof( ptrdiff_t ) ) ; ++i ) {
     
      if (get_stack_direction() == STACK_DIR_GROWING_DOWN) {
        if (*tmp_a > *tmp_b && difff( *tmp_a, *tmp_b) >= diff ) {
#if 0	
          printf(" off %d %x %x %d\n", i, *tmp_a, *tmp_b, difff( *tmp_a, *tmp_b) );
#endif	  
	  rt = i;
        }
      } else {
        if (*tmp_a < *tmp_b && difff( *tmp_a, *tmp_b) >= diff ) {
          assert(0);
#if 0
          printf(" off %d %x %x %d\n", i, *tmp_a, *tmp_b, difff( *tmp_a, *tmp_b) );
#endif
	  rt = i;
        }
      }
      ++tmp_a; 
      ++tmp_b;
   }
   return rt;
}


// try to figure out, which index in jmp_buf holds the stack pointer.
int find_index_of_stack_in_jmpbuf()
{
   int idx_probe_a, idx_probe_b;


   fill_buff_probe_a();
   fill_buff_probe_b();

   idx_probe_a = diff_buf( &buf_probe_a_before, &buf_probe_a_after, PROBE_A_STACK_SIZE );
   idx_probe_b = diff_buf( &buf_probe_b_before, &buf_probe_b_after, PROBE_B_STACK_SIZE );
  
   if (idx_probe_a != idx_probe_b || idx_probe_a == -1) {
       //printf("idx_probe_a %d idx_probe_b %d\n", idx_probe_a, idx_probe_b);
       MLOG_ERROR("Failed to get index in jmp_buf that points to stack size. very sorry");
       exit(-1);
   }
   
   index_of_stack_in_jmpbuf = idx_probe_a;
   MLOG_TRACE( "CTHREAD_libinit - index in jmp_buf where stack pointer is. %u", index_of_stack_in_jmpbuf);

   return idx_probe_a;
}


void switch_to_thread( CTHREAD *thread )
{
   if ( ! setjmp(  thread->context_caller)) {
     longjmp( thread->context_coroutine, 1 ); 
     assert(0);
   }
}

void switch_to_caller( CTHREAD *thread  )
{
   if ( ! setjmp( thread->context_coroutine)) {
     longjmp( thread->context_caller, 1 ); 
     assert(0);
   }
}


void exit_to_caller( CTHREAD *thread  )
{
   SET_TLS( thread );
   longjmp(  GET_TLS()->context_caller, 1 ); 
   assert(0);
}

#endif


int CTHREAD_libinit()
{ 
  if (is_libinit) {
     MLOG_TRACE("CTHREAD_libinit - already initialized" );
     return 0;
  }  
  is_libinit = 1;


#ifdef NO_TLS
  pthread_key_create( &tls_key, 0);
#endif

#ifdef HAS_SETJMP_LONGJMP
  find_index_of_stack_in_jmpbuf();
#endif

#ifdef HAS_WIN32_FIBER
  if (!main_thread_fiber) {
     main_thread_fiber = ConvertThreadToFiber( 0 );
     if (!main_thread_fiber) {                                   
       MLOG_ERROR("CTHREAD_libinit - ConvertThreadToFiber failed error=%d ", GetLastError() );
       return -1; 
     }
     MLOG_TRACE("main fiber %p", main_thread_fiber );
  }
#endif  
  MLOG_DEBUG("CTHREAD_libinit - ok" );
  
  return 0;
}


#define NO_INLINE __attribute__ ((noinline))

#ifdef HAS_SETJMP_LONGJMP
void __attribute__ ((noinline)) ccthread_cont()
{
  CTHREAD *arg = GET_TLS();
#else
void ccthread_cont(CTHREAD *arg)
{
#endif

  if (arg->state == CTHREAD_STATE_RUNNING )  {
    MLOG_DEBUG("cthread %x: enter thread procedure", arg->thread_id );
    arg->proc( &arg->caller_to_thread_value );
    MLOG_DEBUG("cthread %x: leave thread procedure", arg->thread_id );
  } else {
    MLOG_ERROR("cthread %x: thread procedure: can't start thread - thread must be in running state", arg->thread_id );
  }
  
  CTHREAD_exit();
}

#ifdef HAS_WIN32_FIBER
void  ccthread_init( void * aarg )
{
  CTHREAD *arg = (CTHREAD *) aarg;
#else
void ccthread_init( )
{
#endif

#ifdef HAS_SETJMP_LONGJMP
  // right now stack pointer (esp) is ok, but base pointer (ebp) is messed up.
  // so further action are in the next function call - to fix up ebp by creating a new frame (calling ccthread_cont).
  // normally a function starts with mov ebp, esp ; that is.

  if (setjmp( GET_TLS()->context_coroutine ) == 0) {
    return;
  }
  ccthread_cont();   
#else
  ccthread_cont( GET_TLS() );
#endif
}



typedef void (*MK_CTX_FUNC) (void);

CTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc )
{
  CTHREAD *ret;
  int stage = 0;

  ret = (CTHREAD *) malloc( sizeof(CTHREAD) );
  if (!ret) {
     MLOG_ERROR("CTHREAD_init failed - out of memory" );
     return 0;
  }
  stage = 1;
  
  ret->proc = proc;
  ret->prev_thread = 0;
  ret->thread_id = ++next_tid;
  ret->state = CTHREAD_STATE_INIT;
  ret->request_exit = 0;

  DRING_init( &ret->child_threads );  
  DRING_init( &ret->entry );  

  if (VALUES_init(&ret->thread_to_caller_value)) {
    goto err;
  } 
  stage = 2;

  if (VALUES_init(&ret->caller_to_thread_value)) {
    goto err;
  } 
  stage = 3;

  ret->stack_entry = STACKS_get( stacks  );
  if (!ret->stack_entry) {
    MLOG_ERROR("cthread %x: CTHREAD_init - can' get stack for thread", ret->thread_id );
    goto err;
  }
  stage = 4;

  ret->caller_to_thread_value_set = ret->thread_to_caller_value_set = 0;
  ret->stack_size =  STACKS_get_stack_size( stacks );
 
#ifdef HAS_WIN32_FIBER
   MLOG_DEBUG("cthread %x: CTHREAD_init - create thread with stack size %ld", ret->thread_id, STACKS_get_stack_size( stacks ) );
 
  ret->pcontext_coroutine = CreateFiber( STACKS_get_stack_size( stacks ) , ccthread_init, ret ); 
//ret->pcontext_coroutine = CreateFiber( 0 , ccthread_init, ret ); 
  if (!ret->pcontext_coroutine) {
    MLOG_ERROR("cthread %x: CTHREAD_init - can' create fiber %d", ret->thread_id, GetLastError() );
    goto err; 
  }
#endif

  MLOG_DEBUG("cthread %x: CTHREAD_init ok", ret->thread_id );
  return ret;

err:
  MLOG_ERROR("cthread %x: CTHREAD_init failed", ret->thread_id );
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

static void do_start( CTHREAD *thread, CTHREAD *parent )
{
#ifdef HAS_SETJMP_LONGJMP
  void **stk;
  uint8_t *stack_ptr;
#endif

  thread->state = CTHREAD_STATE_RUNNING;
  thread->prev_thread = GET_TLS();  

  if (parent) {
    DRING_push_back( &parent->child_threads, &thread->entry ); 
  }

  MLOG_DEBUG("cthread %x: CTHREAD_start starting thread, suspending thread %x", thread->thread_id, (parent ? parent->thread_id :  (uint32_t) -1));

#ifdef HAS_UCONTEXT 
  if (getcontext( &thread->context_coroutine ) == -1 ) {
    MLOG_ERROR("cthread %x: CTHREAD_start - getcontext failed. errno=%d", thread->thread_id, errno );
    return;  	
  }

  thread->context_coroutine.uc_stack.ss_sp = thread->stack_entry->stack_start;
  thread->context_coroutine.uc_stack.ss_size = thread->stack_entry->stack_size;
  thread->context_coroutine.uc_stack.ss_flags = 0;
 
  SET_TLS( thread );
  
  makecontext( &thread->context_coroutine, (MK_CTX_FUNC) ccthread_init, 0 );
  setcontext( &thread->context_coroutine );
  
  MLOG_ERROR("cthread %x: CTHREAD_start - setcontext failed errno=%d", thread->thread_id, errno );
#endif

#ifdef HAS_SETJMP_LONGJMP
  SET_TLS( thread );
  ccthread_init( thread );

  stack_ptr = thread->stack_entry->stack_start;
  if ( get_stack_direction() == STACK_DIR_GROWING_DOWN )
    stack_ptr += thread->stack_entry->stack_size;
  
  stk = (void **) &thread->context_coroutine; 
  stk[ index_of_stack_in_jmpbuf ] = stack_ptr; 

  //fprintf(stderr,"stack %p\n", thread->stack_entry->stack_start);
 
  switch_to_thread( thread );
#endif

#ifdef HAS_WIN32_FIBER
  thread->pcontext_caller = parent != 0 ?  parent->pcontext_coroutine : main_thread_fiber; 
  assert( thread->pcontext_caller != 0 );
  assert( thread->pcontext_coroutine != 0 );
  assert( thread->pcontext_caller != thread->pcontext_coroutine  );
  
  MLOG_TRACE("main fiber %p", thread->pcontext_caller );
  SwitchToFiber( thread->pcontext_coroutine );  
#endif

}

int CTHREAD_start( CTHREAD *thread, VALUES **rvalue, const char *format , ... )
{
  va_list vlist;
  CTHREAD *parent;

  if (thread->state != CTHREAD_STATE_INIT) {
    MLOG_ERROR("cthread %x: CTHREAD_start - must be in CTHREAD_STATE_INIT, call right after CTHREAD_init", thread->thread_id );
    return -1;
  }
  
  parent = GET_TLS();

#ifdef HAS_UCONTEXT 
  if (getcontext( &thread->context_caller )) {
    MLOG_ERROR("cthread %x: CTHREAD_start - getcontext failed. error=%d", thread->thread_id, errno );
    return -1;
  }
  if (thread->state != CTHREAD_STATE_INIT) {
    goto check_return;
  }  
#endif    

  thread->caller_to_thread_value_set = 0;
  if (format) {
   
    va_start( vlist, format );
    
    if (VALUES_printv( &thread->caller_to_thread_value, format, vlist ) ) {
      va_end( vlist );
      return -1;
    }
    va_end( vlist );
 
    thread->caller_to_thread_value_set = 1;
  } 

  do_start( thread, parent );

#ifdef HAS_UCONTEXT 
  return -1;

check_return:

#endif

  SET_TLS( parent );
  
  if (thread->state == CTHREAD_STATE_RUNNING) {
    
    // got here when the running thread called yield for the first time, or exited without calling yield.
    MLOG_DEBUG("cthread %x: CTHREAD_start - thread suspended, resumed thread %x", thread->thread_id, (parent ? parent->thread_id : (uint32_t) -1) );
    thread->state = CTHREAD_STATE_SUSPENDED;
   
    if (rvalue) {
      if (thread->thread_to_caller_value_set) {
       *rvalue = &thread->thread_to_caller_value; 
      } else {
        *rvalue = 0;
      }
    }
    return 0;
  } else if (thread->state == CTHREAD_STATE_EXIT) {
    
    // got here from thread that has exited.
    MLOG_DEBUG("cthread %x: CTHREAD_start - thread finished, resumed thread %x", thread->thread_id, (parent ? parent->thread_id :  (uint32_t) -1) );
    
    if (rvalue) {
      if (thread->thread_to_caller_value_set) {
        *rvalue = &thread->thread_to_caller_value; 
      } else {
        *rvalue = 0;
      } 
    }
    return 1;
  } 

  MLOG_ERROR("cthread %x: CTHREAD_start - can't be in state %d", thread->thread_id, thread->state );
  return -1;
}

  

#if 0
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
#endif

int CTHREAD_kill(  struct tagCTHREAD *thread )
{
    MLOG_DEBUG("cthread %x: start CTHREAD_kill", thread->thread_id );
    
    thread->request_exit = 1;
    CTHREAD_resume( thread, 0, 0 );

    if (thread->state != CTHREAD_STATE_EXIT) {
      MLOG_ERROR("CTHREAD_kill - thread did not exit when it was resumed" );
      return -1;
    }

    MLOG_DEBUG("cthread %x: completed CTHREAD_kill", thread->thread_id );
     
    return 0;
}


int CTHREAD_resume( CTHREAD *thread, VALUES **rvalue, const char *format, ... )
{
  va_list vlist;
  CTHREAD *current;

  if (thread->state != CTHREAD_STATE_SUSPENDED) {
    MLOG_ERROR("cthread %x: CTHREAD_resume - must resume a suspended thread", thread->thread_id );
    return -1;
  }

  current = GET_TLS();
  if (current == thread) {
    MLOG_ERROR("cthread %x: CTHREAD_resume - can't resume as this is the current thread (which also can't be suspended)", thread->thread_id );
    return -1;
  }

#ifdef HAS_UCONTEXT 
  getcontext( &thread->context_caller );
  
  if (thread->state != CTHREAD_STATE_SUSPENDED) {
    goto check_return;
  }
#endif

  thread->caller_to_thread_value_set = 0;
  if (format) {
   
    va_start( vlist, format );
    
    if (VALUES_printv( &thread->caller_to_thread_value, format, vlist ) ) {
       va_end( vlist );
       MLOG_ERROR("cthread %x: CTHREAD_yield - failed to format message to hread", thread->thread_id );
       return -1;
    }
 
    va_end( vlist );
    thread->caller_to_thread_value_set = 1;
  } 

  MLOG_DEBUG("cthread %x: enter CTHREAD_resume - suspended current thread, resume thread %x", current != 0 ? current->thread_id : (size_t) -1, thread->thread_id );
 
#ifdef HAS_SETJMP_LONGJMP
  switch_to_thread( thread );
#endif
  
#ifdef HAS_UCONTEXT 
  setcontext( &thread->context_coroutine );

  MLOG_ERROR("cthread %x: CTHREAD_resume - setcontext failed errno=%d", thread->thread_id, errno );

  return -1;
#endif

#ifdef  HAS_WIN32_FIBER
  SwitchToFiber( thread->pcontext_coroutine );  
#endif

#ifdef HAS_UCONTEXT 
check_return:
#endif  

  SET_TLS( current );

  if (thread->state == CTHREAD_STATE_RUNNING) {
 
    // got here when the running thread called yield for the first time, or exited without calling yield.
    thread->state = CTHREAD_STATE_SUSPENDED;
    MLOG_DEBUG("cthread %x: exit CTHREAD_resume - thread resumed, suspended thread %x", (current ? current->thread_id : (uint32_t) -1), thread->thread_id );
   
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
    MLOG_DEBUG("cthread %x: exit CTHREAD_resume - thread resumed, finished thread %x", (current ? current->thread_id : (uint32_t) -1), thread->thread_id );
    if (rvalue) {
      if (thread->thread_to_caller_value_set) {
        *rvalue = &thread->thread_to_caller_value; 
      } else {
        *rvalue = 0;
      }
    }
    return 1;
  }
  
  MLOG_ERROR("cthread %d: CTHREAD_resume - can't be in state %d", thread->thread_id, thread->state );
  return -1;
}

int CTHREAD_exit()
{
  CTHREAD *thread, *cur_thread;
  DRING *cur;

  thread = GET_TLS();

  if (!thread) {
    MLOG_ERROR("CTHREAD_exit - no thread is currently running" );
    return -1;
  }
 
  MLOG_DEBUG("cthread %x: enter CTHREAD_exit", thread->thread_id );
 
  if (thread->state != CTHREAD_STATE_RUNNING) {
    MLOG_ERROR("CTHREAD_exit - must be called from running thread" );
    return -1;
  }

  DRING_FOREACH( cur, &thread->child_threads ) {
    cur_thread = (CTHREAD *) _OFFSETOF( cur, CTHREAD, child_threads );
    MLOG_DEBUG("cthread %x: killing thread %x", thread->thread_id, cur_thread->thread_id );
    CTHREAD_kill( cur_thread );
  }  

  MLOG_DEBUG("cthread %x: switch state to exit and leave", thread->thread_id );
  
  thread->state = CTHREAD_STATE_EXIT;
 
  // does not free the stack memory, otherwise we could not call next function. 
  STACKS_release( thread->stack_entry );

#ifdef HAS_SETJMP_LONGJMP
   exit_to_caller( thread ); 
#endif

#ifdef HAS_UCONTEXT 
  setcontext( &thread->context_caller );
#endif

#ifdef HAS_WIN32_FIBER
  SwitchToFiber( thread->pcontext_caller );  

#endif 
  
  MLOG_ERROR("cthread %x: CTHREAD_exit - thread continues after completing exit", thread->thread_id );

  return -1;
}


int CTHREAD_yield(VALUES **rvalue, const char *format, ... )
{
  va_list vlist;
  CTHREAD *thread;
  
  thread = GET_TLS();

  if (!thread) {
    MLOG_ERROR("CTHREAD_yield - no thread is currently running" );
    return -1;
  }

  if (thread->state != CTHREAD_STATE_RUNNING) {
      MLOG_ERROR("cthread %x: CTHREAD_yield - must be called from running thread", thread->thread_id );
      return -1;
  }

  if (thread->request_exit) {
      MLOG_ERROR("cthread %x: CTHREAD_yield - can't yield, calling thread commands this thread to exit now", thread->thread_id );
      CTHREAD_exit();
      assert(0);
      return -1;
  }


  thread->thread_to_caller_value_set = 0;
  if (format) {
   
    va_start( vlist, format );
    if (VALUES_printv( &thread->thread_to_caller_value, format, vlist ) ) {
       va_end( vlist );
       MLOG_ERROR("cthread %x: CTHREAD_yield - failed to format message to parent thread", thread->thread_id );
       return -1;
    }
    va_end( vlist );
    thread->thread_to_caller_value_set = 1;
  }

#ifdef HAS_UCONTEXT 
  getcontext( &thread->context_coroutine );
  
  if (thread->state != CTHREAD_STATE_RUNNING) {
    
    goto check_return;
  }
#endif

  MLOG_DEBUG("cthread %x: enter CTHREAD_yield - suspended current thread", thread->thread_id );
 
#ifdef HAS_SETJMP_LONGJMP
  switch_to_caller( thread );
#endif
 
#ifdef HAS_UCONTEXT 
  setcontext( &thread->context_caller );

  MLOG_ERROR("cthread %x: CTHREAD_yield - setcontext failed errno=%d", thread->thread_id, errno );
  return -1;

check_return:
#endif

#ifdef HAS_WIN32_FIBER
  SwitchToFiber( thread->pcontext_caller );  
#endif

  SET_TLS( thread );

  // got here when this thread was resumed.
  thread->state = CTHREAD_STATE_RUNNING;
  MLOG_DEBUG("cthread %x: exit CTHREAD_yield - thread resumed %s ", thread->thread_id, thread->request_exit ? ". !THREAD MUST EXIST NOW!" : ""  );
 
  if (rvalue) {
    if (thread->caller_to_thread_value_set) {
        *rvalue = &thread->caller_to_thread_value; 
    } else {
       *rvalue = 0;
    }
  }
  return thread->request_exit;
}


int CTHREAD_free( CTHREAD *thread )
{
  if (thread == GET_TLS())  {
    MLOG_ERROR("cthread %x: CTHREAD_free - can't free the currently running thread", thread->thread_id );
    return -1;
  }

  if (thread->state != CTHREAD_STATE_EXIT) {
    MLOG_ERROR("cthread %x: CTHREAD_free - can't free thread that did not exit.", thread->thread_id );
    return -1;
  }
#ifdef HAS_WIN32_FIBER
  DeleteFiber( thread->pcontext_coroutine );
#endif
  
  DRING_unlink( &thread->entry ); //child_threads );
  free(thread);
  MLOG_TRACE("cthread %x: CTHREAD_free - completed", thread->thread_id );
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
    va_end( vlist );
    thread->thread_to_caller_value_set = 0;
    return -1;
  }
  va_end( vlist );
 
  thread->thread_to_caller_value_set = 1;
  return 0;
}

CTHREAD_STATE CTHREAD_state(struct tagCTHREAD *thread) 
{
  return thread->state;
}



