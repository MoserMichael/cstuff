#!/bin/sh

FILE=../config.h

if [ -f $FILE ]; then
  exit 0
fi

set -x

cat <<EOF >${FILE}
#ifndef __CONF_OPTS__
#define __CONF_OPTS__

EOF

# -- test for thread local keyword --
gcc $CFLAGS  configure_has_thread.c -o config 
if [ "x$?" = "x0" ]; then 
 echo "#define  __RECURSIVE_MALLOC " >>${FILE}
fi

cat <<EOF >>${FILE}

#include <limits.h>

#ifndef __WORDSIZE
 #define __WORDSIZE (sizeof(int) * 8)
#endif

#endif

EOF


