#ifndef __TSTART_H__
#define __TSTART_H__

#include <pthread.h>

/** 
  @brief create a new thread - but detached
  Works like pthread_create, just makes sure that the thread is detached;
  Never understood why detached is not the default in pthreads; Tsss...
 */
int pthread_create_detached(pthread_t *thread, pthread_attr_t *attr,
                                 void *(*start_routine) (void *), void *arg);



/**
 * &brief returns number of cores on the system
 */
int get_num_cores();

#endif
