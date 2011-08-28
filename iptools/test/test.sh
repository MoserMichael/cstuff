#!/bin/bash

set -x

if [ "x$BIN_ROOT_DIR" = "x" ]; then
  cat <<EOF
BIN_ROOT_DIR environment not defined.
Run make from current directory.
EOF
  exit 1
fi


function test_loop_udp()
{
  local cnt

  LOOP_UDP=$BIN_ROOT_DIR/bin/loop_udp
 
  cnt=`$LOOP_UDP -s -i fragment3000.dump | wc -l`
  if [ "x$cnt" != "x1" ]; then
     echo "Error: expected 1 line - loop_udp assemble fragments"
     exit 1
  fi

  cnt=`$LOOP_UDP -s -n -i fragment3000.dump | wc -l`
  if [ "x$cnt" != "x3" ]; then
     echo "Error: expected  line - loop_udp do not assemble fragments"
     exit 1
  fi


}

test_loop_udp 

cat <<EOF
***
all tests passed 
***
EOF
