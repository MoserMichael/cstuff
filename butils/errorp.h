/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __ERRORP_H__
#define __ERRORP_H__


int  errorp_open_file(const char *file);

void errorp_close_file();

void errorp(int rval, const char *fmt, ... );

#endif


