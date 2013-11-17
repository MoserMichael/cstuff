/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <pthread.h>
#include <sched.h>

#include "cbarrier.h"
#include <butils/errorp.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

//------------------------------------------------------------------
int CYCLIC_BARRIER_init(CYCLIC_BARRIER *cond, int num)
{
    int rt;

    cond->parties = num;
    cond->left = num;
    cond->is_broken = 0;
    cond->in_await = 0;

    if ((rt = sem_init( &cond->blocked, 0, 0 )) != 0 ) {
       errorp( rt, "sem_init init failed");
       return -1;
    }

    if ((rt = sem_init( &cond->in_await_blocked, 0, 0 )) != 0 ) {
       errorp( rt, "sem_init init failed");
       return -1;
    }


    rt = pthread_mutex_init(&cond->mutex,0);
    if (rt != 0) {
       errorp( rt, "pthread_mutex init failed");
       return -1;
    }

    return 0;
}

int CYCLIC_BARRIER_await(CYCLIC_BARRIER *cond)
{
    int rt = 0;
    int finished = 0;
    int in_harms_way;

    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }

    cond->in_await ++;
    if (cond->left > 0) {
      cond->left --;
    }

    finished = cond->left == 0;

    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
      return -1;
    }

    if (!finished) {
	while( sem_wait( &cond->blocked ) == -1 && errno == EINTR);
    } else {
        int i;

	if (!cond->is_broken) {
	
	    for( i = 0; i < (cond->parties - 1); i++ ) {
		if (sem_post( &cond->blocked ) ) {
		    errorp( -1 , "CYCLIC_BARRIER: sem_post failed %d", i );
		}
	    }
	}	    
    }

    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }

    -- cond->in_await;
    in_harms_way = cond->in_await == 0;

    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
      return -1;
    }

    if (in_harms_way) {
	sem_post( &cond->in_await_blocked ); 
    }

    

    return rt;
}


int  CYCLIC_BARRIER_reset(CYCLIC_BARRIER *cond)
{
    int rt;
    int i;
	
    if ((rt = pthread_mutex_lock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex lock failed");
      return -1;
    }

    cond->left = 0;
    cond->is_broken = 1;
    
    if ((rt = pthread_mutex_unlock(&cond->mutex)) != 0) {
      errorp(rt, "CYCLIC_BARRIER: pthread_mutex_unlock failed");
      return -1;
    }

    for( i=0; i < cond->parties - 1 ; i++ ) {
	sem_post( &cond->blocked );
    }	

    return 0;
}

int CYCLIC_BARRIER_free(CYCLIC_BARRIER*cond)
{
    int rt = 0;
   
    CYCLIC_BARRIER_reset( cond );

    // wait till everybody is out of await.
    while( sem_wait( &cond->in_await_blocked ) == -1 && errno == EINTR);

    sem_destroy( &cond->blocked );
    pthread_mutex_destroy(&cond->mutex);
 
    return rt;
}






