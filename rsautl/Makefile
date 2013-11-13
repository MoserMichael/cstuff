TOPDIR=..

PREBUILD_SUBDIRS=openssl
TARGETS=rsaenc rsadec

# ---------------------------------------

rsaenc_TYPE=exe

rsaenc_SRC_EXTENSION=c

rsaenc_SRC=rsautl.c

rsaenc_CFLAGS=-Iopenssl/include -DENCRYPT

rsaenc_LDFLAGS=-Lopenssl/lib

rsaenc_LIBS=ssl crypto dl


# ---------------------------------------

rsadec_TYPE=exe

rsadec_SRC=rsautl.c

rsadec_CFLAGS=-Iopenssl/include

rsadec_LDFLAGS=-Lopenssl/lib/ 

rsadec_LIBS=ssl crypto dl

# ---------------------------------------


include $(TOPDIR)/rules.make



.PHONY: test
test :
	./test.sh

