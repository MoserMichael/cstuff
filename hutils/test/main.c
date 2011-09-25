/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <vtest/vtest.h>
#include <vtest/vtestcui.h>
#include <string.h>
#include <stdlib.h>

void URI_test_ok();
void URI_test_fail();
void HTTP_request_ok();
void HTTP_request_fail();
void QUERY_test();
void HTTP_writer_ok();



VTEST_DEFINE_SUITE( BASETEST, 0, 0, LASTTEST)
        VTEST_TESTN( HTTP_writer_ok )
	VTEST_TESTN( QUERY_test )
	VTEST_TESTN( URI_test_ok )
	VTEST_TESTN( URI_test_fail )
	VTEST_TESTN( HTTP_request_ok )
	VTEST_TESTN( HTTP_request_fail )
VTEST_END_SUITE


/* define a test suite named LASTTEST; note that this is the last suite in the chain of suites*/
VTEST_DEFINE_LAST_SUITE( LASTTEST, 0, 0)
VTEST_END_SUITE


int main(int argc, char *argv[])
{

  VTEST_CUI_test_runner_cmdline( VTEST_SUITE_GET(BASETEST), argc-1, argv+1 );
  
  return 0;
}

