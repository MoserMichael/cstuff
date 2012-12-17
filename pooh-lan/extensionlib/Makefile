TOPDIR=../..


TARGETS:=extlibexample

# ----------------------------------------------------------------------

extlibexample_TYPE=shlib

extlibexample_SRC_EXTENSION=c

# only cygwin needs this
#extlibexample_LIBS=rtlib cutils butils nutils

ifeq "$(OS_TYPE)" ""
OS_TYPE:=$(shell uname -o | sed -e 's\#/\#-\#g' -e 's\#(\#-\#g' -e 's\#)\#-\#g' )
export OS_TYPE
endif

extlibexample_LIBS-Cygwin=rtlib cutils butils nutils
extlibexample_LIBS-GNU-Linux=

extlibexample_LIBS=$(extlibexample_LIBS-$(OS_TYPE))


# ----------------------------------------------------------------------

include $(TOPDIR)/rules.make
