#include "internal.h"

#ifdef __CYGWIN__
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <alloca.h>
#include <assert.h>
#include <butils/logg.h>


static uint32_t page_size = (uint32_t) -1;
static STACK_DIR stack_dir;
static int       has_stack_dir = 0;

uint32_t get_page_size()
{
  if (page_size != (uint32_t) -1) {
    return page_size;
  }

#ifdef __CYGWIN__
  SYSTEM_INFO sSysInfo;         // useful information about the system

  GetSystemInfo(&sSysInfo);     // initialize the structure

  page_size = sSysInfo.dwPageSize;
#else
  page_size =  sysconf( _SC_PAGE_SIZE );
#endif
  MLOG_TRACE( "page size %d", page_size);

  return page_size;
}

size_t next_frame()
{
   int *a = alloca( 10 );
   return (size_t) a;
}   

STACK_DIR get_stack_direction()
{
  size_t next_f;

  if (has_stack_dir) {
    return stack_dir;
  }
  
  int *a = alloca( 10 );

  next_f = next_frame(); 
   
  has_stack_dir = 1;
  if ((size_t) a > next_f ) {
    stack_dir = STACK_DIR_GROWING_DOWN; 
    MLOG_TRACE( "Stack growing downwards");
  } else {
    assert( (size_t) a < next_f );
    stack_dir = STACK_DIR_GROWING_UP;
    MLOG_TRACE( "Stack growing upwards");
  }
  has_stack_dir = 1;

  return stack_dir;
}

