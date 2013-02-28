TOPDIR=..


ifeq "$(shell uname -o)" "Cygwin"
TLIB=corothrjmp  
else
TLIB=corothructx 
endif


# ---------------------------------------

PREBUILD_SUBDIRS=rtlib extensionlib

# ---------------------------------------

TARGETS=pooh
   

pooh_TYPE=exe

pooh_SRC=inc.c lexer.c parser.c ast.c parsectx.c printast.c checker.c ld.c main.c eval.c 

pooh_DEPENDENCIES=pars.txt lex.txt 


ifeq "$(OS_TYPE)" ""
OS_TYPE:=$(shell uname -o | sed -e 's\#/\#-\#g' -e 's\#(\#-\#g' -e 's\#)\#-\#g' )
export OS_TYPE
endif

pooh_LDFLAGS-Cygwin=-Wl,--export-all-symbols
pooh_LDFLAGS-GNU-Linux=-rdynamic -fPIC

pooh_LDFLAGS=$(pooh_LDFLAGS-$(OS_TYPE))

# the generated lex/bison parser compiles with some warnings, so do not treat warnings as errors for these files.

lexer.c_CFLAGS=-Wno-error

parser.c_CFLAGS=-Wno-error

pooh_LIBS=$(TLIB) pthread cutils butils m rtlib nutils dl

pooh_POSTBUILD=./test-it.pl $(BIN_ROOT_DIR)/bin/pooh test-error test lib .

# ---------------------------------------

include $(TOPDIR)/rules.make

pooh_INSTALL:
	$(eval $(call install-mkdir,$(PREFIX)/lib/pooh))
	$(call install-a-file,lib/matrix.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/complex.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/heap.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/queue.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/stack.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/slist.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/graphlist.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/graphmatrix.inc,$(PREFIX)/lib/pooh)
	$(call install-a-file,lib/graphutils.inc,$(PREFIX)/lib/pooh)



lex.txt : lex.l
		flex  -o $@ $<  
             


pars.txt : pars.y
		bison --warnings=error --report=all --report-file=pars.report --defines=pars.h -o $@ $< 	
		../scripts/parse-bison-report.pl pars.report pars.txt 
