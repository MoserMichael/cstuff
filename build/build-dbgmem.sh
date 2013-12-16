#!/bin/bash


TARGETS="vtest cutils butils nutils tutils preloadut/dbgmem"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



