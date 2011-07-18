/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *test_t(void *arg)
{
  sleep(1);

  pthread_mutex_lock( &count_mutex );
  fprintf(stderr,"signal - before signal\n");
  pthread_cond_signal( &condition_var );
  fprintf(stderr,"signal - after signal\n");
  pthread_mutex_unlock( &count_mutex );

  return 0;
}


int main(int argc, char *argv[])
{
  pthread_t th;

  pthread_create( &th, 0, test_t, 0 );

  pthread_mutex_lock( &count_mutex );
  fprintf(stderr,"signal - before wait\n");
  pthread_cond_wait( &condition_var, &count_mutex );
  fprintf(stderr,"signal - after wait\n");
  pthread_mutex_unlock( &count_mutex );

  return 0;
}
