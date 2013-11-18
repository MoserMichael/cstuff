TOPDIR=..

POSTBUILD_SUBDIRS=test

ifeq "$(shell uname -o)" "Cygwin"
TARGETS=corothrjmp
else
TARGETS=corothructx 
endif


# --- 

corothructx_TYPE=lib

corothructx_SRC_EXTENSION=c

corothructx_LIBS=cutils butils pthread

corothructx_CFLAGS=-DHAS_UCONTEXT


# --- 

corothrjmp_TYPE=lib

corothrjmp_SRC_EXTENSION=c

corothrjmp_CFLAGS:=-DHAS_SETJMP_LONGJMP

include $(TOPDIR)/rules.make


