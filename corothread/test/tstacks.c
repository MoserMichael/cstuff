#include <corothread/stacks.h>
#include "vtest/vtest.h"


#define NUM_STACKS 1000
//#define NUM_STACKS 10000 

void STACKS_test()
{
  STACKS stacks;
  STACK_ENTRY *entry[ NUM_STACKS ];
  VASSERT( !  STACKS_init( &stacks, NUM_STACKS, 5 ) );
  int i;

  for(i = 0; i < NUM_STACKS; i++) {
     entry[ i ] = STACKS_get( &stacks );
     VASSERT( entry[ i ] != 0 );
#ifndef __CYGWIN__
     VASSERT(  entry[ i ]->stack_start != 0 ); 
#endif
  }
  
  for(i = 0; i < NUM_STACKS; i++) {
     VASSERT( ! STACKS_release( entry[i] ) );
  }
  VASSERT( ! STACKS_destroy( &stacks ) ); 
}


