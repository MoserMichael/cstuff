#!/bin/bash

set -x
set -e

VERSION=1.1.1
ACTION=$1

if [ "x$BIN_ROOT_DIR" = "x" ]; then
  echo "BIN_ROOT_DIR environment not defined."
fi

BIN_ROOT_DIR=`readlink -f $BIN_ROOT_DIR`

if [ "x$ACTION" = "xclean" ]; then
   rm -rf libpcap-${VERSION}  
   rm -rf inst
   exit 0
fi

if [ ! -d libpcap-${VERSION} ]; then
  tar xvfz libpcap-${VERSION}.tar.gz
  cp -f pcap-linux.c libpcap-${VERSION}/
fi

if [ -d inst ]; then
  exit 0
fi


DIR=$BIN_ROOT_DIR
DIR_OTHER=$PWD

pushd libpcap-${VERSION}

MNAME=`uname -m`

if [ "x$MNAME" = "xi686" ]; then
  export CFLAGS=-m32
  export LDFLAGS=-m32
fi

if [ "x$MNAME" = "xx86_64" ]; then
  export CFLAGS=-m64
  export LDFLAGS=-m64
fi

#DEBUG_CFG=" --disable-shared  --disable-optimizer   
# --prefix=$PWD/inst  

./configure --bindir $BIN_ROOT_DIR/bin --libdir $BIN_ROOT_DIR/lib --includedir $DIR_OTHER/inst/inc -datarootdir $DIR_OTHER/inst/share

 

if [ "x$DEBUG_CFG" != "x" ]; then
  sed -e s#\-O2#-g#g Makefile >Makefile.out
  cp -f Makefile.out Makefile
fi

make clean
make 
make install

rm -f $BIN_ROOT_DIR/lib/libpcap.so*
popd
