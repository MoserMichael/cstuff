
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

#ifndef _WIN32 

#include <sys/mman.h>
#include <linux/mman.h>
#include <errno.h>
#include <sys/user.h>
#include <dlfcn.h>
#endif

#define ROUND_UP(x,bits) ( ((x) & ~( (1 << (bits)) -1 ))  + (1 << (bits) ) )


MAKE_FUNC( mmap );
MAKE_FUNC( mremap );
MAKE_FUNC( munmap );

//---------------------------------------------------------------------------------
// fixed size allocator - allocate from shared memory; 
// allocator returns index of memory block since shared memory big block can move.
//---------------------------------------------------------------------------------

typedef struct tagFIXED_SIZE_ALLOC_HDR {
  size_t unit_size;
  size_t area_size;
  size_t alloc_area_index;
  size_t first_free;
}
  FIXED_SIZE_ALLOC_HDR;

#define FIXED_SIZE_EOF ((size_t) -1)  

#define FIXED_SIZE_GET_PTR(hdr, offset) BPTR_AT( (FIXED_SIZE_ALLOC_HDR *) (hdr) + 1 , (offset) )

#define FIXED_SIZE_OFFSET_OK(hdr, offset) \
    (((size_t) (offset)  + (hdr)->unit_size) <= (size_t) ((hdr)->area_size)   && \
     ((size_t) (offset) %  (hdr)->unit_size) == 0)

#define FIXED_SIZE_GET_PTR_CHECKED(hdr, offset) (FIXED_SIZE_OFFSET_OK(hdr,offset) ? FIXED_SIZE_GET_PTR(hdr,offset) : 0)


#define ROUND_UP(x,bits) ( ((x) & ~( (1 << (bits)) -1 ))  + (1 << (bits) ) )


#ifndef FIXED_ALLOC_USE_MALLOC

static unsigned int DBGMEM_PAGE_SHIFT;

static void init_page_size()
{
  size_t n = 1, val = 1;

  if (!DBGMEM_PAGE_SHIFT) {
    DBGMEM_PAGE_SHIFT = getpagesize();
    while( val < DBGMEM_PAGE_SHIFT) {
      n+=1;
      val<<=1;
    }
    DBGMEM_PAGE_SHIFT = n;
  }
} 
#endif

STATIC_C FIXED_SIZE_ALLOC_HDR * FIXED_SIZE_ALLOC_init(size_t initial_size,size_t unit_size)
{
  FIXED_SIZE_ALLOC_HDR *hdr;
  size_t sz;
  size_t area_size;
  
  unit_size = unit_size < sizeof(size_t) ? sizeof(size_t) : unit_size;
  area_size = initial_size * unit_size;
  sz = area_size + sizeof( FIXED_SIZE_ALLOC_HDR );


#ifdef FIXED_ALLOC_USE_MALLOC
  hdr = (FIXED_SIZE_ALLOC_HDR *) malloc( sz );
  if (hdr == 0) {
#else
  init_page_size();
 
  hdr = (FIXED_SIZE_ALLOC_HDR *) get_mmap() ( 0, ROUND_UP( sz, DBGMEM_PAGE_SHIFT), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
  if (hdr == MAP_FAILED) {
#endif
    dbgmem_log("Failed to allocated shared memory errno=%d \n", errno);
    return 0;
  }

  hdr->unit_size = unit_size;
  hdr->area_size = area_size;
  hdr->first_free = FIXED_SIZE_EOF;
  hdr->alloc_area_index = 0; 
  
  return hdr;
} 

STATIC_C void FIXED_SIZE_ALLOC_release( FIXED_SIZE_ALLOC_HDR *hdr)
{
#ifdef FIXED_ALLOC_USE_MALLOC
  free( hdr );
#else
  get_munmap() ( (void *) hdr, hdr->area_size );
#endif
}

STATIC_C size_t FIXED_SIZE_ALLOC_malloc(FIXED_SIZE_ALLOC_HDR **hdr)
{
  size_t ret = FIXED_SIZE_EOF; 
  FIXED_SIZE_ALLOC_HDR *hdr_ptr = *hdr; 

again:  
  
  if (hdr_ptr->first_free != FIXED_SIZE_EOF) {
    size_t *ptr;
    
    ret = hdr_ptr->first_free; 
    ptr =  (size_t *) FIXED_SIZE_GET_PTR(hdr_ptr, ret); 
    hdr_ptr->first_free = * ptr;

  } else if (hdr_ptr->alloc_area_index < hdr_ptr->area_size) {
     
    ret = hdr_ptr->alloc_area_index;
    hdr_ptr->alloc_area_index += hdr_ptr->unit_size;
      
  } else {
    
    size_t new_size;
    size_t alloc_size;    
    size_t usize;

    new_size = hdr_ptr->area_size << 1;	
    alloc_size = sizeof( FIXED_SIZE_ALLOC_HDR ) + new_size;
    usize = hdr_ptr->unit_size;

    // grow memory map and do it again.
#ifdef FIXED_ALLOC_USE_MALLOC
    hdr_ptr = (FIXED_SIZE_ALLOC_HDR *) realloc( hdr_ptr, alloc_size );
    if (hdr_ptr) 
#else
    hdr_ptr = (FIXED_SIZE_ALLOC_HDR *) get_mremap() ( hdr_ptr, 
		ROUND_UP( hdr_ptr->area_size, DBGMEM_PAGE_SHIFT), 
		ROUND_UP( alloc_size, DBGMEM_PAGE_SHIFT), 
		MREMAP_MAYMOVE);
    if (hdr_ptr != MAP_FAILED) 
#endif
    {
       *hdr = hdr_ptr;
      (*hdr)->area_size = new_size;
       goto again;
    } else {
       dbgmem_log_err( "FIXED_SIZE_ALLOC failed to allocate big block (requested: %zd unit_size: %zd)\n", new_size, usize );
       exit(6);
    }
  }

  //fprintf(stderr, " ## malloc(%p) %d - next %d \n", hdr_ptr, ret, hdr_ptr->first_free);	
 
  return ret;
}

STATIC_C int FIXED_SIZE_ALLOC_free(FIXED_SIZE_ALLOC_HDR *hdr, size_t idx)
{
  size_t *at;

  if ( ! FIXED_SIZE_OFFSET_OK(hdr,idx) ) {
    dbgmem_log_err("FIXED_SIZE_ALLOC_free - wrong offset at %zd\n", idx);
    return -1;
  }  
  
  at = (size_t *) FIXED_SIZE_GET_PTR(hdr, idx);
  at[0] = hdr->first_free;
  
  hdr->first_free = idx;
	
  //fprintf(stderr, " ## first_free(%p) %d next %d\n", hdr, hdr->first_free, *at);	
  return 0;
}


