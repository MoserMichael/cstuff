#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <cutils/bhash.h>
#include <cutils/array.h>
#include <pooh-lan/ast.h>

struct tagAST_PP_RULE; 
struct tagAST_BASE;
struct tagPARSECONTEXT;

typedef struct tagGRAMMARCHECKERCTX 
{
  HASH rule_defs; // maps rule name to rule definition.
  struct tagPARSECONTEXT *ctx; // for error messages only
  ARRAY visited;

  // state used for parsing.
  AST_PP_RULE *current_rule;
  PARSE_ALT *parse_alt, parse_rule_alt;
  ARRAY  current_alt_stack;
  PARSE_CHAR_CLASS char_class_state;
}
  GRAMMARCHECKERCTX;


int GRAMMAR_init(  GRAMMARCHECKERCTX * ctx,  struct tagPARSECONTEXT *pctx  );
int GRAMMAR_add_rule( GRAMMARCHECKERCTX *ctx,  const char *rule_name, struct tagAST_PP_RULE *rule );
int GRAMMAR_checker( GRAMMARCHECKERCTX *ctx, struct tagAST_BASE *program);



#endif

