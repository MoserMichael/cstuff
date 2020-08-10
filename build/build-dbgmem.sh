#!/bin/bash

set -x

TARGETS="vtest cutils butils nutils tutils preloadut/dbgmem preloadut/tracemem"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



