
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
static int dlsym_nesting;

#include "../common.h"
#include "load.h"




#define FILL_BYTE_MALLOCED_MEMORY 0xDD
#define FILL_BYTE_FREED_MEMORY    0xEE


MAKE_FUNC( malloc );
MAKE_FUNC( realloc );
MAKE_FUNC( free );
MAKE_FUNC( posix_memalign );
MAKE_FUNC( memalign );
MAKE_FUNC( valloc );

EXPORT_C void __attribute__((constructor)) mdbg_init_mem_alloc(void)
{
  get_malloc();
  get_realloc();
  get_free();
  get_posix_memalign();
  get_memalign();
  get_valloc();
  set_core_unlimited();
}

EXPORT_C void __attribute((destructor)) mdbg_cleanup_mem_alloc(void)
{
}

#ifdef EXACT_SIZE

static void init( void *ptr, size_t sz )
{
    if (ptr)
    {
	memset( ptr, FILL_BYTE_MALLOCED_MEMORY, sz );
    }
}

#define INIT(ptr,sz) init(ptr, sz);

#else

static void init( void *ptr )
{
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_MALLOCED_MEMORY, asz );
    }
}

#define INIT(ptr,sz) init(ptr);

#endif	

static void deinit( void *ptr )
{
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_FREED_MEMORY, asz );
    }
}



EXPORT_C void *malloc(size_t sz)
{
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  ret = get_malloc() (sz);

  INIT( ret, sz );

  return ret;
}


EXPORT_C void *realloc( void *ptr, size_t sz )
{
  size_t old_size = 0;
  void *ret;
  
  if (dlsym_nesting) {
    return 0;
  }

  if (ptr) {
    old_size = malloc_usable_size(ptr);
  }

  // memory is freed if size is 0
  if (!sz && ptr) {
    memset(ptr, FILL_BYTE_FREED_MEMORY, old_size);
    return get_realloc() ( ptr, 0 );
  }

  ret = get_realloc() (ptr, sz);
  
  // same memory pointer
  if (ret) {
     size_t nsz;

#ifdef EXACT_SIZE  
     nsz = malloc_usable_size(ret);
#else
     nsz = sz;
#endif     
     
     // if size increased - alloc new memory. (probably incorrect, since old_size is larger than requested memory.
     if (nsz > old_size && ret) {
       memset( BPTR_AT(ret, old_size), FILL_BYTE_MALLOCED_MEMORY, nsz - old_size);
     }
  } 

  return ret;

}

EXPORT_C void free(void *ptr)
{
  if (dlsym_nesting) {
    return;
  }
  
  deinit( ptr );
  
  get_free() (ptr);
} 

EXPORT_C int posix_memalign(void **memptr, size_t boundary, size_t size)
{
  int ret;

  if (dlsym_nesting) {
     return 0;
  }

  if (! *memptr ) {
     return EINVAL;
  } 

  ret = get_posix_memalign() (memptr, boundary, size);
  if (!ret) {
    INIT( *memptr, size );
  }

  return 0;  
}

EXPORT_C void *valloc(size_t size)
{
  void *ret;

  if (dlsym_nesting) {
     return 0;
  }

  ret = get_valloc() (size);
  if (!ret) {
    INIT( ret, size );
  }
  
  return ret;  
}


EXPORT_C void *memalign(size_t boundary, size_t size)
{
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  ret = get_memalign() (boundary, size);
  
  if (ret) {
    INIT( ret, size );
  }

  return ret;  
}

#ifdef  __cplusplus

void* operator new(size_t sz) 
{
  void *ret;

  ret = get_malloc() (sz);

  INIT(ret, sz);

  return ret;
}


void* operator new[] (size_t sz) 
{
  void *ret;

  ret = get_malloc() (sz);

  INIT( ret, sz );

  return ret;
}


void operator delete (void *ptr) 
{
  deinit( ptr );
  
  get_free() (ptr);
}


void operator delete[] (void *ptr) 
{
  deinit( ptr );

  get_free() (ptr); 
}

#endif


