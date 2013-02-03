TOPDIR=../..

ifeq "$(shell uname -o)" "Cygwin"
TESTS=evcorothreadjmptest 
else
TESTS=evcorothreaductxtest 
endif

#--

evcorothreaductxtest_TYPE=exe

evcorothreaductxtest_SRC_EXTENSION=c

evcorothreaductxtest_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

evcorothreaductxtest_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

evcorothreaductxtest_LIBS=corothructx evcorothread cutils butils nutils vtest pthread event rt

#--

evcorothreadjmptest_TYPE=exe

evcorothreadjmptest_SRC_EXTENSION=c

evcorothreadjmptest_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

evcorothreadjmptest_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

evcorothreadjmptest_LIBS=corothrjmp evcorothread cutils butils nutils vtest pthread event rt

#--



include $(TOPDIR)/rules.make
