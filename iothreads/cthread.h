#ifndef __CTHREAD_H__
#define __CTHREAD_H__


#include <ucontext.h>
#include <stdint.h>
#include <iothreads/stacks.h>


typedef void  (*CTHREAD_PROC) (void *arg);

/**
 * @brief denotes return point from co-routine call.
 */
typedef struct tagCRETURN_POINT
{
  ucontext_t context;
} CRETURN_POINT;


CRETURN_POINT *CRETURN_POINT_init();

/** 
 * @defgroup CTHREAD
 * @brief simple co-routine package
 * 
 * @{
 */
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
  void *ctx;
  void *rvalue;
  CTHREAD_PROC proc;  
  struct tagCTHREAD *prev_thread;
  STACK_ENTRY *stack_entry;  
} CTHREAD;



int CTHREAD_libinit();

/**
 * @brief initialises a new co-routine thread. 
 
 * Postcondition: The new thread is in CTHREAD_STATE_INIT state.

 * @param stacks the thread is obtained from the pool of stacks (argument stack).
 * @param proc The thread procecdure is argument proc.
 * @param ctx argument passed to thread procedure.
 */
CTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc, void *ctx );

/**
 * @brief start a new thread - the thread procedure is invoked.
 *
 * Precondition: the thread is in either CTHREAD_STATE_INIT or CTHREAD_STATE_EXIT state.
 * Postcondition: the thread is in CTHREAD_STATE_RUNNING state.
 *
 * The function returns if the running thread either exited or entered suspended state (i.e. the thread called yield)
 */
int CTHREAD_start( CTHREAD *thread );

/**
 * deallocates a thread 
 *
 * Precondition: the thread must be in CTHREAD_STATE_EXIT state 
 */ 
int CTHREAD_free( CTHREAD *thread );

/**
 * @brief start a new thread - the thread procedure is invoked.
 *
 * Precondition: the thread is in either CTHREAD_STATE_INIT or CTHREAD_STATE_EXIT state.
 * Postcondition: the thread is in CTHREAD_STATE_RUNNING state.
 *
 * The function returns to the context defined in CRETURN_POINT *next argument. the function returns to this spot 
 * if the thread exited or entered suspended state.
 */
int CTHREAD_start_ex( CTHREAD *thread, CRETURN_POINT *next );

/**
 * @brief a running thread temporarily suspends execution
 *
 * Precondition: the thread is in CTHREAD_STATE_RUNNING state
 * Postcondition: the thread is in CTHREAD_STATE_SUSPENDED state
 */
int CTHREAD_yield();

/**
 * @brief resume a suspended thread
 *
 * Precondition: the thread is in CTHREAD_STATE_SUSPENDED state
 * Postcondition: the thread is in CTHREAD_STATE_RUNNING state
  */
int CTHREAD_resume( CTHREAD *thread);

/**
 * @brief running thread sets its return value.
 *
 * Precondition: the thread is in CTHREAD_STATE_RUNNING state
 */
int CTHREAD_set_return_value(void *rvalue);

/**
 * @brief get return value of a thread
 *
 * Precondition: the thread is in CTHREAD_STATE_SUSPENDED or CTHREAD_STATE_EXIT state
 */
int CTHREAD_get_return_value(CTHREAD *thread, void **rvalue);

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


