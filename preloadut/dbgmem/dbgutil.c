
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

#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>


#include "memalloc.h"
#include <dbgmemutil.h>
#include <dlfcn.h>
#include <errno.h>
#include <malloc.h>

typedef size_t  (*PFN_DBGMEM_CHECK_STRING_RANGE) (const void *arg_ptr, size_t str_bound, size_t add_length, DBGMEM_MACCESS_TYPE_STR atype, const char *function_name);


typedef size_t  (*PFN_DBGMEM_CHECK_MEMORY_RANGE) (const void *arg_ptr, size_t length,  DBGMEM_MACCESS_TYPE atype, const char *function_name);

typedef int (*PFN_DBGMEM_GET_BLOCK_SIZE) ( const void *arg, size_t *ret_size );


#define DECL_LIB(libname) \
  void *libname_##libname;


#define LOAD_FUNC_LIB(name,libname) \
  if (!real_##name) { \
    real_##name = (PFN_##name) dlsym( libname_##libname, #name); \
    if ( ! real_##name ) { \
      fprintf(stderr,"DBGMEM: can't load %s errno %d\n", #name, errno); \
      exit(222); \
    } \
  }

#define LOAD_LIB(libname) \
  if ( !libname_##libname) { \
    libname_##libname = dlopen( #libname ".so" ,RTLD_NOW); \
    if ( !libname_##libname) { \
      fprintf(stderr,"DBGMEM: can't load library %s errno %d\n", #libname, errno); \
      exit(223);\
    } \
  } \

#define MAKE_FUNC_LIB(name,libname) \
  static PFN_##name real_##name; \
  static PFN_##name get_##name() { \
  \
  LOAD_LIB(libname);\
  LOAD_FUNC_LIB(name,libname); \
  \
  return real_##name; \
}
 
DECL_LIB(libdmemc)

MAKE_FUNC_LIB(DBGMEM_CHECK_STRING_RANGE,libdmemc)
MAKE_FUNC_LIB(DBGMEM_CHECK_MEMORY_RANGE,libdmemc)
MAKE_FUNC_LIB(DBGMEM_GET_BLOCK_SIZE,libdmemc)

DBGMEM_STATUS DBGMEM_is_installed()
{
  const char * eng = getenv("DBGMEM_ENGINE");
  if (!eng) {
    return DBGMEM_NOT_IN_USE;
  }
  
  if (eng[0] == 's') {
    return DBGMEM_SIMPLE_TOOL;
  }
 
  if (eng[0] == 'c') {
    return DBGMEM_CHECK_TOOL;
  }

  return DBGMEM_NOT_IN_USE;
}

int DBGMEM_dump_memory()
{
  if (DBGMEM_is_installed()) {
    mallopt(DUMP_MEMORY,0);
    return 0;
  }
  return -1;
}

int DBGMEM_check_heap()
{
  if (DBGMEM_is_installed()) {
    return mallopt(CHECK_MEMORY,0);
  }
  return 0;
}

int DBGMEM_increase_generation_tag()
{
  if (DBGMEM_is_installed()) {
    mallopt(INC_GENERATION_VAL,0);
    return 0;
  }
  return -1;
}



size_t  DBGMEM_CHECK_STRING(const void *arg_ptr, size_t str_bound, size_t str_length,  DBGMEM_MACCESS_TYPE_STR atype, const char *function_name)
{
      return get_DBGMEM_CHECK_STRING_RANGE() ( arg_ptr, str_bound, str_length, atype, function_name ); 
}

size_t DBGMEM_CHECK_MEMORY(const void *arg_ptr, size_t length,  DBGMEM_MACCESS_TYPE atype, const char *function_name)
{
  return get_DBGMEM_CHECK_MEMORY_RANGE() ( arg_ptr, length, atype, function_name ); 
}


int DBGMEM_GET_SIZE( const void *arg, size_t *ret_size)
{
  return get_DBGMEM_GET_BLOCK_SIZE() ( arg, ret_size ); 
}



