/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef __CPPLIBA_H__
#define __CPPLIBA_H__

#include "cpplib.h"


class rrvalue : public rvalue
{
public:
  rrvalue(int _extvalue, int basevalue);

  int extvalue;     
};

#endif


