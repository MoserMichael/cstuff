
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

#include "memalloc.h"

#include "valloc.c"

//---------------------------------------------------------------------------
// memory allocator.	 
//---------------------------------------------------------------------------
STATIC_C void * alloc_mem(size_t sz, size_t align, void *** frames, unsigned int allocation_type)
{
  ARENA_ENTRY *mem;

  mem = (ARENA_ENTRY *) common_alloc_mem( sz, align, sizeof(ARENA_ENTRY) , sizeof(void*) );

  if (mem) {
    MEM_ENTRY *mentry;
    size_t entry_idx;
		
    MT_LOCK( &memlock );
    mentry = MEM_ENTRY_new_range( mem+1, sz, &entry_idx, allocation_type );
    if (!mentry) {
      //ERROR_CHECKER_OUT_OF_MEMORY; 
      
      MT_UNLOCK( &memlock );
      return 0;
    }
   //mem->mem_entry_offset = entry_idx;

    SET_USER_DATA_LOW_GUARD( mentry, !align ? SENTINEL :  SENTINEL_VALIGN );
    SET_USER_DATA_HIGH_GUARD( mentry, SENTINEL );
    MT_UNLOCK( &memlock );


    *frames = (void **) &mentry->frames;

    return mem + 1;
  
  } else {
    return 0;
  } 
}

STATIC_C int free_mem(void *	udata , unsigned int allocation_type ) 
{
   int ret;
   ARENA_ENTRY *entry;
   MEM_ENTRY *mentry;
   size_t sz;

#ifndef __cplusplus
  (void) allocation_type;
#endif
   
   MT_LOCK( &memlock );
   if ((ret = MEM_ENTRY_check_exact_ptr( udata, &mentry, "free_mem" )) != 0) {
     MT_UNLOCK( &memlock );
     return ret;
   }
#ifdef  __cplusplus
   if (mentry && cpp_warnings && allocation_type != GET_ALLOC_TYPE(mentry->generation)) {
       
     dbgmem_log_err( "ERROR Memory allocated with %s but freed with %s. Entry: %p size: %zd generation: %d\n",
	       alloc_name( GET_ALLOC_TYPE(mentry->generation) ),
	       free_name( allocation_type ),
	       mentry->ptr,
	       mentry->udata_size,
	       GET_GENERATION(mentry->generation)
     );
     dbgmem_log_err( "status OK\n" );
     DBG_ERRROR_REPORT(mentry,2);
   }
#endif

   sz = mentry->udata_size;
   mentry->ptr = 0;
   mentry->udata_size = 0;


   ret = MEM_EMTRY_free_range( udata );
   MT_UNLOCK( &memlock );

   // freed memory is initialised with some value, so that access to free memory will cause some error
   if (fill_byte && udata) {
     memset(udata,FILL_BYTE_FREED_MEMORY,sz);
   } 
#if 0   
   if (ret) {
      return ret;
   }
#endif
   
   entry =  ((ARENA_ENTRY *) udata) - 1;  
   common_free( entry, entry->guard_ptr == SENTINEL_VALIGN );	 
   return 0;
}

STATIC_C size_t mem_size(void *udata)
{
   MEM_ENTRY *mentry;
   int ret;

   MT_LOCK( &memlock );
   ret = MEM_ENTRY_check_exact_ptr( udata, &mentry, "realloc" );  
   MT_UNLOCK( &memlock );

   if (ret) {
     return (size_t) -1;
   }
   return mentry->udata_size;
}

STATIC_C int do_check_memory()
{
  int num_errors = 0;
 
  MT_LOCK( &memlock );
  num_errors = DLIST_walk_block_list(g_mem_entry_alloc, check_block);
  MT_UNLOCK( &memlock );
  return num_errors;
}


STATIC_C void do_print_leaks()
{
   dbgmem_log("MEMALLOC: AREA_ALLOC\n");

  MT_LOCK( &memlock );
  DLIST_walk_block_list(g_mem_entry_alloc, print_block); 
  MT_UNLOCK( &memlock );
  
   dbgmem_log( "MEMALLOC: EOF\n");
}

//#include "mem_check.c"


             
STATIC_C int get_arena_size()
{
  return sizeof(ARENA_ENTRY) + sizeof(void *);
}

 
