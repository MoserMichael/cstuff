#ifndef __COMMON_PRELOAD_H_
#define __COMMON_PRELOAD_H_


#include <sys/resource.h>
#include <limits.h>

#ifndef WIN32

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <pthread.h>
#include <execinfo.h>
#endif

#include "load.h"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void *) (-1)
#endif

//----------------------------------------------------------
// prefix 
//----------------------------------------------------------

#ifndef __cplusplus 
# define EXPORT_C
# define INLINE_C static inline 
#else
# define EXPORT_C extern "C"
# define INLINE_C inline
#endif

#ifdef __GNUC__
# define STATIC_C __attribute__((unused)) static 
#else 
# define STATIC_C static
#endif


//----------------------------------------------------------
// 32/64 bits 
//----------------------------------------------------------

//----------------------------------------------------------
// access lock
//----------------------------------------------------------
#ifndef WIN32

STATIC_C pthread_mutex_t memlock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP; 

#define MT_LOCK(x) pthread_mutex_lock(x)
#define MT_UNLOCK(x) pthread_mutex_unlock(x)

#else

#define MT_LOCK(x) 
#define MT_UNLOCK(x)

#endif


//----------------------------------------------------------
// thunked functions from runtime library.
//----------------------------------------------------------
#define TRACE_TRACE(msg) \
  write(1, msg, strlen(msg));

#define LOAD_TRACE(msg) \
  write(2, msg, strlen(msg));

#define LOAD_FUNC(name) \
  real_##name = (PFN_##name) dlsym(RTLD_NEXT, #name ); \
  if ( ! real_##name ) { \
    fprintf(stderr,"DBGMEM: can't load %s\n", #name); \
    exit(222); \
  }

#ifdef _TRACE_THUNKS_
#define TRACE_THUNK(msg,add)\
  LOAD_TRACE( #msg );\
  LOAD_TRACE( add );
#else
#define TRACE_THUNK(msg,add)
#endif

#define DECLARE_FUNC(name) \
  static PFN_##name get_##name();

#define MAKE_FUNC(name) \
  static PFN_##name real_##name = 0; \
  static PFN_##name get_##name() { \
  \
  if ( !real_##name) { \
    TRACE_THUNK( #name, "\n" ); \
    dlsym_nesting ++; \
    LOAD_FUNC(name); \
    dlsym_nesting --; \
    TRACE_THUNK( #name, " exit \n" ); \
  } \
  return real_##name; \
}\

//----------------------------------------------------------
// pointer arithmetics 
//----------------------------------------------------------

#define BPTR(ptr) ((unsigned char *) (ptr))
#define BPTR_AT(ptr, offset) (BPTR(ptr) + (size_t) (offset))
#define BPTR_AT_SUB(ptr, offset) (BPTR(ptr) - (size_t) (offset))
#define VVPTR_AT(ptr, offset) ((void **) BPTR_AT(ptr,offset))
#define VVPTR_AT_SUB(ptr, offset) ((void **) BPTR_AT_SUB(ptr,offset))
#define BPTR_DIFF(ptr, ptr2) ((size_t) (ptr) - (size_t) (ptr2))

#define TO_NUM(x) ((size_t) x)
#define IS_POWER_OF_TWO( num ) ((TO_NUM(num) & (TO_NUM(num) - 1)) == 0)
#define DO_ALIGN( num, align) TO_NUM( TO_NUM( TO_NUM(num)  + (align) - 1 ) & (~((align) - 1)) )


static void set_core_unlimited()
{
  struct rlimit l;

  getrlimit( RLIMIT_CORE, &l);
 
  if (l.rlim_cur == RLIM_INFINITY) {
    return; 
  }
  
  if (getuid()==0 || geteuid() == 0) {
    l.rlim_cur = l.rlim_max = RLIM_INFINITY;
  } else {
    if (l.rlim_cur == l.rlim_max) {
      return; // nothing left to do.
    }
    l.rlim_cur = l.rlim_max;
  }

  if (setrlimit( RLIMIT_CORE, &l )) {
    LOAD_TRACE("DBGMEM: Warning: Failed to remove core limit errno \n");
  }
}

#endif

