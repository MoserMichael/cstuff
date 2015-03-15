#!/bin/bash

#set -x

ONAME=$(uname -o)


if [[ "x$ONAME" == "xCygwin" ]]; then
  if [ -e $BIN_ROOT_DIR/lib/libntracenativemodule.dll ]; then
  	mv $BIN_ROOT_DIR/lib/libntracenativemodule.dll  $BIN_ROOT_DIR/lib/ntracenativemodule.dll
  fi
else
  if [ -e $BIN_ROOT_DIR/lib/libntracenativemodule.so ]; then
  	mv $BIN_ROOT_DIR/lib/libntracenativemodule.so  $BIN_ROOT_DIR/lib/ntracenativemodule.so
  fi
fi
             

#--------------------------------------------------------------

TEST_DIR=test-ntrace

LS=`ls ${TEST_DIR}/*.py`

OPTS="-sgo"
STATUS=0

for f in $LS
do
    echo "running: ${f}"
    python ntrace.py $OPTS $f >$f.out 2>$f.err
    ERRSIZE=`stat --format=%s $f.err`
    if [ "x$ERRSIZE" != "x0" ]; then
	echo "test $f failed"
	cat $f.err
	STATUS=1
    fi
done

echo "running: test_ntrace.py"
python test_ntrace.py >${TEST_DIR}/test_ntrace.out 2>${TEST_DIR}/test_ntrace.err
ERRSIZE=`stat --format=%s ${TEST_DIR}/test_ntrace.err`
if [ "x$ERRSIZE" != "x0" ]; then
	echo "test test_ntrace.py failed"
	cat $f.err
	STATUS=1
fi

exit $STATUS
