#ifndef __STACK_CACHE_H_
#define __STACK_CACHE_H_

#include <stdint.h>
#include <cutils/dlist.h>

typedef enum {
   STACK_DIR_GROWING_DOWN,
   STACK_DIR_GROWING_UP,
}  STACK_DIR;

/** 
 * @defgroup STACKS
 * @brief a pool of stacks for a threading package. 
 * 
 * @{
 */
typedef struct tagSTACKS {
  uint8_t *mapping;
  size_t mapping_length;
  int num_stacks;
  size_t one_stack_size;
  DLIST root;
} STACKS;

typedef struct tagSTACK_ENTRY {
  DLIST_entry entry;
  void *stack_start;
  STACKS *stacks;
} STACK_ENTRY;

int   STACKS_init( STACKS *stack, int num_stacks, int pages_per_stack );
int   STACKS_destroy( STACKS *stack );
void *STACKS_get(STACKS *stack, STACK_ENTRY **rentry );
int   STACKS_release(STACK_ENTRY *entry);


M_INLINE size_t STACKS_get_stack_size(STACKS *stack) {
  return stack->one_stack_size;
}

/*
 * @}
 */

#endif


