TOPDIR=..

PREBUILD_SUBDIRS=libevent
POSTBUILD_SUBDIRS=test

TARGETS:=evcorothread evcorothreadsh

# --- 

evcorothread_TYPE=lib

evcorothread_SRC_EXTENSION=c

evcorothread_CFLAGS=-Ilibevent/install/include


# --- 

evcorothreadsh_TYPE=shlib

evcorothreadsh_SRC_EXTENSION=c

evcorothreadsh_CFLAGS=-Ilibevent/install/include

evcorothreadsh_LIBS=$(evcorothreadsh_LIBS-$(OS_TYPE)) 
evcorothreadsh_LIBS-Cygwin=cutilssh butilssh nutils event corothrjmp 
evcorothreadsh_LIBS-GNU-Linux=

evcorothreadsh_LDFLAGS=$(evcorothreadsh_LDFLAGS-$(OS_TYPE))

evcorothreadsh_LDFLAGS-Cygwin=-L$(ROOT_DIR)/evcorothread/libevent/install/lib
evcorothreadsh_LDFLAGS-GNU-Linux=


include $(TOPDIR)/rules.make
