#!/bin/bash


set -x
set -e

BIN_ROOT=GNU-Linux-2.6.38-8-generic-i686 
BIN_DIR=../$BIN_ROOT/bin
LIB_DIR=../$BIN_ROOT/lib

export LD_PRELOAD=$PWD/$LIB_DIR/libstack.so

rungdb ./$BIN_DIR/tst 

echo "*** the same with optimization but with frames ***"

./$BIN_DIR/tstop



echo "*** C++ excpeption test ***"

./$BIN_DIR/ex


echo "*** setjmp/longjmp test ***"

./$BIN_DIR/jmp
