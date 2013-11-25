#!/bin/bash


TARGETS="vtest cutils butils tutils rsautl"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



