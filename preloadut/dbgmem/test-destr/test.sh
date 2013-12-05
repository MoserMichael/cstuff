#!/bin/sh

set -x
set -e

gcc -shared -fPIC -Wl,-soname,libshlib.so -o libshlib.so -lc -ldl shlib.c  
gcc main.c -o main

export LD_PRELOAD=$PWD/libshlib.so

./main
