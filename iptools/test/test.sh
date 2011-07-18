#!/bin/bash

set -x

if [ "x$BIN_ROOT_DIR" = "x" ]; then
  cat <<EOF
BIN_ROOT_DIR environment not defined.
Run make from current directory.
EOF
  exit 1
fi

LOOP_UDP=$BIN_ROOT_DIR/bin/loop_udp

$LOOP_UDP -i fragment3000.dump -n

$LOOP_UDP -i fragment3000.dump

