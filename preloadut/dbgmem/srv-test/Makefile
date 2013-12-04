TOPDIR=../../..


TESTS=testlib testexe

TEST_TOOL=true

#---

testlib_TYPE=shlib

testlib_SRC=testlib.c

testlib_CFLAGS=-I..

testlib_LIBS=dbgmemutil

#---

testexe_TYPE=exe

testexe_SRC=test.c

testexe_CFLAGS=-I..

testexe_LIBS=testlib

 
#--


include $(TOPDIR)/rules.make
