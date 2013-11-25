#include "tstart.h"
#include <unistd.h>

int pthread_create_detached(pthread_t *thread, pthread_attr_t *attr,
                                 void *(*start_routine) (void *), void *arg)
{
    
    pthread_attr_t *pattr, real_attr;
    int rt;

    if (!attr) {
      pthread_attr_init( &real_attr );
      pthread_attr_setdetachstate( &real_attr, PTHREAD_CREATE_DETACHED );
      pattr = &real_attr;
    } else {
      pattr = attr;
    }	

    rt = pthread_create( thread, pattr, start_routine, arg );

    if (!pattr) {
      pthread_attr_destroy( &real_attr );
    }

    return rt;
}

int get_num_cores()
{
  return  (int) sysconf( _SC_NPROCESSORS_ONLN );
}


