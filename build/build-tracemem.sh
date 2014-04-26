#!/bin/bash


TARGETS="vtest preloadut/tracemem"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

. common-build.sh



