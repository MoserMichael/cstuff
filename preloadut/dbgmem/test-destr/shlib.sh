#!/bin/sh

set -x
set -e

gcc -O3 mall.c -o mall
gcc -O3 -shared -fPIC -Wl,-soname,libshlib.so -o libshlib.so -lc -ldl shlib.c  
gcc -O3 main.c -o main

# exports from libc put into file exports
objdump -T `ldd main | grep libc.so  | awk '{ print $3 }'` >exports

#run test - if destr is printed then shared library destructor function has been called.
export LD_PRELOAD=$PWD/libshlib.so

./main
