#!/bin/bash

TARGETS="vtest cutils nutils corothread"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh


