TOPDIR=../..


TESTS=evcorothreadtest evcorothreadtestsh  


evcorothreadtest_TYPE=exe

evcorothreadtest_SRC_EXTENSION=c

evcorothreadtest_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

evcorothreadtest_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

evcorothreadtest_LIBS=corothread evcorothread cutils butils nutils vtest pthread event rt


evcorothreadtestsh_TYPE=exe

evcorothreadtestsh_SRC_EXTENSION=c

evcorothreadtestsh_LIBS=corothreadsh evcorothreadsh cutilssh butilssh nutilssh vtest pthread event rt

evcorothreadtestsh_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

evcorothreadtestsh_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib





include $(TOPDIR)/rules.make
