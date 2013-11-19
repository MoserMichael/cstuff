/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __ERRORP_H__
#define __ERRORP_H__


/**
 * @defgroup ERRORP 
 * @brief error logging with stack trace (not the symbols, just the addresses).
 * @{
 */

int  errorp_open_file(const char *file);

int  errorp_is_file_open();

void errorp_close_file();

void errorp(int rval, const char *fmt, ... );

/**
 * @brief install crash handler; crash handler prints stack trace and exits
 */
void install_crash_handler();


/**
 * @}
 */

#endif


