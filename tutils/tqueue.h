/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __TQUEUE__H_Y_Z_
#define __TQUEUE__H_Y_Z_


#include <cutils/dlist.h>
#include <pthread.h>

/** 
 @brief Implements a thread safe queue with maximum limit of requess
 */
typedef struct tagTQUEUE {
  
  DLIST  dlist;
  size_t max_count;
  size_t waiting_empty;

  pthread_mutex_t mutex;
  pthread_cond_t  cond_empty;
  pthread_cond_t  cond_max;

} TQUEUE;


/**
 @brief create new queue 
 */
int   TQUEUE_init(TQUEUE *queue, size_t max_count );

/**
 @brief destroy queue
 */
int   TQUEUE_free(TQUEUE *queue );

/**
 @brief add new entry to queue, block if maximum queue limit has been reached 
*/
int   TQUEUE_push_block_on_queue_full(TQUEUE *queue, void *entry);

/**
 @brief add new entry to queue, fail if maximum queue limit has been reached 
*/
int   TQUEUE_push_fail_on_queue_full(TQUEUE *queue, void *entry);

/**
 @brief push out of order exit message (null message at top of queue)
*/
int   TQUEUE_push_exit_message(TQUEUE *queue);

/**
 @brief pop queue, block if empty
*/
void *TQUEUE_pop(TQUEUE *queue);

/**
 @brief pop queue, return error if empty. Does not block if queue is empty.
*/
int   TQUEUE_pop_non_blocking(TQUEUE *queue, void **rret);


#endif

