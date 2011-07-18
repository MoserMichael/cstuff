/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */


#include "tlib.h"
#include "slib.h"

int tlib_export(int a)
{
  return slib_export(a+1);
}
