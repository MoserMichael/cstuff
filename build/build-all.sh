#!/bin/bash


TARGETS="make-system-test vtest cutils butils nutils tutils stack corothread pooh-lan evcorothread hutils webby iptools"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



