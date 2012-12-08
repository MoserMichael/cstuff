#ifndef __STACK_CACHE_H_
#define __STACK_CACHE_H_

#include <stdint.h>
#include <cutils/dlist.h>

typedef enum {
  STACK_MODE_ALLOC_ON_DEMAND, 
  STACK_MODE_PREALLOC,
}
  STACK_MODE;
/** 
 * @defgroup STACKS
 * @brief a pool of stacks for a threading package. 
 * 
 * @{
 */
typedef struct tagSTACKS {
  uint8_t *mapping;		// if stack_mode ==STACK_MODE_PREALLOC - base of range for all stacks. 
  size_t mapping_length;	// if stack_mode ==STACK_MODE_PREALLOC - length of range for all stacks.  
  size_t num_stacks;		// maximum of stacks that can be allocated (-1 if no limit set).
  size_t one_stack_size;	// size of one stack (without guard page).
  STACK_MODE stack_mode;  	// either preallocate memory for all stacks PREALLOC - aor allocate each stack on demand ALLOC_ON_DEMAND.
  DLIST root;			
} STACKS;

typedef struct tagSTACK_ENTRY {
  DLIST_entry entry;		// links all stacks.	
  void *stack_mem;		// if allocated - base of allocated range for stack.
  size_t stack_mem_size;	// if allocated - size of range.

  uint8_t *stack_start;		// top of user stack (depends on direction) 
  size_t stack_size;		// size of user stack.

  STACKS *stacks;		// pointer back to factory.
} STACK_ENTRY;

/**
 * all stacks are all allocated here, out of one virtual memory block.
 */
int   STACKS_init( STACKS *stack, int bytes_per_stack, int pages_per_stack );

/**
 * Each stack is allocated when it is needed; 
 */
int   STACKS_init_on_demand( STACKS *stack, int num_stacks, int pages_per_stack );

int   STACKS_destroy( STACKS *stack );
STACK_ENTRY *STACKS_get(STACKS *stack  );
int   STACKS_release(STACK_ENTRY *entry);


M_INLINE size_t STACKS_get_stack_size(STACKS *stack) {
  return stack->one_stack_size;
}

/*
 * @}
 */

#endif


