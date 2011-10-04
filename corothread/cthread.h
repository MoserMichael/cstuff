#ifndef __CTHREAD_H__
#define __CTHREAD_H__


#include <ucontext.h>
#include <stdint.h>
#include <corothread/stacks.h>
#include <corothread/val.h>

/** 
 * @defgroup CTHREAD
 * @brief a coroutine thread.
 *
 * A very limited user mode threading package that allows constructions of co-routines.
 * Coroutine scheduling is done by mean of mkcontet/setcontext/getcontext functions (glibc)
 *
 * User mode threads are best when used in a very limited scenario, complex usage scenarios often
 * lead to intricate situations that are hard to debug.
 * Areas where user mode threading shines 
 *   - coroutines / continuations
 *   - generator funcions (like python/ruby/perl).
 *
 *
 * @{
 */

typedef void  (*CTHREAD_PROC) (VALUES *);

typedef enum {
  CTHREAD_STATE_INIT,
  CTHREAD_STATE_RUNNING,
  CTHREAD_STATE_SUSPENDED,
  CTHREAD_STATE_EXIT,
} CTHREAD_STATE;



typedef struct tagCTHREAD {
  ucontext_t context_coroutine;
  ucontext_t context_caller;
  CTHREAD_STATE state;
  uint32_t thread_id;
 
  VALUES thread_to_caller_value; 
  VALUES caller_to_thread_value;  

  CTHREAD_PROC proc;  
  struct tagCTHREAD *prev_thread;
  STACK_ENTRY *stack_entry;  
} CTHREAD;



/**
 * @brief library initialisation
 */
int CTHREAD_libinit();

/**
 * @brief initialises a new co-routine thread. 
 
 * Postcondition: The new thread is initialised and enters CTHREAD_STATE_INIT state.

 * @param stacks the thread is obtained from the pool of stacks (argument stack).
 * @param proc The thread procecdure is argument proc.
 */
CTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc );

/**
 * deallocates a thread 
 *
 * Precondition: the thread must be in CTHREAD_STATE_EXIT state 
 */ 
int CTHREAD_free( CTHREAD *thread );

/**
 * @brief start a new thread - the thread procedure is invoked.
 *
 * The initialised thread is started; The function returns if the running thread either exited or entered suspended state (i.e. the thread called CTHREAD_yield)
 *
 * Values passed from the thread to the caller are stored in rvalue parameter.
 * If this parameter is 0 then the return values are ignored.
 * 
 * Values passed from the caller to the thread -
 *   
 * Precondition: the thread is in either CTHREAD_STATE_INIT or CTHREAD_STATE_EXIT state.
 * Postcondition: the thread is in CTHREAD_STATE_RUNNING state.
 *
 */
int CTHREAD_start( CTHREAD *thread, VALUES **rvalue, const char *format , ... );

/**
 * @brief resume a suspended thread
 *
 * Resumes a suspended thread, the function will return when the resumed thread calls CTHREAD_yield or when it exits.
 *
 * Passing parameters to the thread; a variable length set of arguments can be sent to the thread, and the thread will be able to read them.
 * Parameter format is the format specifier, subsequent variable length argument values are then sent to the thread.
 *  
 * Retrieving parameters from the thread:
 * We can access values returned by the thread, once CTHREAD_resume returns. The values passed by the thread are stored in rvalue parameter.
 *
 * Precondition: the thread is in CTHREAD_STATE_SUSPENDED state
 * Postcondition: the thread is in CTHREAD_STATE_RUNNING state
  */
int CTHREAD_resume( CTHREAD *thread, VALUES **rvalue, const char *format, ... );

/**
 * @brief a running thread temporarily suspends execution
 *
 * Precondition: the thread is in CTHREAD_STATE_RUNNING state
 * Postcondition: the thread is in CTHREAD_STATE_SUSPENDED state
 
 
 */
int CTHREAD_yield(VALUES **rvalue, const char *format, ... );

/**
 * @brief waits till the thread has finished 
 * Calls CTHREAD_resume repeatedly unntil the thread finishes.
 */
int CTHREAD_join( CTHREAD *thread, VALUES **rvalue );


/**
 * @brief a running thread sets it return values
 *
 */
int CTHREAD_set_return_value( const char *format, ... );

/**
 * @brief get thread id
 *
 * Precondition: the thread is in any state other than CTHREAD_STATE_INIT
 */
uint32_t CTHREAD_get_tid();

/**
 * @brief get thread state
 */
M_INLINE CTHREAD_STATE CTHREAD_state(CTHREAD *thread) 
{
  return thread->state;
}

/**
 * @}
 */

#endif


