
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


typedef struct tagMDLIST {
  struct tagMDLIST *prev,*next;
} MDLIST;

typedef int (*MDLIST_VISIT_BLOCK) (MDLIST *entry);


// ouch, just one instance of this object.
static int block_count;

STATIC_C void MDLIST_init(MDLIST *root)
{
  root->prev = root->next = root;
}

STATIC_C void MDLIST_add(MDLIST *prev, MDLIST *nptr)
{ 
   MDLIST *next_link = prev->next;

   nptr->next = next_link;
   prev->next = nptr;
   
   nptr->prev = prev;
   next_link->prev = nptr;

   block_count += 1;
}

STATIC_C void MDLIST_remove(MDLIST *elm)
{
   MDLIST *t_next, *t_prev;

   t_next = elm->next;
   t_prev = elm->prev;

   t_prev->next = t_next;
   t_next->prev = t_prev;

   block_count -= 1;
}


STATIC_C int MDLIST_walk_block_list(MDLIST *root, MDLIST_VISIT_BLOCK visit)
{
  int size = 0;
  MDLIST *prev, *entry;
  int ret = 0;
  
  for( prev = root, entry = root->next ; entry != root; prev = prev->next, entry = entry->next ) {

     if (!entry) {
        dbgmem_log_err("ERROR: ALLOCATION CHAIN IS NULL - SOMEWHERE MEMORY IS OVERWRITTEN, VERY BAD");
        break;
     }
  
     if (entry->prev != prev || prev->next != entry) {
        dbgmem_log_err("ERROR: ALLOCATION CHAIN LINKS CORRUPTED, VERY BAD");
	break;
     } 
  
     ret += visit( entry );
     size+= 1;
  }

  if (block_count != size) {
    dbgmem_log_err("ERROR: BLOCK CHAIN CORRUPTED - WE HAVE %d but counted %d entries, VERY BAD", block_count, size);  
  }
  return ret;
}

STATIC_C int MDLIST_walk_nodes(MDLIST *root, MDLIST *start, int nodes, int direction, MDLIST_VISIT_BLOCK visit)
{
  int size = 0;
  MDLIST *prev, *entry;
  int ret = 0;
  
  for( prev= 0, entry = start ; size < nodes && entry != root; ) {

     if (!entry) {
        dbgmem_log_err("ERROR: ALLOCATION CHAIN IS NULL - SOMEWHERE MEMORY IS OVERWRITTEN, VERY BAD");
	ret ++;
        break;
     }
  
     if (prev) {
       if (direction) { 	
         if (entry->prev != prev || prev->next != entry) {
           dbgmem_log_err("ERROR: ALLOCATION CHAIN LINKS CORRUPTED, VERY BAD");
	   ret ++; 
           break;
         }
       } else {
         if (entry->next != prev || prev->prev != entry) {
           dbgmem_log_err("ERROR: ALLOCATION CHAIN LINKS CORRUPTED, VERY BAD");
	   ret ++;
	   break;
         }
       }
 
     } else {
       prev = start;
     } 
  
     ret += visit( entry );
     
     size+= 1;
     if (direction) {
       prev = prev->next; 
       entry = entry->next;
     } else { 
       prev = prev->prev; 
       entry = entry->prev;
     }   
  }
  
  if (nodes != size) {
    dbgmem_log_err("ERROR: BLOCK CHAIN CORRUPTED - WE HAVE %d but counted %d entries, VERY BAD", block_count, size);
    ret ++;
  }
  return ret;
}


