TOPDIR=../..

## prebuild step - configure
#PREBUILD_SUBDIRS=config

# postbuild step - run tests
POSTBUILD_SUBDIRS=test 

#TARGETS:=dmems dmemc
TARGETS:=tracemem tracememcpp


#---

tracemem_TYPE=shlib

tracemem_SRC=tracemem.c

tracemem_CFLAGS=

tracemem_LIBS=dl

#---

tracememcpp_TYPE=shlib

tracememcpp_SRC=tracemem.c

tracememcpp_CFLAGS=-x c++ -fno-check-new
 

tracememcpp_LIBS=dl stdc++

 
#---

include $(TOPDIR)/rules.make
