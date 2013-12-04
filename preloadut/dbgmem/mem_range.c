
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


//---------------------------------------------------------------------------
// memory range lib 
//---------------------------------------------------------------------------

typedef struct tagMEM_ENTRY {
  DLIST entry;
  void *ptr;
  unsigned int generation;
  size_t udata_size;
  void  *frames[0];
} 
  MEM_ENTRY;

static FIXED_SIZE_ALLOC_HDR * g_mem_entry_alloc;
static int MENTRY_SIZE; 
static RADIX_TREE lookup_tree;

STATIC_C int MEM_ENTRY_init(int stack_frames)
{
  MENTRY_SIZE = sizeof(MEM_ENTRY) +  stack_frames * sizeof(void *);  

  // allocate additional header for each allocation 
  g_mem_entry_alloc =  FIXED_SIZE_ALLOC_init(10,MENTRY_SIZE);
  if (!g_mem_entry_alloc) {
    dbgmem_log_err("DBGMEM: Can't allocate from shared memory (tracker allocator)\n");
    return -1;
  }
  
  DLIST_init( & g_mem_entry_alloc );

  // allocate lookup treee - map pointer to arena.
  if (RADIX_TREE_init(&lookup_tree, 20 )) {
    dbgmem_log_err("DBGMEM: Can't allocate from shared memory (tree allocator)\n");
    return -1;
  }
  return 0;
}

STATIC_C void MEM_ENTRY_release()
{
   FIXED_SIZE_ALLOC_release( g_mem_entry_alloc ); 
   RADIX_TREE_release(&lookup_tree);
}

STATIC_C MEM_ENTRY *MEM_ENTRY_new_range( void *udata, size_t udata_size, size_t *ret_entry_idx, unsigned int allocation_type)
{
  size_t entry_idx;
  MEM_ENTRY *mentry;

#ifndef  __cplusplus
  (void) allocation_type;
#endif

  entry_idx = FIXED_SIZE_ALLOC_malloc( &g_mem_entry_alloc );
  if (entry_idx == FIXED_SIZE_EOF) {
    dbgmem_log_err("MEM_ENTRY_new_range: no new entry");
    return 0;
  }
  mentry =  (MEM_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( g_mem_entry_alloc, entry_idx  ); 
  if (!mentry) {
    dbgmem_log_err("MEM_ENTRY_new_range: no new entry (2)");
    return 0;
  }
  if (DLIST_add( g_mem_entry_alloc, entry_idx )) {
    dbgmem_log_err("MEM_ENTRY_new_range: can't add to list");
    return 0;
  }
  if (RADIX_TREE_add( &lookup_tree, udata , entry_idx)) {
    dbgmem_log_err("MEM_ENTRY_new_range: can't add to tree");

    DLIST_remove( g_mem_entry_alloc, entry_idx );
    FIXED_SIZE_ALLOC_free( g_mem_entry_alloc, entry_idx );
    
    return 0;
  }
  

  mentry->ptr = udata;
  mentry->udata_size = udata_size;
#ifndef  __cplusplus
  mentry->generation = alloc_generation;
#else 
  mentry->generation = MAKE_GENERATION( alloc_generation, allocation_type ); 
#endif
  *ret_entry_idx = entry_idx;

 

  return mentry;
}

STATIC_C int MEM_EMTRY_free_range( void *udata  )
{
  size_t entry_offset; 

  if ( (entry_offset = RADIX_TREE_remove( &lookup_tree, udata ))  == (size_t) -1) {
	dbgmem_log_err("MEM_EMTRY_free_range: failed to remove from tree [ptr %p offset %zd]\n", udata, entry_offset);
	return -1;
  }

  if (DLIST_remove( g_mem_entry_alloc, entry_offset) != 0) {
	dbgmem_log_err("MEM_EMTRY_free_range: failed to remove from list [ptr %p offset %zd]\n", udata, entry_offset);
	return -1;
  }
  FIXED_SIZE_ALLOC_free( g_mem_entry_alloc, entry_offset );

  return 0;
}

STATIC_C size_t MEM_ENTRY_match(const void *arg_key, size_t value, const void *udata_low)
{
   MEM_ENTRY *mentry;

   (void) arg_key;

   mentry =  (MEM_ENTRY *) FIXED_SIZE_GET_PTR_CHECKED( g_mem_entry_alloc, (size_t) value  ); 
   
   if (!udata_low) {
     return (size_t) mentry;
   }
   
   if ((size_t) BPTR_AT(mentry->ptr, mentry->udata_size) > (size_t) udata_low) {
     return (size_t) mentry;
   }
   return (size_t) -1;
}

STATIC_C MEM_ENTRY *MEM_ENTRY_find( const void *udata_low, const void *udata_high )
{
  size_t ret;
  
  ret = RADIX_TREE_find_match( &lookup_tree, udata_high, MEM_ENTRY_match, udata_low); 
  if (ret == (size_t) -1) {
    return 0;
  }
  return (MEM_ENTRY *) ret; 
}


