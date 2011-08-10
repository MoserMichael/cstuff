/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __CBARRIER_UTIL_X_Y_Z_
#define __CBARRIER_UTIL_X_Y_Z_

#include <pthread.h>

//------------------------------------------------------------------
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int tcount;
  int is_finished;
  int left;
} CYCLIC_BARRIER;


void CYCLIC_BARRIER_init(CYCLIC_BARRIER*cond, int num);
int  CYCLIC_BARRIER_free(CYCLIC_BARRIER*cond);
int  CYCLIC_BARRIER_await(CYCLIC_BARRIER *cond);


//------------------------------------------------------------------
#ifdef _DO_DEBUG
#include <stdio.h>
#define DEBUGT(format, ... ) fprintf( stderr, "\t[%d] " format, getpid(), __VA_ARGS__ );
#else
#define DEBUGT(format, ... )
#endif


#endif

