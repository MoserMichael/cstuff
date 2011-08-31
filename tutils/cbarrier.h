/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __CBARRIER_UTIL_X_Y_Z_
#define __CBARRIER_UTIL_X_Y_Z_

#include <pthread.h>

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
 
 @{
 */
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int initial_count;
  int tcount;
  int is_finished;
  int left;
} CYCLIC_BARRIER;


/** 

  @brief initialises a new cyclic barrier

  @param cond object to initialise
  @param num number of parties (threads) to synchronze.
 */
void CYCLIC_BARRIER_init(CYCLIC_BARRIER *cond, int num);

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

