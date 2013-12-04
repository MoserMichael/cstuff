
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

int dlsym_nesting;

#include "memalloc.h"
#include "load.h"


typedef int    (*PFN_posix_memalign) (void **m, size_t boundary, size_t size);

typedef void * (*PFN_memalign) (size_t boundary, size_t size);

typedef void * (*PFN_valign) (size_t size);


MAKE_FUNC( malloc );
MAKE_FUNC( realloc );
MAKE_FUNC( free );
MAKE_FUNC( calloc );
MAKE_FUNC( mallopt );
MAKE_FUNC( posix_memalign );
MAKE_FUNC( memalign );
#if 0
MAKE_FUNC( valign );
#endif

EXPORT_C void __attribute__((constructor)) mdbg_init_mem_alloc(void)
{
  get_malloc();
  get_realloc();
  get_calloc();
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

#define GUARD_AREA 10 

#if 0
void init_guard(void *ret, size_t size)
{
   size_t sz; 
   
   if (ret) {
    sz = malloc_usable_size(ret);
    if (sz > size) {
      size_t diff = sz - size;
      
#if 1      
      if (diff > GUARD_AREA) {
	diff = GUARD_AREA; 
      }
#endif

      memset( BPTR_AT(ret, size), FILL_BYTE_MALLOCED_MEMORY, diff);  
    }
  } 
}
#endif

EXPORT_C void *malloc(size_t sz)
{
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  ret = get_malloc() (sz);

  if (ret) {
    memset(ret, FILL_BYTE_MALLOCED_MEMORY, sz);
  }

  //init_guard(ret,sz);

  return ret;
}


EXPORT_C void *realloc( void *ptr, size_t sz )
{
  size_t old_size; 
  void *ret;
  
  if (dlsym_nesting) {
    return 0;
  }

  // memory is freed if size is 0
  if (!sz && ptr) {
    memset(ptr, FILL_BYTE_FREED_MEMORY, sz);
  }

  if (ptr) {
    old_size = malloc_usable_size(ptr);
  }

  ret = get_realloc() (ptr, sz);
  
  if (ptr) {
    // if size increased - alloc new memory. (probably incorrect, since old_size is larger than requested memory.
    if (sz > old_size && ret) {
      memset( BPTR_AT(ret, old_size), FILL_BYTE_MALLOCED_MEMORY, sz - old_size);
    }
  } else {
     // newly allocated memory will be inited; (ptr is 0) 
    if (ret) {
      memset(ret, FILL_BYTE_MALLOCED_MEMORY, sz);
    }
  }

  //init_guard(ret,sz);

  return ret;  
}

void * calloc (size_t count, size_t size )
{
  void * ret;
 
  if (dlsym_nesting) {
    return 0;
  }
  
  ret = get_calloc() (count, size); 
  
  //init_guard(ret, size );
  return ret;
}

EXPORT_C void free(void *ptr)
{
  size_t sz;

  if (dlsym_nesting) {
    return;
  }
  
  // ups, don't have the size.
  if (ptr) {
    
    // might do more than required - malloc_usable_size can return more than was requested.
    sz = malloc_usable_size(ptr);
    memset(ptr, FILL_BYTE_FREED_MEMORY, sz);
  }
  
  get_free() (ptr);
} 

EXPORT_C int mallopt(int arg, int value )
{
    if (dlsym_nesting) {
      return 0;
    }
    
    if (arg) {
	return 1;
	} 	
    
    return get_mallopt() (arg, value );
}

EXPORT_C int posix_memalign(void **memptr, size_t boundary, size_t size)
{
  int ret;

  if (!memptr) {
     return EINVAL;
  } 

  ret = get_posix_memalign() (memptr, boundary, size);
  if (!ret) {
    memset(*memptr, FILL_BYTE_MALLOCED_MEMORY, size);
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
    memset(ret, FILL_BYTE_MALLOCED_MEMORY, size);
  }

  return ret;  
}

#if 0
EXPORT_C void *valign(size_t size)
{
  void *ret;

  if (dlsym_nesting) {
    return 0;
  }

  ret = get_valign() (size);
  
  if (ret) {
    memset(ret, FILL_BYTE_MALLOCED_MEMORY, size);
  }

  return ret;  
}
#endif


