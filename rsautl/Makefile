TOPDIR=..

PREBUILD_SUBDIRS=openssl

#TARGETS=rsaencmt rsadecmt
TARGETS=rsaenc rsadec rsaencmt rsadecmt

# ---------------------------------------

rsaenc_TYPE=exe

rsaenc_SRC_EXTENSION=c

rsaenc_SRC=rsautl.c

rsaenc_CFLAGS=-Iopenssl/include -DENCRYPT

rsaenc_LDFLAGS=-Lopenssl/lib

rsaenc_LIBS=butils ssl crypto dl


# ---------------------------------------

rsadec_TYPE=exe

rsadec_SRC=rsautl.c

rsadec_CFLAGS=-Iopenssl/include

rsadec_LDFLAGS=-Lopenssl/lib/ 

rsadec_LIBS=butils cutils tutils butils pthread ssl crypto dl

# ---------------------------------------

rsaencmt_TYPE=exe

rsaencmt_SRC_EXTENSION=c

rsaencmt_SRC=rsautl.c

rsaencmt_CFLAGS=-Iopenssl/include -DENCRYPT -DMULTI 
#rsaencmt_CFLAGS=-Iopenssl/include -DENCRYPT -DMULTI -D_TRACE
 

rsaencmt_LDFLAGS=-Lopenssl/lib

rsaencmt_LIBS=cutils tutils butils pthread ssl crypto dl


# ---------------------------------------

rsadecmt_TYPE=exe

rsadecmt_SRC=rsautl.c

rsadecmt_CFLAGS=-Iopenssl/include -DMULTI 
#rsadecmt_CFLAGS=-Iopenssl/include -DMULTI -D_TRACE 


rsadecmt_LDFLAGS=-Lopenssl/lib/ 

rsadecmt_LIBS=cutils tutils butils pthread ssl crypto dl

# ---------------------------------------


include $(TOPDIR)/rules.make



.PHONY: test
test :
	./test.sh

