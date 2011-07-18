#!/bin/sh

set -x
set -e

#OBJDIR=obj
OBJDIR=../../CYGWIN_NT-6.1-1.7.9-0.237-5-3--i686/obj/cygdrive/c/mystuff/make-system/test/projB/shlib

BINDIR=.

#LIBDIR=lib
LIBDIR=../../CYGWIN_NT-6.1-1.7.9-0.237-5-3--i686/lib

mkdir -p $OBJDIR $LIBDIR

gcc -c -o $OBJDIR/slib.o -I. slib.c 
ar rv $LIBDIR/libslib.a $OBJDIR/slib.o
gcc  -c -o $OBJDIR/slibuser.o -I. slibuser.c  
gcc -o slibexe $OBJDIR/slibuser.o -L$LIBDIR -lslib

echo "*** ok ***"
