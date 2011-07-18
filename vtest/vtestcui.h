/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __VTEST_CUI_H_
#define __VTEST_CUI_H_

#ifdef  __cplusplus
extern "C" {
#endif

int VTEST_CUI_test_runner(VTEST_TEST_SUITE *suite);

int VTEST_CUI_test_runner_cmdline(VTEST_TEST_SUITE *suite, int argc,  char *argv[]);


typedef void (*PFN_DEBUG_FUNCTION) (const char *msg);


void VTEST_CUI_set_debug_output_hook( PFN_DEBUG_FUNCTION debug_output_hook );

#ifdef  __cplusplus
}
#endif

#endif

