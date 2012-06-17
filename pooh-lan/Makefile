TOPDIR=..


# ---------------------------------------

PREBUILD_SUBDIRS=rtlib

# ---------------------------------------

TARGETS=parser
   

parser_TYPE=exe

parser_SRC=lexer.c parser.c ast.c parsectx.c printast.c checker.c main.c 

parser_DEPENDENCIES=pars.txt lex.txt 

parser_LIBS=cutils butils m rtlib


parser_POSTBUILD=./test-it.pl $(BIN_ROOT_DIR)/bin/parser test test-error .


# ---------------------------------------

include $(TOPDIR)/rules.make



lex.txt : lex.l
		flex  -o $@ $<  
             


pars.txt : pars.y
		bison --warnings=error --report=all --report-file=pars.report --defines=pars.h -o $@ $< 	
		../scripts/parse-bison-report.pl pars.report pars.txt 
