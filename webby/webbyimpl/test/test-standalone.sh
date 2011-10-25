#!/bin/sh

set -x 
set -e

BIN_ROOT_DIR=../../../GNU-Linux-2.6.38-8-generic-i686/
export LD_LIBRARY_PATH=$BIN_ROOT_DIR/lib

$BIN_ROOT_DIR/bin/webbycoro -c tconfig.props -s $BIN_ROOT_DIR/test/libshowheaderservlet.so -s $BIN_ROOT_DIR/test/libhelloworldservlet.so &
pid=$!
trap "kill $pid" EXIT

sleep 1

curl -vv http://127.0.0.1:12321/headerservlet
curl -vv http://127.0.0.1:12321/headerservlet
curl -vv http://127.0.0.1:12321/headerservlet
curl -vv http://127.0.0.1:12321/headerservlet
curl -vv http://127.0.0.1:12321/headerservlet

curl -vv http://127.0.0.1:12321/helloworldservlet 
curl -vv http://127.0.0.1:12321/helloworldservlet 
curl -vv http://127.0.0.1:12321/helloworldservlet 
curl -vv http://127.0.0.1:12321/helloworldservlet 
curl -vv http://127.0.0.1:12321/helloworldservlet 

echo "*** test completed ***"




