
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
// linked list based on indexes. 
//---------------------------------------------------------------------------

typedef struct tagDLIST {
  size_t prev,next;
} DLIST;

typedef int (*VISIT_BLOCK) (DLIST *entry);


STATIC_C int DLIST_init(FIXED_SIZE_ALLOC_HDR **hdr)
{ 
  size_t root;
  DLIST *entry;
 
  // assumes we are the first allocation from this allocator.
  root = FIXED_SIZE_ALLOC_malloc(hdr);
  if (root) {
    return -1;
  }

  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( *hdr, 0 );
  if (!entry) {
    return -1;
  }
  entry->next = entry->prev = 0;

  return 0;
} 

STATIC_C int DLIST_add(FIXED_SIZE_ALLOC_HDR *hdr, size_t pos)
{
  DLIST *root, *entry, *next; 

  root = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, 0 );
  if (!root) {
    return -1;
  }
 
  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, pos );
  
  next = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, root->next );
  if (!next) {
    return -1;
  }

  entry->next = root->next; 
  root->next = pos;

  next->prev = pos;
  entry->prev = 0;

  return 0;
}

STATIC_C int DLIST_remove(FIXED_SIZE_ALLOC_HDR *hdr, size_t pos)
{
  DLIST *entry, *next, *prev;

  // can't remove the root entry.
  if (pos == 0) {
    return -1;
  }  
  
  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, pos );
  if (!entry) {
    return -1;
  }

  next =  (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, entry->next );
  if (!next) {
    return -1;
  }

  prev =  (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( hdr, entry->prev );
  if (!prev) {
    return -1;
  }  
 
  next->prev = entry->prev;
  prev->next = entry->next;

  return 0;
}


STATIC_C int DLIST_walk_block_list(FIXED_SIZE_ALLOC_HDR *alloc, VISIT_BLOCK visit)
{
  DLIST *entry;
  size_t cur_idx,prev_idx = 0;
  int ret = 0;
 
  entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, 0 );
  if (entry->next != 0) {
   
    cur_idx = entry->next;
    entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, cur_idx ); //entry->next );

    while(1) {
      if (!entry) {
	dbgmem_log_err("ERROR: LINKED LIST CORRUPTED - WRONG LINK VALUE, VERY BAD");
        return -1;
      }

      if (entry->prev != prev_idx) {
	dbgmem_log_err("ERROR: LINKED LIST CORRUPTED, VERY BAD");
	return -1;
      }
	
      ret += visit(entry);

      if (entry->next == 0) {
        entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, 0 );
        if (!entry || entry->prev != cur_idx) {
	  dbgmem_log_err("ERROR: LINKED LIST HAS LOOPS, VERY BAD");
        }
        return -1;
      }
      
      prev_idx = cur_idx;
      cur_idx = entry->next;
      entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, cur_idx );  //entry->next
    }
  }
  return ret;
}

STATIC_C int DLIST_walk_nodes(FIXED_SIZE_ALLOC_HDR *alloc, DLIST *start, int nodes, int direction, VISIT_BLOCK visit)
{
  DLIST *tmp, *entry, *prev = 0;
  size_t cur_idx = (size_t) -1;
  int ret = 0;
 
  for( entry = start; nodes > 0; nodes -- ) {	
      if (!entry) {
	dbgmem_log_err("ERROR: LINKED LIST CORRUPTED - WRONG LINK VALUE, VERY BAD");
        ret ++;
	return -1;
      }

      if (prev) {
        if (direction) {
	  tmp = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, entry->prev ); 	
          if ( tmp != prev) {
		dbgmem_log_err("ERROR: LINKED LIST CORRUPTED, VERY BAD");
		ret ++;
		return -1;
          }
        } else {
          tmp = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, entry->next ); 
          if ( tmp != prev) {
		dbgmem_log_err("ERROR: LINKED LIST CORRUPTED, VERY BAD");
		ret ++;
		return -1;
          }
        }
      }
	
      ret += visit(entry);
      
      if (direction) {
        cur_idx = entry->next;
      } else {
        cur_idx = entry->prev;
      }
      if (!cur_idx) {
        break;
      }
      prev = entry;
      entry = (DLIST *) FIXED_SIZE_GET_PTR_CHECKED( alloc, cur_idx );  //entry->next
  }
  return 0;
}
