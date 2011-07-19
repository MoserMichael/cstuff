TOPDIR=..


TARGETS=stack 

TESTS=tstop tstopf tstop3 tstop3f tst

# do not invoke the test program, test is run by the POSTBUILD script
TEST_TOOL=true

# ---------------------------------------

stack_TYPE=shlib

stack_SRC=stack.c

stack_LIBS=c

stack_POSTBUILD=./stack.sh

   
# ---------------------------------------

tst_TYPE=exe

tst_SRC=test.c

tst_CFLAGS=-finstrument-functions 

tst_POSTBUILD=./stack.sh

# ---------------------------------------

tstopf_TYPE=exe

tstopf_SRC=test.c

tstopf_CFLAGS=-finstrument-functions -O2 -fno-omit-frame-pointer 

tstopf_POSTBUILD=./stack.sh

# ---------------------------------------

tstop_TYPE=exe

tstop_SRC=test.c

tstop_CFLAGS=-finstrument-functions -O2 

tstop_POSTBUILD=./stack.sh


# ---------------------------------------

tstop3f_TYPE=exe

tstop3f_SRC=test.c

tstop3f_CFLAGS=-finstrument-functions -O3 -fno-omit-frame-pointer 

tstop3f_POSTBUILD=./stack.sh

# ---------------------------------------

tstop3_TYPE=exe

tstop3_SRC=test.c

tstop3_CFLAGS=-finstrument-functions -O3 

tstop3_POSTBUILD=./stack.sh


# ---------------------------------------

tstgcc_TYPE=exe

tstgcc_SRC=test.c

tstgcc_CFLAGS=-fstack-protector-all

# ---------------------------------------

include $(TOPDIR)/rules.make

.PHONY: stack_INSTALL
stack_INSTALL:
	$(call install-a-file,../scripts/readerrors.pl,$(PREFIX)/bin)
 
