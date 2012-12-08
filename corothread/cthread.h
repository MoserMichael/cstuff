#ifndef __CTHREAD_H__
#define __CTHREAD_H__


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
  CTHREAD_STATE_INIT = 1,
  CTHREAD_STATE_RUNNING,
  CTHREAD_STATE_SUSPENDED,
  CTHREAD_STATE_EXIT,
} CTHREAD_STATE;


struct tagCTHREAD;

#ifndef _CTHREAD_MAIN_
typedef struct tagCTHREAD CTHREAD;
#endif

 

/**
 * @brief library initialisation
 */
int CTHREAD_libinit();

/**
 * @brief initialises a new co-routine thread. 
 
 * Postcondition: The new thread is initialised and enters INIT state.
 * next thing to do is to start the thread (CTHREAD_start)

 * @param stacks the thread stack is obtained from the pool of stacks
 * @param proc The thread procecdure that will be invoked when the thread is started.
 * @return pointer to new thread on success; 0 on failure (if no stack is available or could not be created)
 */
struct tagCTHREAD * CTHREAD_init( STACKS *stacks, CTHREAD_PROC proc );

/**
 * destroys the thread object. 
 *
 * Precondition: the thread must be in EXIT state; meaning the thread has called CTHREAD_exit or returned from the thread procedure. 
 */ 
int CTHREAD_free( struct tagCTHREAD *thread );

/**
 * @brief start a new thread - the thread procedure is invoked.
     *
 * Precondition: the calling thread is in RUNNING state; the argument thread is in INIT state, CTHREAD_init has been called.
 * Action: the calling thread is in SUPENDED state; the argument thread is in RUNNING state; 
 * Postcondition: the calling thread is in RUNNING state; the argument is either in SUSPENDED state - it called CTHREAD_yield, or in EXIT state - called CTHREAD_exit / or return from thread procedure.
 *
 * @param rvalues -  optional; if not null then will receive pointer to message from the new thread, once the current thread is resumed.
 * @param format -   format string of arguments passed to the new thread, followed by the parameters passed. If NULL then no message is passed to the new thread.
 *
 * @return
 *    0 - the thread has been started, did some work while in RUNNING state, and then called CTHREAD_yield, the thread is now suspended.
 *    1 - the thread has been started, did some work while in RUNNING state, and then called CTHREAD_exit, the thread has exited.
 *   -1 - the thread failed to start.    
 */
int CTHREAD_start( struct tagCTHREAD *thread, VALUES **rvalue, const char *format , ... );

/**
 * @brief a running thread suspends execution; controll passes to the caller of this thread.
 *
 * Precondition: the current thread is in  RUNNING state ; the calling thread is in SUSPENDED state 
 * Action: the current thread is in SUSPENDED state; the calling thread is in RUNNING state.
 * Postcondition: the current thread is in RUNNING state; the calling thread is in SUSPENDED state - it has called CTHREAD_resume; or it requests this thread to exit by having called CTHREAD_kill.
 *
 * @param rvalues -  optional; if not null then will receive pointer to message from the parent thread, once the current thread is resumed.
 * @param format -   format string of message passed to the  parent thread, followed by the parameters passed. If NULL then no message is passed to the parent thread.
 *
 *
 * @return
 *    0 - the calling thread has been resumed, the calling thread has later called CTHREAD_resume
 *
 *    1 - the calling thread has been resumed, the calling thread has later called CTHREAD_kill in order to shut down the current thread;
 *        the current thread must now clean up all resources it owns; the call CTHREAD_exit, or return from the thread function.
 *
 *   -1   the function failed to activate the calling thread.
 */
int CTHREAD_yield(VALUES **rvalue, const char *format, ... );

/**
 * @brief resumes a suspended thread
 *
 * Resumes a suspended thread, the current thread is suspended; the argument thread will be activated and in RUNNING state;
 * The function will return when the resumed thread calls CTHREAD_yield or when it exits (call CTHREAD_exit).
 *
 * Precondition: The current thread is in RUNNING state; the argument thread is in SUSPENDED state
 * Action: the current thread is in SUSPENDED state; the argument thread is in RUNNING state
 * Postcondition: the current thread is in RUNNING state; the argument thread is in SUSPENDed state - called CTHREAD_resume 
 * or it is in EXIT state
 *
 * @param rvalues -  optional; if not null then will receive pointer to message from the argument thread, once the current thread is resumed.
 * @param format -   format string of arguments passed to the argument thread, followed by the parameters passed. If NULL then no message is passed to the new thread.
 *
 * @return 
 *    0 - the thread has been started, did some work while in RUNNING state, and then called CTHREAD_yield, the thread is now suspended.
 *    1 - the thread has been started, did some work while in RUNNING state, and then called CTHREAD_exit, the thread has exited.
 *   -1 - the function failed.
 */
int CTHREAD_resume( struct tagCTHREAD *thread, VALUES **rvalue, const char *format, ... );



/** 
 * @brief kills a thread.
 *
 *
 * The thread which is currently blocked in call to CTHREAD_yield will wake up, CTHREAD_yield will return -1,
 * Upon this event the thread is expected to clean up resources that it owns and call CTHREAD_exit.
 *
 * Does also kill all threads started by the argument thread, 
 *
 * Precondition: the current thread is in RUNNING state, the argument thread is in SUSPENDED state
 * Action: the current thread is in SUSPENDED state, the argument thread is in RUNNING state, it must now free its resources and then call CTHREAD_exit or return from the thread function.
 * Postcondition: the current thread is in RUNNING state, the argument thread is in EXIT state
 */
int CTHREAD_kill(  struct tagCTHREAD *thread );

/**
 * @brief exit the current thread
 * Exits the current thread,
 *
 * Precondition: the current thread is in RUNNING state; the calling thread is in SUSPENDED state
 * Action: stops all threads that were started by the current thread.
 * Postcondition: the current thread is in EXIT state; the calling thread is in RUNNING state. 
 * 
 * @return
 *    0 - function completed
 *   -1 - error.
 */
int CTHREAD_exit();



/**
 * @brief a running thread sets it return values
 *
 */
int CTHREAD_set_return_value( const char *format, ... );

/**
 * @brief get current thread id
 *
 * precondition: a thread is running.
 * @returns current thread id ; -1 if no thread is active.
 */
uint32_t CTHREAD_get_tid();

/**
 * @brief get thread state
 */
CTHREAD_STATE CTHREAD_state(struct tagCTHREAD *thread);


/**
 * @}
 */

#endif


