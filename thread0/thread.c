#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/user.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>

static void * sleeper_thread( void * arg)
{
  (void) arg;

  sleep( 1000000 );
  return 0;
}

int read_proc_val(const char *valname, long *rval)
{
  FILE *fp;
  char buf[120];
  
  fp = fopen(valname, "r");
  if (fp == 0) {
    return -1;
  }

  fread( buf, 1, sizeof(buf), fp);
  *rval =  atol( buf );
  fclose(fp);

  return 0; 
}


int main()
{
  int i, rt;
  long tmaxval;
  pthread_attr_t attr;
  pthread_t pth;
  struct rlimit lm;  	 
	 
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  pthread_attr_setstacksize( &attr, PTHREAD_STACK_MIN );


  rt = getrlimit( RLIMIT_NPROC, &lm );
  if (rt != 0) {
    fprintf(stderr,"failed to get limit. ret %d errno %d\n", rt, errno);
  }

  if (! read_proc_val( "/proc/sys/kernel/threads-max", &tmaxval ) ) {
    fprintf(stderr,"System wide limit on number of threads %ld\n", tmaxval );  
  } else {
    fprintf(stderr,"Can't read system wide limit of threads\n");
  }


  fprintf(stderr,"Process limit of threads:\n\t");
  if (lm.rlim_cur == RLIM_INFINITY ) {
    fprintf(stderr,"soft limit: no-limit " );
  } else {
    fprintf(stderr,"soft limit: %ld ", lm.rlim_cur );
  }

  if (lm.rlim_cur == RLIM_INFINITY ) {
    fprintf(stderr,"hard limit: no-limit\n" );
  } else {
    fprintf(stderr,"hard limit: %ld\n", lm.rlim_max );
  }
  for(i = 0; ; i++) {
      if ((rt = pthread_create( &pth, &attr, sleeper_thread, 0 ) )  != 0) {
	    fprintf(stderr,"pthread_create  failed #%d. errno %d\n", i, errno);
	    fprintf(stderr,"If you want more threads consider raising the system wide limit of thread/process numbers.\n"
			   "As root user do\n"
	                   "  echo 10000 > /proc/sys/kernel/threads-max\n");


	    fprintf(stderr,"All threds created, sleeping...\n");
	    sleep(10);

	    exit(1);
         }
  }

  return 0;
}

