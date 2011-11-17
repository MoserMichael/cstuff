#ifndef _SYNTAX_H_
#define _SYNTAX_H_


#include "lexer.h"


int PARSER_init();
int PARSER_free();
int PARSER_run(struct tagAST_BASE **root);


void do_yyerror (YYLTYPE *loc, const char  *format, ...);

#endif
