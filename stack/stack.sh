#!/bin/bash


set -x
set -e

EXE_FILE=$1
TST_NAME=`basename $EXE_FILE`
OUT_DIR=out

ROOT_DIR=`readlink -f $BIN_ROOT_DIR` 
export LD_PRELOAD=$ROOT_DIR/lib/libstack.so

mkdir -p $OUT_DIR

$EXE_FILE 2>$OUT_DIR/out.$TST_NAME || true
../scripts/readerrors.pl $OUT_DIR/out.$TST_NAME >$OUT_DIR/out.$TST_NAME.result







