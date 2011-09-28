TOPDIR=../..


TESTS=iothreadtest iothreadtestsh  


iothreadtest_TYPE=exe

iothreadtest_SRC_EXTENSION=c

iothreadtest_LIBS=iothread cutils butils nutils vtest pthread


iothreadtestsh_TYPE=exe

iothreadtestsh_SRC_EXTENSION=c

iothreadtestsh_LIBS=iothreadsh cutilssh butilssh nutilssh vtest pthread

iothreadtestsh_CFLAGS=-I$(ROOT_DIR)/iothreads/libevent/install/include

iothreadtestsh_LDFLAGS=-L$(ROOT_DIR)/iothreads/libevent/install/lib





include $(TOPDIR)/rules.make
