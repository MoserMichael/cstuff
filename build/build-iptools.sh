#!/bin/bash


TARGETS="vtest cutils iptools"

DIR=`dirname $0`

if [ "x$DIR" != "x" ]; then
  cd $DIR
fi

pushd ../iptools/libpcap
make clean
popd

. common-build.sh



