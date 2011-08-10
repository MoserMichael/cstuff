/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __TQUEUE__H_Y_Z_
#define __TQUEUE__H_Y_Z_


#include <cutils/dlist.h>
#include <pthread.h>

typedef struct tagTQUEUE {
  
  DLIST  dlist;
  size_t max_count;
  size_t waiting_empty;

  pthread_mutex_t mutex;
  pthread_cond_t  cond_empty;
  pthread_cond_t  cond_max;

} TQUEUE;


int   TQUEUE_init(TQUEUE *queue, size_t max_count );
int   TQUEUE_free(TQUEUE *queue );
int   TQUEUE_push_block_on_queue_full(TQUEUE *queue, void *entry);
int   TQUEUE_push_fail_on_queue_full(TQUEUE *queue, void *entry);
int   TQUEUE_push_exit_message(TQUEUE *queue);
void *TQUEUE_pop(TQUEUE *queue);
int   TQUEUE_pop_non_blocking(TQUEUE *queue, void **rret);


#endif

