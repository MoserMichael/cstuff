TOPDIR=../..

## prebuild step - configure
#PREBUILD_SUBDIRS=config

# postbuild step - run tests
POSTBUILD_SUBDIRS=test 

#TARGETS:=dmems dmemc
TARGETS:=initmem initmemcpp


#---

initmem_TYPE=shlib

initmem_SRC=initmem.c

initmem_CFLAGS=

initmem_LIBS=dl

#---

initmemcpp_TYPE=shlib

initmemcpp_SRC=initmem.c

initmemcpp_CFLAGS=-x c++ -fno-check-new
 

initmemcpp_LIBS=dl stdc++

 
#---

include $(TOPDIR)/rules.make
