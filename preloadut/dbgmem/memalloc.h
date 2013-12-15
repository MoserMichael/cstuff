
/* 
 * DBGMEM - memory allocation leak tracker and debugging tool.
 *
 *  (c) 2008 by Michael Moser
 * 
 * This code is available under the GNU Lesser Public License version 2.1
 *
 * See LICENSE file for the full copy of the license
 *
 */

#ifndef _MEMALLOC_H_
#define _MEMALLOC_H_

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

//----------------------------------------------------------
// mem checker.
//----------------------------------------------------------

#ifndef __WORDSIZE
#error "__WORDSIZE is not defined. very bad."
#endif

#if __WORDSIZE == 32
#define SENTINEL		(void *) 0xACDEACDE
#define SENTINEL_VALIGN	(void *) 0xBEEFFEED
#define SENTINEL_FREED	(void *) 0x10000001
#endif


#if __WORDSIZE == 64
#define SENTINEL		(void *) 0xACDEACDEACDEACDE
#define SENTINEL_VALIGN	(void *) 0xBEEFFEEDBEEFFEED
#define SENTINEL_FREED	(void *) 0x1000000110000001
#endif


#define FILL_BYTE_MALLOCED_MEMORY 0xDD
#define FILL_BYTE_FREED_MEMORY    0xEE

//----------------------------------------------------------
//  arguments for mallopt function.
//----------------------------------------------------------
#define IS_INSTALLED		1000 
#define GET_GENERATION_VAL	1001 
#define INC_GENERATION_VAL	1002
#define CHECK_MEMORY		1003
#define SET_STRACE_ON_OFF	1004
#define DUMP_MEMORY		1005

//----------------------------------------------------------
//  arguments for mallopt function.
//----------------------------------------------------------
#define ALLOC_MALLOC		(1U<<31)
#define ALLOC_NEW               (1U<<30) 
#define ALLOC_NEW_ARRAY         (1U<<29)  

#define ALLOC_GEN_FLAGS         ((1U<<29)-1) 
#define ALLOC_TYPE_FLAGS        (0xFFFFFFFF & ~ALLOC_GEN_FLAGS) 
  
#ifndef  __cplusplus

#define GET_GENERATION(value)	(value) 

#else

#define MAKE_GENERATION(generation,alloc_type) ((generation & ALLOC_GEN_FLAGS) | (alloc_type & ALLOC_TYPE_FLAGS))
#define GET_GENERATION(value)		 (value & ALLOC_GEN_FLAGS) 
#define GET_ALLOC_TYPE(value)		 (value & ALLOC_TYPE_FLAGS) 


inline const char * alloc_name(unsigned int val)
{
   switch(val) {
     case ALLOC_MALLOC: 
       return "malloc";
     case ALLOC_NEW:
       return "new";
     case ALLOC_NEW_ARRAY:
       return "new[]";
     default:
       return "<unknown>";
     }  
}

inline const char * free_name(unsigned int val)
{
   switch(val) {
     case ALLOC_MALLOC: 
       return "free";
     case ALLOC_NEW:
       return "delete";
     case ALLOC_NEW_ARRAY:
       return "delete[]";
     default:
       return "<unknown>";
     }  
}

#endif

#endif
