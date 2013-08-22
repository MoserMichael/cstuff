/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include "vtest.h"
#include "vtestrunner.h"
#include "vtestcui.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>



static PFN_DEBUG_FUNCTION  debug_output_hook = 0;


static void MSG( const char * format, ... )
{
  va_list ap;
  
  va_start(ap, format);
  if (!debug_output_hook) {
    vfprintf( stdout, format, ap );   
  } else {
    char big_buf[ 32 * 1024 ];
    int n;

    n = vsnprintf( big_buf, sizeof( big_buf ) - 1, format, ap );
    big_buf[ n ] = '\0';
    debug_output_hook( big_buf );
    fputs( big_buf, stdout );
  }
  va_end(ap);

}

void VTEST_CUI_set_debug_output_hook( PFN_DEBUG_FUNCTION debug_output_hook_ ) {
	debug_output_hook = debug_output_hook_;
}

void CUI_report_suite_start (const char *suite_name)
{
	MSG( "Suite %s\n", suite_name );
}

void CUI_report_test_start (const char *suite_name, const char *test_name, int iteration, int maxiteration)
{
	if (maxiteration == 1) {
		MSG( "  Test: %s/%s started\n", suite_name, test_name);
	} else {
		MSG( "  Test: %s/%s started. iteration #%d out of #%d\n", suite_name, test_name, iteration, maxiteration);
	}
}

#ifdef TIME_TEST_GETTIMEOFDAY

static void format_time( char *time_str, size_t time_str_len, struct timeval *duration)
{
   if (duration->tv_sec != 0) {
     snprintf(time_str, time_str_len," (Duration - %ld sec %ld micro secs)\n", duration->tv_sec, duration->tv_usec);
   } else {
     snprintf(time_str, time_str_len," (Duration - %ld micro secs)\n", duration->tv_usec);
   }

}
#endif


void CUI_report_results(	VTEST_STATUS status, 
						const char *suite_name, const char *test_name,
						struct timeval *duration,
						const char *fail_cond, const char *fail_file,int fail_line)
{
	(void) (test_name);

	switch(status) {
		case VTEST_TEST_OK: {
			#ifdef TIME_TEST_GETTIMEOFDAY
			  char msg[100];
			  char *ptr;
                          size_t size;

                          ptr = msg;
                          size = sizeof(msg);

                          strcpy(msg, "    Status: Ok" );
			
                          size -= strlen(msg);
                          ptr  += strlen(msg);

                          format_time( ptr, size, duration );

			  MSG( msg );
                          
			#else
                          (void) (duration);
			  MSG( "    Status: Ok\n");
                        #endif
                        }
			break;
		case VTEST_TEST_FAILED:
			MSG( "    Status: Failed - %s at %s:%d\n", 
							fail_cond, fail_file, fail_line);
			break;
		case VTEST_SUITE_SETUP_FAILED:
			MSG( "Suite %s setup failed - %s at %s:%d\n",
							suite_name, fail_cond, fail_file, fail_line);
			break;

		case VTEST_SUITE_TEARDOWN_FAILED:
			MSG( "Suite %s teardown failed - %s at %s:%d\n",
							suite_name, fail_cond, fail_file, fail_line);
			break;

		default: {
				 }
	}
}

void CUI_report_wrapup	   (int suitesinitfailed, int suitesteardownfailed, 
							int tests_passed, int tests_failed, int testnotrun)
{
	MSG( "\nTest summary\n"
				   "  Tests passed:			%d\n"
				   "  Tests failed:			%d\n"
				   "  Tests not run:		%d (due to suite setup failure)\n"
   				   "  Suites setup failed:		%d\n"
				   "  Suites teardown failed:	%d\n",

					tests_passed, tests_failed, testnotrun,
					suitesinitfailed, suitesteardownfailed);

	if (!suitesinitfailed && !suitesteardownfailed && 
		!tests_failed  && !testnotrun) {

		MSG( "\nALL TESTS PASSED\n");

	} else {

		MSG( "\n***YOU HAVE TESTS TO FIX***\n");

	}

}

int VTEST_CUI_test_runner(VTEST_TEST_SUITE *suite)
{
  VTEST_RUNNER_IMPL impl;

  impl.suite_start			= CUI_report_suite_start;
  impl.test_start			= CUI_report_test_start;
  impl.results				= CUI_report_results;
  impl.wrapup				= CUI_report_wrapup;

  return VTEST_test_runner( suite, &impl );

}

int VTEST_CUI_test_runner_cmdline(VTEST_TEST_SUITE *suite, int argc, char *argv[])
{
  VTEST_RUNNER_IMPL impl;

  impl.suite_start			= CUI_report_suite_start;
  impl.test_start			= CUI_report_test_start;
  impl.results				= CUI_report_results;
  impl.wrapup				= CUI_report_wrapup;

  return VTEST_test_runner_cmdline( suite, &impl, argc, argv);

}

void VTEST_CUI_list_suite(VTEST_TEST_SUITE *suite)
{
	for(;suite; suite = suite->next_suite) {
		MSG( "%s ",suite->name);
	}
}

int VTEST_CUI_list_tests_of_suite(VTEST_TEST_SUITE *suite,const char *suite_name)
{
	int ret = 0;
	VTEST_TEST *test;
			

	for(;suite; suite = suite->next_suite) {
		if (strcmp(suite->name,suite_name) == 0) {
			ret = 1;

			for(test = suite->test_cases; test->name != 0; test++) {
				
				MSG( "%s ",test->name);
			}
		}
	}

	return ret;
}

