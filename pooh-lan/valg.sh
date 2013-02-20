#!/bin/bash

#set -x
set -e

FILL_OPTS="--malloc-fill=0xde --free-fill=0xbf"

export TEST_TOOL="valgrind -q --max-stackframe=10485760 $FILL_OPTS"

OS_TYPE=`uname -o | sed -e 's#/#-#g' -e 's#(#-#g' -e 's#)#-#g' `

OS_VERSION=`uname -r | sed -e 's#/#-#g' -e 's#(#-#g' -e 's#)#-#g' `

ARCH=`uname -m`

#-
# Name of directory where built results are put in.
#-
PLATFORM_NAME="$OS_TYPE-$OS_VERSION-$ARCH"

echo "running tests with valgrind. Please Wait (tm)"

./test-it.pl ../$PLATFORM_NAME/bin/pooh test lib >valg.log 2>&1


ERRORS=`egrep -n "^==[^=]" valg.log` || true

test_ok=0

TEST_OK=`tail -n 1 valg.log`
if [ "x$TEST_OK" != "xTest ok" ]; then
   echo "Test Failed. Oy oy oy "
   test_ok=1
else 
   echo "Test ok, without errors"
fi

if [ "x$ERRORS" != "x" ]; then
   echo "Valgrind errors, oy oy oy"
   test_ok=1
else   
   echo "No valgrind errors"
fi

exit ${test_ok}



