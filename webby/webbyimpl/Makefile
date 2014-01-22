TOPDIR=../..


TARGETS:=webbythreadlib webbycorolib webbycoro webbythread

POSTBUILD_SUBDIRS=test

# ---------------------------------------

webbythreadlib_TYPE=lib

webbythreadlib_SRC=webbythread.c

# ---------------------------------------

webbycorolib_TYPE=lib

webbycorolib_SRC=webbycoro.c

webbycorolib_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

# ---------------------------------------


webbythread_TYPE=exe

webbythread_SRC=webbymain.c webbymain_blurb.c

webbythread_LIBS=webbylib webbythreadlib hutils tutils nutils butils cutils dl pthread

webbythread_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

webbythread_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

# ---------------------------------------

webbycoro_TYPE=exe

webbycoro_SRC=webbymain.c webbymain_blurb.c

webbycoro_LIBS=$(webbycoro_LIBS-$(OS_TYPE))

webbycoro_LIBS-Cygwin=webbylib webbycorolib evcorothread corothrjmp event hutils nutils butils cutils dl rt pthread
webbycoro_LIBS-GNU-Linux=webbylib webbycorolib evcorothread corothructx event hutils nutils butils cutils dl rt pthread

webbycoro_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

webbycoro_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

# ---------------------------------------



include $(TOPDIR)/rules.make


webbymain_blurb.c : webbymain_blurb.txt
	perl $(TOPDIR)/scripts/make-blurb.pl $<


