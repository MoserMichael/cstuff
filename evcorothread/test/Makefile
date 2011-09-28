TOPDIR=../..


TESTS=iothreadtest iothreadtestsh  


iothreadtest_TYPE=exe

iothreadtest_SRC_EXTENSION=c

iothreadtest_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

iothreadtest_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

iothreadtest_LIBS=iothread cutils butils nutils vtest pthread event rt


iothreadtestsh_TYPE=exe

iothreadtestsh_SRC_EXTENSION=c

iothreadtestsh_LIBS=iothreadsh cutilssh butilssh nutilssh vtest pthread event rt

iothreadtestsh_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

iothreadtestsh_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib





include $(TOPDIR)/rules.make
