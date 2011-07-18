/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VTESTRUNNER_H_
#define _VTESTRUNNER_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <sys/time.h>

typedef enum {
  VTEST_TEST_FAILED = 0,
  VTEST_TEST_OK,

  VTEST_SUITE_SETUP_FAILED,
  VTEST_SUITE_SETUP_OK,

  VTEST_SUITE_TEARDOWN_FAILED,
  VTEST_SUITE_TEARDOWN_OK,
  
}
  VTEST_STATUS;

typedef void (*VTEST_RUNNER_report_suite_start) (const char *suite_name);

typedef void (*VTEST_RUNNER_report_test_start)  (const char *suite_name, const char *test_name, 
												 int iteration, int maxiteration);

typedef void (*VTEST_RUNNER_report_results)	   (VTEST_STATUS status, 
							const char *suite_name, const char *test_name, 
							struct timeval *duration,
							const char *fail_cond, const char *fail_file,int fail_line);

typedef void (*VTEST_RUNNER_report_wrapup)	   (int suitesinitfailed, 
												int suitesteardownfailed, 
												int tests_passed, 
												int tests_failed, 
												int testnotrun);


typedef struct tagVTEST_RUNNER_IMPL {
	
	VTEST_RUNNER_report_suite_start suite_start;
	VTEST_RUNNER_report_test_start  test_start;
	VTEST_RUNNER_report_results     results;
	VTEST_RUNNER_report_wrapup      wrapup;

	int				  current_test_state;
	VTEST_STATUS	  scope_fail;
	VTEST_TEST_SUITE *suite;
	VTEST_TEST		 *test;

} VTEST_RUNNER_IMPL;

/**
  @brief run all test suites.
  @param suite (in) the first test suite out of a chain of test suites.
  @param impl (in) class that implements a test runner.
 */

int VTEST_test_runner(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl);

/**
  @brief run selected list of suites and tests; selection is specified via command line

  @brief run all test suites.
  @param suite (in) the first test suite out of a chain of test suites.
  @param impl (in) class that implements a test runner.
  @param argc (in) number of strings in command line
  @param argv (in) command line strings.

  command line specified by arguments argv and argc.
		
		<cmd_line>   ::= <cmd_line> SPACE <build_spec>  | <build_spec>
		
		<build_spec> ::= SUITENAME | SUITENAME/<test_list>

		<test_list>  ::= <test_list>,TESTNAME | TESTNAME

		SUITENAME	 name of a test suite
		TESTNAME	 name of a test suite.

  If a SUITENAME or TESTNAME does not exist, then it does not match and is not run; (no error are reported for these conditions).

  Explanation:
		Empty command line means - run all tests.
		
		Test suites are always run in the order of their declaration.

		Can select execution of individual suite, and suites with selected set of list.

 */	
int VTEST_test_runner_cmdline(VTEST_TEST_SUITE *suite, VTEST_RUNNER_IMPL *impl, int argc, char *argv[]);


/* 
	A unit test can call this function in order to set the next test suite to execute.
	Good for implementation of test loops.
	
	Scenario: run a series of test, the last test changes global environment and goes to first test suite;
		      Test series is now run with modified global environment.
 */
void VTEST_goto_next_suite(const char *suite_name);

#ifdef  __cplusplus
}
#endif

#endif

