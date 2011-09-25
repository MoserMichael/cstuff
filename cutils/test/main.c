/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <vtest/vtest.h>
#include <vtest/vtestcui.h>
#include <string.h>
#include <stdlib.h>

void DLIST_test();
void DRING_test();
void SLIST_test();
void SRING_test();
void HASH_test();
void HEAP_test();
void ARRAY_test();
void TREE_test();
void DLISTUNR_test_insert();
void DLISTUNR_test();



VTEST_DEFINE_SUITE( BASETEST, 0, 0, LASTTEST)
	VTEST_TESTN( DLIST_test )
	VTEST_TESTN( DLISTUNR_test )
	VTEST_TESTN( DLISTUNR_test_insert )
	VTEST_TESTN( DRING_test )
	VTEST_TESTN( SLIST_test )
	VTEST_TESTN( SRING_test )
	VTEST_TESTN( HASH_test )
	VTEST_TESTN( HEAP_test )
	VTEST_TESTN( ARRAY_test )
	VTEST_TESTN( TREE_test )
VTEST_END_SUITE


/* define a test suite named LASTTEST; note that this is the last suite in the chain of suites*/
VTEST_DEFINE_LAST_SUITE( LASTTEST, 0, 0)
VTEST_END_SUITE


int main(int argc, char *argv[])
{

  VTEST_CUI_test_runner_cmdline( VTEST_SUITE_GET(BASETEST), argc-1, argv+1 );
  
  return 0;
}

