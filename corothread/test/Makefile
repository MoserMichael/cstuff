TOPDIR=../..


TESTS=corothreadtest  corothreadtestsh


corothreadtest_TYPE=exe

corothreadtest_SRC_EXTENSION=c

corothreadtest_LIBS=corothread cutils nutils vtest pthread


corothreadtestsh_TYPE=exe

corothreadtestsh_SRC_EXTENSION=c

corothreadtestsh_LIBS=corothreadsh cutilssh nutilssh vtest pthread

corothreadtestsh_CFLAGS=-I$(ROOT_DIR)/iothreads/libevent/install/include

corothreadtestsh_LDFLAGS=-L$(ROOT_DIR)/iothreads/libevent/install/lib





include $(TOPDIR)/rules.make
