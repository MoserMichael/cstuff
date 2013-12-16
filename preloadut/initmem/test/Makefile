TOPDIR=../../..


TESTS=initmemtest initcppmemtest


TEST_TOOL=true

#---

initmemtest_TYPE=exe

initmemtest_SRC=test.c 

initmemtest_CFLAGS=-I..

initmemtest_LIBS=

initmemtest_POSTBUILD=./initmemtest.pl ${ROOT_DIR}/scripts initmemtest

#---

initcppmemtest_TYPE=exe

initcppmemtest_SRC=test.cpp

initcppmemtest_CFLAGS=-I..

initcppmemtest_LIBS=

initcppmemtest_POSTBUILD=./initmemtest.pl ${ROOT_DIR}/scripts initcppmemtest



include $(TOPDIR)/rules.make
