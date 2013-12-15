#include <malloc.h>
#include <stdlib.h>
#include <string.h>


void  * test_malloc(int num);

void * test_malloc2(int num)
{
  if (num <= 0) {
    return  (void *) malloc(8);

  } else {
    return test_malloc( num - 1 );
  }

}

void * test_malloc(int num)
{
  return test_malloc2(num);
}

void consume_tdata(void *arg)
{
    (void) arg;
}

void memset_wrap( void *trg, int ch, size_t n)
{
    memset( trg, ch, n);
}

void memcpy_wrap( void *trg, void *src, size_t n)
{
    memcpy( trg, src, n);
}

