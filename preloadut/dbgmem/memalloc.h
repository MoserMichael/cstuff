
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
