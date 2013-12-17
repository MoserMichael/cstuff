/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __CBARRIER_UTIL_X_Y_Z_
#define __CBARRIER_UTIL_X_Y_Z_

#include <pthread.h>
#include <semaphore.h>

/**
 @defgroup CBARRIER
 @ingroup  THREADPOOL

 @brief synchronization point for a fixed party of threads.

 Allows a fixed set of threads to wait for each other until they reach
 a common synchronization point. It is called cyclic becaus this object
 can be used several times (after calling CYCLIC_BARRIER_reset)

 A common synchronization point means that all threads in this party
 must call CYCLIC_BARRIER_await, and all threads block untill the others have called this function

 Idea is taken from java.util.concurrent.CyclicBarier - without waiting with timeouts that is.
 
 Funny that pthread already seems to have it: there is pthread_barrier_init / etc. 
 At the time of writing this code I did not know this, so this is implemented by means of semaphores and mutexes.
 Another lesson learned: always look at the header files, not at the man pages ;-)

 However pthread_barrier has no way to cancel/reset blocked thread; this one has this feature (CYCLIC_BARRIER_reset);
 Feature of canceling barrier is needed if there was a failure and we have to wrap up.

 @{
 */
typedef struct {

  pthread_mutex_t mutex;
  sem_t blocked;
  sem_t in_await_blocked;

  int  parties;  // number of threads that take part (set in init method).
  int left;	 // how many parties still have to call 'await'
  int is_broken; // reset has been called; the instance is in 'broken state'.
  int in_await;	 // are we finished ?
} CYCLIC_BARRIER;


/** 

  @brief initialises a new cyclic barrier

  Returns 0 on success -1 on failure.

  @param cond object to initialise
  @param num number of parties (threads) to synchronze.
 */
int CYCLIC_BARRIER_init(CYCLIC_BARRIER *cond, int num);

/**

  @brief Frees a cyclic barrier.

  Precondition, all threads must have reached common synchronization point (all need to
  have called CYCLIC_BARRIER_await.
 
  @param cond - condition object
 
 */
int  CYCLIC_BARRIER_free(CYCLIC_BARRIER *cond);

/**
  @brief All threads call this function, nobody continues untill all have called this function
 
  @param cond - condition object
 */
int  CYCLIC_BARRIER_await(CYCLIC_BARRIER *cond);


/**
 @brief Returns cyclic barrier to it's initial state.

 Precondition, all parties are part CYCLIC_BARRIER_await.
 Postcondition, can use this cyclic barrier again (i.e. set of parties can do CYCLIC_BARRIER_await again).

 @param cond - condition object

 */
int  CYCLIC_BARRIER_reset(CYCLIC_BARRIER *cond);

/**
 @}
 */

//------------------------------------------------------------------
/*
#ifdef _DO_DEBUG
#include <stdio.h>
#define DEBUGT(format, ... ) fprintf( stderr, "\t[%d] " format, getpid(), __VA_ARGS__ );
#else
#define DEBUGT(format, ... )
#endif
*/

#endif

