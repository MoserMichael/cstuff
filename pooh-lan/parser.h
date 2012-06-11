#ifndef _SYNTAX_H_
#define _SYNTAX_H_


#include "parsectx.h"

PARSECONTEXT * PARSER_init();
int PARSER_free(PARSECONTEXT *ctx);
int PARSER_run(PARSECONTEXT *ctx, struct tagAST_BASE **rval );

void do_yyerror (YYLTYPE *loc,  PARSECONTEXT *parse_context, const char  *format, ...);
void do_yywarning (YYLTYPE *loc, PARSECONTEXT *parse_context, const char  *format, ...);

#endif
