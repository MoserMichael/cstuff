TOPDIR=..


TARGETS:=ntracenativemodule
# ----------------------------------------------------------------------

ntracenativemodule_TYPE=shlib

ntracenativemodule_SRC_EXTENSION=c

ntracenativemodule_CFLAGS= 

ntracenativemodule_LIBS-Cygwin=python2.7
ntracenativemodule_LIBS-GNU-Linux=

ntracenativemodule_LIBS=$(ntracenativemodule_LIBS-$(OS_TYPE))

ntracenativemodule_POSTBUILD=./test_ntrace.sh
# ----------------------------------------------------------------------

include $(TOPDIR)/rules.make
