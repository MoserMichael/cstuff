
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


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "memalloc.h"
#include "load.h"
#include "sbuf.c"

static int dlsym_nesting;

#define MSG_LEN  512
#define TRACE_FD 2
#define STACK_FRAMES 3
#define FILL_BYTE_MALLOCED_MEMORY 0xDD
#define FILL_BYTE_FREED_MEMORY    0xEE

typedef int    (*PFN_posix_memalign) (void **m, size_t boundary, size_t size);

typedef void * (*PFN_memalign) (size_t boundary, size_t size);

typedef void * (*PFN_valign) (size_t size);


MAKE_FUNC( malloc );
MAKE_FUNC( realloc );
MAKE_FUNC( free );
MAKE_FUNC( posix_memalign );
MAKE_FUNC( memalign );
#if 0
MAKE_FUNC( valign );
#endif

#if 1
static __thread int is_in_malloc = 0;

#define ENTER_MALLOC  is_in_malloc = 1;
#define EXIT_MALLOC   is_in_malloc = 0;
#define TEST_IN_MALLOC  is_in_malloc
#endif

#define TRACE_STACK\
  {\
  ENTER_MALLOC;\
  void *sframes[ STACK_FRAMES + 1 ];\
  int nframes, i;\
\
  nframes = backtrace( sframes, STACK_FRAMES + 1); \
  for(i = 1;i < nframes; i++) { \
    SBUF_add_s(&sbuf,"Frame: "); \
    SBUF_fmt_size_t(&sbuf, i); \
    SBUF_add_s(&sbuf, " " ); \
    SBUF_fmt_ptr(&sbuf, sframes[ i ]); \
    SBUF_add_s(&sbuf, "\n" ); \
  } \
  EXIT_MALLOC; \
  }


EXPORT_C void __attribute__((constructor)) mdbg_init_mem_alloc(void)
{
  get_malloc();
  get_realloc();
  get_free();
  get_posix_memalign();
  get_memalign();
#if 0  
  get_valign();
#endif
}

EXPORT_C void __attribute((destructor)) mdbg_cleanup_mem_alloc(void)
{
}

static void init( void *ptr )
{
#ifdef ZERO_MEM
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_MALLOCED_MEMORY, asz );
    }
#else
    (void) ptr;
#endif    
}


static void deinit( void *ptr )
{
#ifdef ZERO_MEM
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_FREED_MEMORY,Y, asz );
    }
#else
    (void) ptr;
#endif    
}



EXPORT_C void *malloc(size_t sz)
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];


  if (TEST_IN_MALLOC)
  {
    return get_malloc() (sz);
  }
  ret = get_malloc() (sz);

  init( ret );

  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "malloc size=");
  SBUF_fmt_size_t(&sbuf, sz );
  SBUF_add_s(&sbuf, " ptr=0x");
  SBUF_fmt_ptr(&sbuf, ret );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


  return ret;
}


EXPORT_C void *realloc( void *ptr, size_t sz )
{
  size_t old_size = 0;
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  if (ptr) {
    old_size = malloc_usable_size(ptr);
  }

  // memory is freed if size is 0
  if (!sz && ptr) {
#ifdef ZERO_MEM
    memset(ptr, FILL_BYTE_FREED_MEMORY, old_size);
#endif  

    SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
    SBUF_add_s(&sbuf, "free by realloc inptr=0x");
    SBUF_fmt_ptr(&sbuf, ptr );

    TRACE_STACK

    write(TRACE_FD, sbuf.buf, sbuf.pos );
    
    return get_realloc() ( ptr, 0 );
  }

  ret = get_realloc() (ptr, sz);
 
  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "realloc size=");
  SBUF_fmt_size_t(&sbuf, sz );
  SBUF_add_s(&sbuf, " inptr=0x");
  SBUF_fmt_ptr(&sbuf, ptr );
  SBUF_add_s(&sbuf, " ptr=0x");
  SBUF_fmt_ptr(&sbuf, ret );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


#ifdef ZERO_MEM
  // same memory pointer
  if (ret) {
     size_t nsz = malloc_usable_size(ret);
     
     // if size increased - alloc new memory. (probably incorrect, since old_size is larger than requested memory.
     if (nsz > old_size && ret) {
       memset( BPTR_AT(ret, old_size), FILL_BYTE_MALLOCED_MEMORY, nsz - old_size);
     }
  } 
#endif

  return ret;

}

EXPORT_C void free(void *ptr)
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  
  deinit( ptr );
  
  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "free ptr=0x");
  SBUF_fmt_ptr(&sbuf, ptr );
 
  get_free() (ptr);
} 

EXPORT_C int posix_memalign(void **memptr, size_t boundary, size_t size)
{
  int ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  if (! *memptr ) {
     return EINVAL;
  } 

  ret = get_posix_memalign() (memptr, boundary, size);
  if (!ret) {
    init( *memptr );
  }

  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "posix_memalign size=");
  SBUF_fmt_size_t(&sbuf, size );
  SBUF_add_s(&sbuf, " boundary=");
  SBUF_fmt_size_t(&sbuf, boundary);
  SBUF_add_s(&sbuf, " ret=0x");
  SBUF_fmt_ptr(&sbuf, *memptr );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


  return 0;  
}

EXPORT_C void *memalign(size_t boundary, size_t size)
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 

  ret = get_memalign() (boundary, size);
  
  if (ret) {
    init( ret );
  }
  
  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "posix_memalign size=");
  SBUF_fmt_size_t(&sbuf, size );
  SBUF_add_s(&sbuf, " boundary=");
  SBUF_fmt_size_t(&sbuf, boundary);
  SBUF_add_s(&sbuf, " ret=0x");
  SBUF_fmt_ptr(&sbuf, ret );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


  return ret;  
}

#ifdef  __cplusplus

void* operator new(size_t sz) 
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  ret = get_malloc() (sz);

  init( ret );

  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "new size=");
  SBUF_fmt_size_t(&sbuf, sz );
  SBUF_add_s(&sbuf, " ptr=0x");
  SBUF_fmt_ptr(&sbuf, ret );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


  return ret;
}


void* operator new[] (size_t sz) 
{
  void *ret;
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  ret = get_malloc() (sz);

  init( ret );

  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "new[] size=");
  SBUF_fmt_size_t(&sbuf, sz );
  SBUF_add_s(&sbuf, " ptr=0x");
  SBUF_fmt_ptr(&sbuf, ret );
  SBUF_add_s(&sbuf, "\n" );

  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );


  return ret;
}


void operator delete (void *ptr) 
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  deinit( ptr );
  
  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "delete ptr=0x");
  SBUF_fmt_ptr(&sbuf, ptr );
 
  TRACE_STACK
 
  write(TRACE_FD, sbuf.buf, sbuf.pos );
 
  get_free() (ptr);
}


void operator delete[] (void *ptr) 
{
  SBUF sbuf;
  char sbuf_mem[ MSG_LEN ];
 
  deinit( ptr );
  
  SBUF_init( &sbuf, sbuf_mem, sizeof(sbuf_mem) );
  SBUF_add_s(&sbuf, "delete[] ptr=0x");
  SBUF_fmt_ptr(&sbuf, ptr );
 
  TRACE_STACK

  write(TRACE_FD, sbuf.buf, sbuf.pos );

  get_free() (ptr); 
}

#endif


