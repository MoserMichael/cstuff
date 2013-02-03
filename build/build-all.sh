#!/bin/bash


TARGETS="make-system-test vtest cutils butils nutils tutils stack corothread pooh-lan iptools evcorothread hutils webby"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



