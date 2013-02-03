#include <sys/mman.h>
#include "stacks.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "internal.h"
#include <unistd.h>
#include <butils/logg.h>

#if defined(__CYGWIN__) || defined(_WIN32)
#include <windows.h>
#endif

// address must be aligned to boundary.
#define ALIGN_UP(ptr) (uint8_t *) ((((size_t) (ptr)) & ~ (size_t) 7) + 8)
#define ALIGN_DOWN(ptr) (uint8_t *) (((size_t) (ptr)) & ~ (size_t) 7)



int   STACKS_init_on_demand( STACKS *stack, int num_stacks, int pages_per_stack )
{
  size_t page_size, one_stack_size;

  page_size = get_page_size();
  one_stack_size = page_size *  pages_per_stack; 
  
  stack->num_stacks = num_stacks;
  stack->one_stack_size  = one_stack_size;
  stack->stack_mode = STACK_MODE_ALLOC_ON_DEMAND;  
 
  DLIST_init( &stack->root );

  return 0;
}

STACK_ENTRY *make_one_stack( STACKS *stack, uint8_t * offset )
{
  STACK_ENTRY *entry;
  size_t page_size;

#ifndef HAS_WIN32_FIBER
  uint8_t *guard_page = 0;
  size_t stack_len;
#else
  (void) offset;
#endif
  
  page_size = get_page_size();
  
  entry = (STACK_ENTRY *) malloc( sizeof( STACK_ENTRY ) );
  if (!entry) {
     return 0;
  }
  memset( entry, 0, sizeof( STACK_ENTRY ) );
  
  entry->stacks = stack;

#ifndef HAS_WIN32_FIBER
  if (!offset) {
    stack_len = stack->one_stack_size + page_size; 
    offset = (uint8_t *) mmap( 0 , stack_len, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
    if (!offset || offset == (uint8_t *) -1) {
       MLOG_ERROR("STACKS_init - can' mmap range of %ld bytes", stack_len );
       return 0;
    }  
    entry->stack_mem = (void *) offset;
    entry->stack_mem_size = stack_len;
    
    MLOG_TRACE( "STACKS_init - mmap range from %p to %p", offset, offset + stack_len );
  }
 
  // base of address range is aligned to page size, isn't it?
  assert( (((size_t) offset) & (size_t) (page_size - 1)) == 0);
 
  switch( get_stack_direction() ) {
      case STACK_DIR_GROWING_DOWN:
	entry->stack_start = offset + page_size; 
        entry->stack_size =  stack->one_stack_size;
	guard_page = offset;
	break;
      case STACK_DIR_GROWING_UP:
	entry->stack_start = offset; 
        entry->stack_start = entry->stack_start; 
	guard_page = offset + stack->one_stack_size;  
        entry->stack_size = (guard_page - entry->stack_start);
	break;
  }

  // check aligned to page size.
  assert( (((size_t) guard_page) & (size_t) (page_size - 1)) == 0);

#if defined(__CYGWIN__) || defined(_WIN32)
  VirtualProtect( guard_page, 1, PAGE_NOACCESS, 0 );
#else  
  if (mprotect( guard_page , 1, PROT_NONE )) {
       free(entry);
       return 0;
  }
#endif  
#endif

  return entry;
}                              

int STACKS_init( STACKS *stack, int num_stacks, int pages_per_stack )
{
  int page_size;
  size_t len, one_stack_size;
  STACK_ENTRY *entry;
  uint8_t *mapping;
  uint8_t *off;
  int i;

  if (num_stacks < 1 || pages_per_stack < 1) {
    MLOG_ERROR( "STACKS_init - invalid parameter" );
    return -1;
  }
  
  page_size = get_page_size();
  one_stack_size = page_size * pages_per_stack; 
  len = num_stacks * (one_stack_size  + page_size ); // each stack includes one guard page.

  mapping = (uint8_t *) mmap( 0 , len, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS 

#if 0
#ifdef MAP_STACK
			| MAP_STACK
#endif
#endif
		      , -1, 0 );
  if (!mapping || mapping == (uint8_t *) -1) {
     MLOG_ERROR("STACKS_init - can' mmap range of %ld bytes", len );
     return -1;
  }

  memset( mapping , 0, len );
  
  // base of address range is aligned to page size, isn't it?
  assert( (((size_t) mapping) & (size_t) (page_size - 1)) == 0);
  
  MLOG_TRACE( "STACKS_init - mmap big range from %p to %p", mapping, mapping + len );


  stack->mapping = mapping;
  stack->mapping_length = len;
  stack->num_stacks = num_stacks;
  stack->one_stack_size  = one_stack_size;
  stack->stack_mode = STACK_MODE_PREALLOC;  
  
  DLIST_init( &stack->root );
  
  for( off = mapping, i = 0; i < num_stacks; i++, off += (one_stack_size + page_size) ) {
      entry = make_one_stack( stack, off );
      if (!entry) {
        return -1;
      }	
      DLIST_push_back( &stack->root, &entry->entry );
    //DLIST_push_front( &stack->root, &entry->entry );
  }

  return 0;
}


STACK_ENTRY *STACKS_get(STACKS *stack)
{
  STACK_ENTRY *entry;
 
  if (DLIST_isempty( &stack->root ) ) {
    if ( stack->stack_mode == STACK_MODE_ALLOC_ON_DEMAND) {
      if (stack->num_stacks == (size_t) -1 || DLIST_size( &stack->root ) < stack->num_stacks) {
	 entry = make_one_stack( stack, 0 );
	 if (! entry ) {
	   return 0;
	 }
 	 return entry;
      }
    }
    return 0;
  }
  entry = (STACK_ENTRY *) DLIST_pop_front( &stack->root );
  return entry;
}

int STACKS_release(STACK_ENTRY *entry)
{
  STACKS *stack = entry->stacks;
  DLIST_push_front( &stack->root, &entry->entry );
  return 0;
}

int STACKS_destroy( STACKS *stack )
{
  DLIST_entry *r1, *r2;

  if ( stack->num_stacks != (size_t) -1 && DLIST_size( &stack->root) != (size_t) stack->num_stacks ) {
    MLOG_ERROR("STACKS_destroy: some %d stack are in use (out of %d)", stack->num_stacks, DLIST_size( &stack->root) );
    return -1;
  }
  
#ifndef HAS_WIN32_FIBER
  if (stack->stack_mode == STACK_MODE_PREALLOC) {  
    if (munmap( stack->mapping, stack->mapping_length)) {
       MLOG_ERROR("STACKS_destroy: Can't unmap %p",stack->mapping );
       return -1;
    }
    MLOG_TRACE( "STACKS_destroy - unmap %p to %p", stack->mapping, stack->mapping + stack->mapping_length );
  }
#endif

  DLIST_FOREACH_SAVE( r1, r2, &stack->root ) {
    STACK_ENTRY *entry;
    
    entry = (STACK_ENTRY *) r1;
#ifndef HAS_WIN32_FIBER
    if (stack->stack_mode == STACK_MODE_ALLOC_ON_DEMAND) {
      if (munmap( entry->stack_mem, entry->stack_mem_size )) {
        MLOG_ERROR("STACKS_destroy: Can't unmap %p", entry->stack_mem );
        return -1; 
      }              	
      MLOG_TRACE( "STACKS_destroy - unmap %p to %p", entry->stack_mem, entry->stack_mem + entry->stack_mem_size );
    }  
#endif 
    free(entry);    
  } 

  
  return 0;
}

