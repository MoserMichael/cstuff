#!/bin/bash

set -x
set -e

VERSIONDIR=1.4.14b-stable

VERSION=libevent-${VERSIONDIR}

if [ "x$1" = "xclean" ]; then
  rm -rf $VERSION
  rm -rf install
  rm -rf libevent
  exit 0
fi

if [ ! -d $VERSION ]; then
  tar xvfz $VERSION.tar.gz
fi

if [ ! -h libevent ]; then
  ln -s $VERSION libevent
fi

if [  -e install/lib/libevent.a ]; then
  exit 0
fi

if [ ! -d install ]; then
  mkdir install
fi
INSTALL=$(readlink -f install)

export CFLAGS="-m32 -g -fno-omit-frame-pointer -O2" 
export LDFLAGS=-m32

pwd
cp  -rf patch/* libevent/

pushd libevent
./configure --disable-shared --prefix $INSTALL

make clean
make 
make install
popd
