#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <cutils/bhash.h>
#include <cutils/array.h>
#include <pooh-lan/ast.h>

struct tagAST_PP_RULE; 
struct tagAST_BASE;
struct tagPARSECONTEXT;

// ******************************************************************

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
  int is_reporting_circle;
  AST_BASE_LIST *rules_list;
}
  GRAMMARCHECKERCTX;


GRAMMARCHECKERCTX * GRAMMAR_init( struct tagPARSECONTEXT *pctx  );
void GRAMMAR_free( GRAMMARCHECKERCTX * ctx );
int GRAMMAR_add_rule( GRAMMARCHECKERCTX *ctx,  const char *rule_name, struct tagAST_PP_RULE *rule );
int GRAMMAR_checker( GRAMMARCHECKERCTX *ctx, struct tagAST_BASE *program);

// ******************************************************************

#include <cutils/circbuf.h>


int PARSER_DATA_SRC_file (struct tagCIRCBUF *buf, size_t *read, void *ctx );

typedef struct tagPEG_PARSER {
  AST_PP_BASE *root;		// root of parsed grammar
  
  CIRCBUF lookahead;		// lookahead buffer for data.
  PEG_PARSER_POS offset;        // curren position in lookahead buffer (the current character)
  int    undo_nesting;          // is this a nested option? if yes then we must be able to backtrack.
  ARRAY	 error_stack;		// stack of error entries / each clause that fails leaves an entry.
    
  PEG_PARSER_DATA_SRC data_src; // data source
  void *data_src_ctx;		// context data for data source.
  S_PP_RULE_TYPE rtype;		// currently parsing a rule or a token.

  union tagBINDING_DATA *fail_reason;	// value set by x_failparsing.
  int	is_fail_parsing;	// is set if semantic action has stopped the parsing (x_failparsing called)

  struct tagEVAL_CTX *eval_ctx;		// needed to run associated scripts.


  PP_BASE_INFO * current_rhs_clause;	// during script invocation: runtime gets righ hand side clauses from here.
  size_t	current_rhs_clause_len;

} PEG_PARSER;

int PEG_PARSER_init( PEG_PARSER *parser, struct tagAST_BASE *program, size_t lookahead_size, PEG_PARSER_DATA_SRC data_source, void *data_src_ctx,  struct tagEVAL_CTX *eval_ctx );
 

int PEG_PARSER_run( PEG_PARSER *parser, PP_BASE_INFO *rinfo );


#endif

