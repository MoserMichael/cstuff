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
 echo "/*** has tls ***/" >>${FILE} 
 echo "#define  __RECURSIVE_MALLOC " >>${FILE}
fi

# -- test for sigaction
gcc $CFLAGS  configure_sigaction.c -o config 
if [ "x$?" = "x0" ]; then 
 echo "/*** has sigaction ***/"  >>${FILE} 
 echo "#define  HAVE_SIGACTION " >>${FILE}
fi

cat <<EOF >>${FILE}

#include <limits.h>

#ifndef __WORDSIZE
 #define __WORDSIZE (sizeof(int) * 8)
#endif

#endif

EOF

rm -f config


