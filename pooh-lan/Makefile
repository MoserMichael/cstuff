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

pooh_LDFLAGS=-rdynamic

# the generated lex/bison parser compiles with some warnings, so do not treat warnings as errors for these files.

lexer.c_CFLAGS=-Wno-error

parser.c_CFLAGS=-Wno-error

pooh_LIBS=$(TLIB) pthread cutils butils m rtlib nutils dl

pooh_POSTBUILD=./test-it.pl $(BIN_ROOT_DIR)/bin/pooh test-error test .


# ---------------------------------------

include $(TOPDIR)/rules.make



lex.txt : lex.l
		flex  -o $@ $<  
             


pars.txt : pars.y
		bison --warnings=error --report=all --report-file=pars.report --defines=pars.h -o $@ $< 	
		../scripts/parse-bison-report.pl pars.report pars.txt 
