TOPDIR=../../..


TESTS=examplecoro examplethread showheaderservlet helloworldservlet

# script checks targets examplecoro and examplethread
TEST_TOOL=./test-embed.sh

# ---------------------------------------
# embedding example with coroutine libray
# ---------------------------------------

examplecoro_TYPE=exe

examplecoro_SRC=embed_example.c

examplecoro_LIBS=webbylib webbycorolib evcorothread event corothread hutils nutils butils cutils dl rt pthread

examplecoro_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

examplecoro_LDFLAGS=-L$(ROOT_DIR)/evcorothread/libevent/install/lib

# ---------------------------------------
# embedding example with threading libray
# ---------------------------------------

examplethread_TYPE=exe

examplethread_SRC=embed_example.c

examplethread_LIBS=webbylib webbythreadlib hutils tutils nutils butils cutils dl pthread


# ---------------------------------------
# example servlet shared library
# ---------------------------------------

showheaderservlet_TYPE=shlib

showheaderservlet_SRC=showheaderservlet.c

showheaderservlet_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include


# ---------------------------------------
# example servlet shared library
# ---------------------------------------

helloworldservlet_TYPE=shlib

helloworldservlet_SRC=helloworld.c

helloworldservlet_CFLAGS=-I$(ROOT_DIR)/evcorothread/libevent/install/include

# ---------------------------------------

include $(TOPDIR)/rules.make

