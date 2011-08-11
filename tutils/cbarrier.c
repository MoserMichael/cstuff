/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <pthread.h>
#include <sched.h>

#include "cbarrier.h"
#include <butils/errorp.h>
#include <stdio.h>

static int CYCLIC_BARRIER_wait_for_all_to_finish(CYCLIC_BARRIER *cond);

//------------------------------------------------------------------
void CYCLIC_BARRIER_init(CYCLIC_BARRIER *cond, int num)
{
    int rt;

    cond->initial_count = num;
    cond->tcount = num;
    cond->is_finished = 0;
    cond->left = num;
 
    if ((rt = pthread_mutex_init(&cond->mutex,0)) != 0) {
       errorp( rt, "pthread_mutex init failed");
    }
    if ((rt = pthread_cond_init(&cond->cond, 0)) != 0) {
       errorp( rt, "pthread_cond init failed");
    }
}



int  CYCLIC_BARRIER_reset(CYCLIC_BARRIER *cond)
{
    int rt;

    if (CYCLIC_BARRIER_wait_for_all_to_finish(cond)) {
      return -1;
    }
  
    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }
    
    cond->tcount = cond->initial_count;
    cond->left = cond->initial_count;
    cond->is_finished = 0;
   
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
      return -1;
    }

    return 0;
}

int CYCLIC_BARRIER_free(CYCLIC_BARRIER*cond)
{
    int rt = 0;
   
    if (CYCLIC_BARRIER_wait_for_all_to_finish(cond)) {
      return -1;
    }
    
    if ((rt = pthread_cond_destroy( &cond->cond )) != 0 ) {
      errorp(rt, "CYCLIC_BARRIER: pthread_cond_destroy failed");
    }
    if ((rt = pthread_mutex_destroy( &cond->mutex )) != 0 ) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutext destroy failed");
    }
 
    return rt;
}


 int CYCLIC_BARRIER_await(CYCLIC_BARRIER *cond)
{
    int rt = 0;


    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }
    if (! cond->is_finished ) {
      --cond->tcount;
      if (cond->tcount > 0) {
         if ((rt = pthread_cond_wait(&cond->cond,&cond->mutex)) != 0) {
	   errorp(rt, "CYCLIC_BARRIER: pthread_cond_wait failed");
	 }
      } else {
	 cond-> is_finished = 1;
         if ((rt = pthread_cond_broadcast(&cond->cond)) != 0) {
           errorp(rt, "CYCLIC_BARRIER: pthread_cond_broadcast failed");
	 }
      }
    }

    cond->left--;
    
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
    }

    return rt;
 }


static int CYCLIC_BARRIER_wait_for_all_to_finish(CYCLIC_BARRIER *cond)
{
    int rt, left;
    int finished, count, init_count;
   
    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }
    
    finished = cond->is_finished;
    count = cond->tcount;
    init_count = cond->initial_count;
    
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
    }

    if (!finished) {
      errorp(-1,"Can't finish, not all parties have reached common synchronisation point, %d parties out of %d not finished", count, init_count);
      return -1;
    }


   do {
      if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
        errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
        return -1;
      }
    
      left = cond->left;
    
      if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
        errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
	return -1;
      }

      if (left != 0) {
        sched_yield();
      }

    } while ( left != 0);

    return 0;
}




