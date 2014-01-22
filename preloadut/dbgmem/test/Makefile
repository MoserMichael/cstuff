TOPDIR=../../..


TESTS=mdbgtest mdbgtest2 


TEST_TOOL=true

#---

mdbgtest_TYPE=exe

mdbgtest_SRC=test.c testm.c

mdbgtest_CFLAGS=-I..

mdbgtest_LIBS=dbgmemutil

#---

mdbgtest2_TYPE=exe

mdbgtest2_SRC=test.c testm.c 

mdbgtest2_CFLAGS=-I.. -x c++ 

mdbgtest2_LIBS=dbgmemutil stdc++

#mdbgtest2_POSTBUILD=/bin/sh -c "set -e ; ./sys-test.pl $(BIN_ROOT_DIR)/lib $(BIN_ROOT_DIR)/test s" 
mdbgtest2_POSTBUILD=/bin/sh -c "set -e ; ./sys-test.pl $(BIN_ROOT_DIR)/lib $(BIN_ROOT_DIR)/test s ; ./sys-test.pl $(BIN_ROOT_DIR)/lib $(BIN_ROOT_DIR)/test c" 
 
#--
 
 



include $(TOPDIR)/rules.make
