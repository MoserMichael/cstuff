TOPDIR=../../..


TESTS=mdbgtest mdbgtest2 mdbgtest3

TEST_TOOL=true

#---

mdbgtest_TYPE=exe

mdbgtest_SRC=test.c

mdbgtest_CFLAGS=-I..

mdbgtest_LIBS=dbgmemutil

#---

mdbgtest2_TYPE=exe

mdbgtest2_SRC=test.c

mdbgtest2_CFLAGS=-I.. -x c++ 

mdbgtest2_LIBS=dbgmemutil stdc++

mdbgtest2_POSTBUILD=./sys-test.pl $(BIN_ROOT_DIR)/lib $(BIN_ROOT_DIR)/test c
 
#--

mdbgtest3_TYPE=exe

mdbgtest3_SRC=test3.cpp

mdbgtest3_CFLAGS=-I..

mdbgtest3_LIBS=dbgmemutil stdc++

mdbgtest3_POSTBUILD=./sys-test.pl $(BIN_ROOT_DIR)/lib $(BIN_ROOT_DIR)/test s
 
 



include $(TOPDIR)/rules.make
