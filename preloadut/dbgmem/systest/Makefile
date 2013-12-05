TOPDIR=../../..


#TESTS= mdbgtest3 mdbgleaklib mdbgleaksrv
TESTS= mdbgleaklib mdbgleaksrv


TEST_TOOL=true

#--

mdbgtest3_TYPE=exe

mdbgtest3_SRC=test3.cpp

mdbgtest3_CFLAGS=-I..

mdbgtest3_LIBS=dbgmemutil stdc++

mdbgtest3_POSTBUILD=./mdbgtest3.pl ${ROOT_DIR}/scripts
 
#--

mdbgleaklib_TYPE=shlib

mdbgleaklib_SRC=leaklib.c

#--

mdbgleaksrv_TYPE=exe

mdbgleaksrv_SRC=leaksrv.c

mdbgleaksrv_LIBS=mdbgleaklib nutils tutils pthread

mdbgleaksrv_POSTBUILD=./mdbgleaksrv.pl ${ROOT_DIR}/scripts
 

include $(TOPDIR)/rules.make
