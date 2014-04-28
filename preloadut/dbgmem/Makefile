TOPDIR=../..

# prebuild step - configure
PREBUILD_SUBDIRS=config

# postbuild step - run tests
POSTBUILD_SUBDIRS=test systest

#TARGETS:=dmems dmemc
TARGETS:=dmemc dmems dbgmemutil dmemccpp dmemscpp dbgmemutilcpp


#---

dmemc_TYPE=shlib

dmemc_SRC=memalloc.c

dmemc_CFLAGS=-D_ADVANCED_TOOL_ -Wno-unused-result -fno-inline

dmemc_LIBS=dl

dmemc_INSTALL:
	$(call install-a-file,../../scripts/run,$(PREFIX)/etc)


#---

dmems_TYPE=shlib

dmems_SRC=memalloc.c

dmems_CFLAGS=-D_SIMPLE_TOOL_ -Wno-unused-result
 

dmems_LIBS=dl

#---

dbgmemutil_TYPE=shlib

dbgmemutil_SRC=dbgutil.c 
 
dbgmemutil_LIBS=dl
 
#---

dmemccpp_TYPE=shlib

dmemccpp_SRC=memalloc.c
 

dmemccpp_CFLAGS=-D_ADVANCED_TOOL_ -x c++ -Wno-unused-result -fno-inline 
 
 
dmemccpp_LIBS=dl stdc++  

#---

dmemscpp_TYPE=shlib

dmemscpp_SRC=memalloc.c

dmemscpp_CFLAGS=-D_SIMPLE_TOOL_	-x c++ -Wno-unused-result 

dmemscpp_LIBS=dl stdc++

#---

dbgmemutilcpp_TYPE=shlib

dbgmemutilcpp_SRC=dbgutil.c

dbgmemutilcpp_CFLAGS=-x c++

#---

include $(TOPDIR)/rules.make
