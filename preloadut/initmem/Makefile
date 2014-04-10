TOPDIR=../..

## prebuild step - configure
#PREBUILD_SUBDIRS=config

# postbuild step - run tests
POSTBUILD_SUBDIRS=test 

#TARGETS:=dmems dmemc
TARGETS:=initmem initmemcpp initmeme initmemecpp 


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

initmeme_TYPE=shlib

initmeme_SRC=initmem.c

initmeme_CFLAGS=-DEXACT_SIZE 

initmeme_LIBS=dl

#---

initmemecpp_TYPE=shlib

initmemecpp_SRC=initmem.c

initmemecpp_CFLAGS=-x c++ -fno-check-new -DEXACT_SIZE 

initmemecpp_LIBS=dl stdc++


include $(TOPDIR)/rules.make
