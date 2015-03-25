#!/bin/bash

#set -x

#--------------------------------------------------------------

OPTS="-sgo"
STATUS=0
OUT_DIR="out"

if [ ! -d $OUT_DIR ]; then
    mkdir $OUT_DIR
fi

LS=`ls *_test.py`
for f in $LS
do
    echo "running: ${f}"
    #../ntrace/trace.py ${OPTS} $f >${OUT_DIR}/${f}.out 2>${OUT_DIR}/${f}.err
    python -m ntrace/trace ${OPTS} $f >${OUT_DIR}/${f}.out 2>${OUT_DIR}/${f}.err
 
    ERRSIZE=`stat --format=%s ${OUT_DIR}/${f}.err`
    if [ "x$ERRSIZE" != "x0" ]; then
	echo "test $f failed"
	cat ${OUT_DIR}/${f}.err
	STATUS=1
    fi
done

echo "running: test_ntrace.py"
python test_ntrace.py >${OUT_DIR}/test_ntrace.out 2>${OUT_DIR}/test_ntrace.err
ERRSIZE=`stat --format=%s ${OUT_DIR}/test_ntrace.err`
if [ "x$ERRSIZE" != "x0" ]; then
	echo "test test_ntrace.py failed"
	cat ${OUT_DIR}/$f.err
	STATUS=1
fi

exit $STATUS
