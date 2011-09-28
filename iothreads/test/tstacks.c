#include <iothreads/stacks.h>
#include "vtest/vtest.h"


#define NUM_STACKS 10000 

void STACKS_test()
{
  STACKS stacks;
  STACK_ENTRY *entry[ NUM_STACKS ];
  VASSERT( !  STACKS_init( &stacks, NUM_STACKS, 5 ) );
  int i;

  for(i = 0; i < NUM_STACKS; i++) {

     VASSERT( STACKS_get( &stacks, &entry[ i ] ) );
  }
  
  for(i = 0; i < NUM_STACKS; i++) {
     VASSERT( ! STACKS_release( entry[i] ) );
  }
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}


