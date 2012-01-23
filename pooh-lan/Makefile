TOPDIR=..


TARGETS=parser
   
# ---------------------------------------


parser_TYPE=exe

parser_SRC=lexer.c ast.c parsectx.c checker.c parser.c main.c 

parser_DEPENDENCIES=pars.txt lex.txt 

parser_LIBS=cutils butils


parser_POSTBUILD=./test-it.pl test 


# ---------------------------------------

include $(TOPDIR)/rules.make



lex.txt : lex.l
		flex  -o $@ $<  
             


pars.txt : pars.y
		bison --warnings=error --report=all --report-file=pars.report --defines=pars.h -o $@ $< 	
		../scripts/parse-bison-report.pl pars.report pars.txt 
