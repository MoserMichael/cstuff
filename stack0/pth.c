/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <syscall.h>
#include <linux/unistd.h>

pthread_key_t tkey;

pid_t gettid() { return  syscall( __NR_gettid ); }

void cancel_it(void *arg)
{
  (void) arg;
  fprintf(stderr,"thread local destructor pid %d tid %d ppid %d tid %ld \n", getpid(), gettid(), getppid(), pthread_self() );
}

void *do_it(void *arg)
{
  (void) arg;
  fprintf(stderr,"Thread function pid %d tid %d ppid %d %ld\n",getpid(), gettid(), getppid(), pthread_self());
  
  pthread_setspecific( tkey, malloc(20) );
  return 0;
}

int main()
{
  pthread_t pth;

  fprintf(stderr,"Main thread pid %d tid %d ppid %d tid %ld\n", getpid(), gettid(), getppid(), pthread_self());

  if (pthread_key_create( &tkey, cancel_it ) ) {
    fprintf(stderr,"pthread_key create failed. %d\n", errno );
  }
  if (pthread_create( &pth, 0, do_it, 0) ) {
    fprintf(stderr,"pthread not created. %d\n", errno );

  }
  pthread_setspecific( tkey, malloc(20) );

  sleep(10);
  return 0;
}
