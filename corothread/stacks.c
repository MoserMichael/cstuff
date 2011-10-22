#include <sys/mman.h>
#include <unistd.h>
#include "stacks.h"
#include <stdio.h>

STACK_DIR stack_dir( int *ptr )
{
   int a = 0;
   if ((size_t) &a < (size_t) ptr ) {
     return STACK_DIR_GROWING_DOWN; 
   }
   return STACK_DIR_GROWING_UP;
}

int stack_direction()
{
  int a = 0;

  return stack_dir( & a );
}

int STACKS_init( STACKS *stack, int num_stacks, int pages_per_stack )
{
  int page_size;
  size_t len, one_stack_size;
  STACK_DIR dir;
  STACK_ENTRY *entry;
  uint8_t *mapping, *stack_start;
  uint8_t *guard_page;
  int i;
  
  page_size = sysconf( _SC_PAGE_SIZE );
  one_stack_size = page_size *  pages_per_stack; 
  len = num_stacks * one_stack_size;
  dir = stack_direction();

  mapping = (uint8_t *) mmap( 0 , len, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS 

#if 0
#ifdef MAP_STACK
			| MAP_STACK
#endif
#endif
		      , -1, 0 );
  if (!mapping) {
    return -1;
  }

  //fprintf( stderr, "all stacks %p - %p\n", mapping, mapping + len );
  
  stack->mapping = mapping;
  stack->mapping_length = len;
  stack->num_stacks = num_stacks;
  stack->one_stack_size = one_stack_size - page_size;
 
  DLIST_init( &stack->root );

  for( i = 0; i < num_stacks; i ++ ) {
    stack_start = mapping + ( i * one_stack_size);
    
    if (dir == STACK_DIR_GROWING_DOWN) {
    //stack_start = mapping + ( (i + 1) * one_stack_size) - 1;
      guard_page =  mapping + (i * one_stack_size);
    } else {
      guard_page = mapping + ( i + 1 ) * one_stack_size - page_size;
    }
  
    if (mprotect( guard_page , page_size, PROT_NONE )) {
      return -1;
    }
    
    entry = (STACK_ENTRY *) malloc( sizeof( STACK_ENTRY ) );
    if (!entry) {
      return -1;
    }
    entry->stack_start = stack_start;
    entry->stacks = stack;
    DLIST_push_back( &stack->root, &entry->entry );
  }
  return 0;
}

int STACKS_destroy( STACKS *stack )
{
  if ( DLIST_size( &stack->root) != (size_t) stack->num_stacks ) {
    return -1;
  }
  
  if (munmap( stack->mapping, stack->mapping_length)) {
    return -1;
  }
  return 0;
}


void *STACKS_get(STACKS *stack, STACK_ENTRY **rentry )
{
  STACK_ENTRY *entry;
  if (DLIST_isempty( &stack->root ) ) {
    return 0;
  }
  entry = (STACK_ENTRY *) DLIST_pop_front( &stack->root );
  *rentry = entry;
  return entry->stack_start;
}

int STACKS_release(STACK_ENTRY *entry)
{
  STACKS *stack = entry->stacks;
  DLIST_push_front( &stack->root, &entry->entry );
  return 0;
}
