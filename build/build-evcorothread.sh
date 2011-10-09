#!/bin/bash

TARGETS="vtest cutils nutils butils tutils corothread evcorothread"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh


