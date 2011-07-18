#!/bin/sh

set -x
set -e

OBJDIR=obj
#OBJDIR=../../CYGWIN_NT-6.1-1.7.9-0.237-5-3--i686/obj/cygdrive/c/mystuff/make-system/test/projB/shlib

BINDIR=lib

LIBDIR=lib
#LIBDIR=../../CYGWIN_NT-6.1-1.7.9-0.237-5-3--i686/lib

mkdir -p $OBJDIR $LIBDIR


CC=g++

$CC -fPIC -c -o $OBJDIR/slib.o -I. slib.c 
$CC -Wl,-soname,libshlibso.so -shared  -o $LIBDIR/libshlibso.dll $OBJDIR/slib.o 


objdump -T $LIBDIR/libshlibso.dll
#-Wl,--export-all-symbols,-soname,libshlibso.so 

$CC  -c -o $OBJDIR/slibuser.o -I. slibuser.c  
$CC -o $BINDIR/slibexe $OBJDIR/slibuser.o -L$LIBDIR -lshlibso

echo "*** ok ***"
