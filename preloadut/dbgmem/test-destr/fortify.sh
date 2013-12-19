#!/bin/sh
set -x
set -e

gcc -O3 fortify.c -o fortify
./fortify
gcc -O2 fortify.c -o fortify
./fortify
gcc -O1 fortify.c -o fortify
./fortify
gcc fortify.c -o fortify
./fortify



