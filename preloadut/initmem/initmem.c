
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

#include "memalloc.h"
#include "load.h"




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

void init( void *ptr )
{
    if (ptr)
    {
	size_t asz =  malloc_usable_size( ptr );
	memset( ptr, FILL_BYTE_MALLOCED_MEMORY, asz );
    }
}

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

  init( ret );

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
     size_t nsz = malloc_usable_size(ret);
     
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
    init( *memptr );
  }

  return 0;  
}

EXPORT_C void *memalign(size_t boundary, size_t size)
{
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  ret = get_memalign() (boundary, size);
  
  if (ret) {
    init( ret );
  }

  return ret;  
}

#ifdef  __cplusplus

void* operator new(size_t sz) 
{
  void *ret;

  ret = get_malloc() (sz);

  if (ret) {
    memset(ret, FILL_BYTE_MALLOCED_MEMORY, malloc_usable_size(ret) );
  }

  return ret;
}


void* operator new[] (size_t sz) 
{
  void *ret;

  ret = get_malloc() (sz);

  init( ret );

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


