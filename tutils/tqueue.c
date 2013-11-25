/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include "tqueue.h"
#include <errno.h>
#include <stdio.h>
#include <butils/errorp.h>

typedef struct tagTQUEUE_Entry {
  DLIST_entry  dlist;
  void *entry;
} TQUEUE_Entry;


int TQUEUE_init(TQUEUE *queue, size_t max_count )
{
  pthread_mutex_init( &queue->mutex, 0 );
  pthread_cond_init( &queue->cond_empty, 0 );
  pthread_cond_init( &queue->cond_max, 0 );

  DLIST_init( &queue->dlist );

  queue->max_count = max_count;  
  queue->waiting_empty = 0;
  queue->waiting_full = 0; 
  return 0;
}

int   TQUEUE_free(TQUEUE *queue )
{
  pthread_cond_destroy( &queue->cond_empty );
  pthread_cond_destroy( &queue->cond_max );
  pthread_mutex_destroy( &queue->mutex );
  return 0;
}

static int push_entry(DLIST *lst, void *data )
{
  TQUEUE_Entry *entry = (TQUEUE_Entry *) malloc( sizeof(TQUEUE_Entry) );
  if (!entry) {
    return -1;
  }
  entry->entry = data;
  DLIST_push_front( lst, (DLIST_entry *) entry );
  return 0;
}

int   TQUEUE_push_exit_message(TQUEUE *queue)
{
  int rt = 0,r;
  TQUEUE_Entry *entry; 

  if ((r = pthread_mutex_lock(&queue->mutex)) != 0) {
    errorp(r, "pthread_mutex_lock failed");
    return -1;
  }
  entry = (TQUEUE_Entry *) malloc( sizeof(TQUEUE_Entry) );
  entry->entry = 0;
  
  if (entry) {
    DLIST_push_back( &queue->dlist, (DLIST_entry *) entry );
    if (queue->waiting_empty > 0) {
      if ((r = pthread_cond_signal( &queue->cond_empty )) != 0) {
	errorp(r, "pthread_cond_signal failed");
      }
    }
  } else {
    rt = -1;
  }

  if ((r = pthread_mutex_unlock( &queue->mutex )) != 0) {
    errorp( r, "pthread_mutex_unlock failed");
    return -1;
  }
   
  return rt;
}

int TQUEUE_push_block_on_queue_full(TQUEUE *queue, void *entry)
{
  int r;

  if ((r = pthread_mutex_lock(&queue->mutex)) != 0 ) {
    errorp(r,"pthread_mutex_lock failed");
    return -1;
  }
  int rt = 0;

  if (queue->max_count != 0 &&
      DLIST_size( &queue->dlist ) >= queue->max_count) {
	
      queue->waiting_full++;
      if ((r = pthread_cond_wait( &queue->cond_max, &queue->mutex )) != 0 ) {
	errorp(r,"pthread_cond_wait failed");
      }
      queue->waiting_full++;
  }
  
  if (!push_entry( &queue->dlist, entry )) {
    if (queue->waiting_empty > 0) {
      if ((r = pthread_cond_signal( &queue->cond_empty )) != 0)  {
	errorp(r, "pthread_cond_signal failed");
      }
    }
  } else {
    rt = -1;
  }

  if ((r = pthread_mutex_unlock( &queue->mutex )) != 0) {
    errorp(r, "pthread_mutex_unlock fails");
    return -1;
  }
  return rt;

}

int TQUEUE_push_fail_on_queue_full(TQUEUE *queue, void *entry)
{
  int rt = 0, r;

  if ((r = pthread_mutex_lock(&queue->mutex)) != 0) {
    errorp(r, "pthread_mutex_lock failed");
    return -1;
  }

  if (queue->max_count != 0 &&
    DLIST_size( &queue->dlist ) >= queue->max_count) {
    rt = -1;
  } else {
  
    if (! push_entry( &queue->dlist, entry )) {
      if (queue->waiting_empty > 0) {
         rt = pthread_cond_signal( &queue->cond_empty );
         if (rt) {
	   errorp(rt, "pthread_cond_signal failed");
	 } 
      }
    } else {
      rt = -1;
    }
  }
  if ((r = pthread_mutex_unlock( &queue->mutex )) != 0) {
    errorp(r, "pthread_mutex_unlock failed");
    return -1;
  }

  return rt;
}


int   TQUEUE_pop_non_blocking(TQUEUE *queue, void **rret)
{
    TQUEUE_Entry  *entry = 0;
    int rt;

    if (pthread_mutex_lock(&queue->mutex)) {
      perror("pthread_mutex_lock failed");
      return -1;
    }
    if (!DLIST_isempty( &queue->dlist)) {
        entry = (TQUEUE_Entry  *) DLIST_pop_back( &queue->dlist );
	rt = 0;
    } else {
        rt = -1;
    }	
    pthread_mutex_unlock( &queue->mutex );

    if (entry) {
      *rret = entry->entry;
      free(entry);
    }

    return rt;
}

void *TQUEUE_pop(TQUEUE *queue)
{
    TQUEUE_Entry  *entry;
    void *ret  = 0;
    int rt;

    if (pthread_mutex_lock(&queue->mutex)) {
      perror("pthread_mutex_lock failed");
      return 0;
    }

    if (DLIST_isempty( &queue->dlist ) ) {
         queue->waiting_empty ++;
         rt = pthread_cond_wait( &queue->cond_empty, &queue->mutex );
	 if (rt) {
	   perror("pthread_cond_wait failed");
	 }
         queue->waiting_empty --;
    }

    entry = (TQUEUE_Entry  *) DLIST_pop_back( &queue->dlist );
    if (entry) {
      ret = entry->entry;
      free(entry);
    }


    if (queue->waiting_full != 0) {
        rt = pthread_cond_signal( &queue->cond_max );
    	if (rt) {
	   perror("pthread_cond_signal failed");
        }
    } 

    pthread_mutex_unlock( &queue->mutex );

    return ret;
}



