/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <pthread.h>
#include "cbarrier.h"
#include <butils/errorp.h>
#include <stdio.h>

//------------------------------------------------------------------
void CYCLIC_BARRIER_init(CYCLIC_BARRIER *cond, int num)
{
    cond->tcount = num;
    cond->is_finished = 0;
    pthread_mutex_init(&cond->mutex,0);
    pthread_cond_init(&cond->cond, 0);
}

int CYCLIC_BARRIER_free(CYCLIC_BARRIER*cond)
{
    int rt = 0;
    int finished;
    int count; 

    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }
    
    finished = cond->is_finished;
    count = cond->tcount;
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
    }

    if (! finished ) {
      errorp( -1, "CYCLIC_BARRIER: Can't free  %d clients not finished with cyclic barrier.", count );
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
         if ((rt = pthread_cond_broadcast(&cond->cond)) != 0) {
           errorp(rt, "CYCLIC_BARRIER: pthread_cond_broadcast failed");
	 } 
	 cond->is_finished = 1;
       }
    }
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
    }
    return rt;
 }





