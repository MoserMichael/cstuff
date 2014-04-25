TOPDIR=../../..


TESTS=tracememtest tracecppmemtest


TEST_TOOL=true

#---

tracememtest_TYPE=exe

tracememtest_SRC=test.c 

tracememtest_CFLAGS=-I..

tracememtest_LIBS=

tracememtest_POSTBUILD=./tracememtest.pl ${ROOT_DIR}/scripts tracememtest test.html

#---

tracecppmemtest_TYPE=exe

tracecppmemtest_SRC=testo.cpp

tracecppmemtest_SRC_EXTENSION=cpp

tracecppmemtest_CFLAGS=-I..

tracecppmemtest_LIBS=stdc++

tracecppmemtest_POSTBUILD=./tracememtest.pl ${ROOT_DIR}/scripts tracecppmemtest testcpp.html



include $(TOPDIR)/rules.make
