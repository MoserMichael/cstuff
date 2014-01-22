/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include "vtest.h"
#include "vtestcui.h"
#include <stdio.h>

void always_fails()
{ 
	VASSERT( 1 == 0 );
}

void always_pass()
{
}

void setUpFails()
{
	VASSERT(0);
}

/* define a test suite named FIRSTTEST*/
VTEST_DEFINE_SUITE( FIRSTTEST, 0, 0, SECONDTEST)
	VTEST_TEST( "alwayspass", always_pass)
	VTEST_TEST( "failtest", always_fails)
	VTEST_TEST( "anothertest", always_pass)
VTEST_END_SUITE

/* define a test suite named SECONDTEST*/
VTEST_DEFINE_SUITE( SECONDTEST, setUpFails, 0, LASTTEST)
	VTEST_TEST( "alwayspass", always_pass)
	VTEST_TEST( "failtest", always_fails)
	VTEST_TEST( "anothertest", always_pass)
VTEST_END_SUITE


/* define a test suite named LASTTEST; note that this is the last suite in the chain of suites*/
VTEST_DEFINE_LAST_SUITE( LASTTEST, 0, 0)
	/* repeat execution of the same test three times */
	VTEST_TEST_REPEATED( "failtest", always_fails, 3)
	VTEST_TEST( "alwayspass", always_pass)
VTEST_END_SUITE

int main(int argc, char *argv[])
{
  VTEST_CUI_ignore_test_failure(); // here: tell next function not to exit on failure
  VTEST_CUI_test_runner_cmdline( VTEST_SUITE_GET(FIRSTTEST), argc-1, argv+1 );
  printf("*** Please ignore the previous message ***\n");
  return 0; 
}
