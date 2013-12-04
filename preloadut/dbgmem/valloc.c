
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


STATIC_C void * common_alloc_mem( size_t sz, size_t align, size_t arena_size, size_t tail_size)
{
   size_t to_alloc; 
   void *alloc;
   void *udata;
   void *offset;

   to_alloc = sz + arena_size + tail_size ;
   if (align) {
     to_alloc += sizeof(size_t) + align; 
   }

   alloc = get_malloc() (to_alloc);
   if (!align) {
     return alloc;
   }
   udata = BPTR_AT( alloc, arena_size + sizeof(size_t) );

   // now align ret to get user data pointer.
   udata = (void *) DO_ALIGN( udata, align );

   // now set offset before arena to point to difference
   offset = BPTR_AT_SUB(udata, arena_size + sizeof(size_t));

   * VVPTR_AT( offset, 0 ) = (void *) BPTR_DIFF( offset, alloc );

   // now return the value
   return BPTR_AT(offset, sizeof(size_t));
}

STATIC_C void common_free(void *ptr, int is_valign )
{
  size_t diff;
  void *free_ptr;

  if (!is_valign) {
    get_free()( ptr );
  } else {
    diff = (size_t) * VVPTR_AT_SUB( ptr,  sizeof(size_t) );
    free_ptr = BPTR_AT_SUB( ptr, sizeof(size_t) + diff );  
    get_free() ( free_ptr );
  }
}


