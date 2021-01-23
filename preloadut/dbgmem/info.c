
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * 3-clause BSD License applies  
 *
 * See LICENSE file for the full copy of the license
 *
 */

#include <limits.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>

size_t next_frame()
{
  int idx;
  size_t ret = (size_t) &idx;
  return ret;
}


int main()
{
  char top_stack;
  struct rlimit r;
  size_t sz;
  pthread_attr_t attr;
  int err; 
  size_t ptr;
  char *val;
  int a,b,d;
  
  printf("\n\n");
#ifdef __WORDSIZE
  printf("word size: %d\n", __WORDSIZE );
#endif
  printf("top_stack %p\n", &top_stack);
  
  getrlimit( RLIMIT_STACK, &r );
  
  printf("rlimit_stack cur: %ld max: %ld\n", r.rlim_cur, r.rlim_max );
  
  err = pthread_attr_getstacksize( &attr, &sz );
  if (err) {
    printf("pthread_attr_getstacksize %zd\n", sz);
  } else {
    printf("no pthread_attr_getstacksize\n");
  }

  ptr = next_frame();
  printf("stack is growing %s\n",
    (ptr < (size_t) &top_stack) ? "down" : "up");
   
  val = (char *) malloc(10);
  
  a = malloc_usable_size(val);
  b = malloc_usable_size(val+1);
  // this one crashes on gcc 2.96
  //c = malloc_usable_size(val-1);
  d =  malloc_usable_size(val+20);
  
  printf("malloc_usable_size\nok %d\nfalse offset: %d\nfalse offset: %d\n",
    a, b, d);
  
     
  printf("\n");
  return 0;
}
