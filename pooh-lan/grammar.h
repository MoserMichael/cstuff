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
#include <cutils/dbuf.h>

// ******************************************************************

typedef enum {
  PERR_TERM_EXPECTED_HERE,	    // a terminal or token was expected at this position ;
				    // only applies if past the first position of a sequence
  PERR_TERM_NOT_EXPECTED_HERE,	    // multiplicity error: token not expected here (multiplicity 0)
  PERR_TERM_NOT_ENOUGH_OCCURANCES,  // multiplicity error: not enought occurances
  PERR_ALTERNATIVE_NOT_PARSED,      // none of the choices for this alternative matched.
  PERR_STOPPED_BY_SEMANTIC_ACTION,  // semantic action stopped parsing.

} PEG_PARSER_ERROR;

typedef struct tagPEG_ERROR_ENTRY {
  PEG_PARSER_ERROR type;	// error type
  PEG_PARSER_POS start_pos;	// position where error occured at.
  AST_PP_BASE *term;		// term that failed parsing.
  union {
    struct {
	int expected;
	int actual;
    } multiplicity;
    //BINDING_DATA *semantic_error_msg;
  } u;    
} PEG_ERROR_ENTRY;

// ******************************************************************

int PARSER_DATA_SRC_file (struct tagCIRCBUF *buf, size_t *read, void *ctx );

// ******************************************************************

#define PEG_PARSER_TRACE_RULES  1
#define PEG_PARSER_TRACE_TOKENS 2

typedef struct tagPEG_PARSER {
  AST_PP_BASE *root;		// root of parsed grammar
  
  CIRCBUF lookahead;		// lookahead buffer for data.
  PEG_PARSER_POS offset;        // curren position in lookahead buffer (the current character)
  int    undo_nesting;          // is this a nested option? if yes then we must be able to backtrack.
  
  ARRAY	 error_stack;		// stack of error entries / each clause that fails leaves an entry.
  int    error_set_enabled;
  int    num_non_terminal;      // number of non terminal as seen from current top level rule (must be backtracked)

  //PEG_ERROR_ENTRY   error_entry;
  //int		    error_entry_set;

  PEG_PARSER_DATA_SRC data_src; // data source
  void *data_src_ctx;		// context data for data source.
  S_PP_RULE_TYPE rtype;		// currently parsing a rule or a token.

  union tagBINDING_DATA *fail_reason;	// value set by x_failparsing.
  int	is_fail_parsing;	// is set if semantic action has stopped the parsing (x_failparsing called)

  struct tagEVAL_CTX *eval_ctx;		// needed to run associated scripts.


  PP_BASE_INFO * current_rhs_clause;	// during script invocation: runtime gets righ hand side clauses from here.
  size_t	current_rhs_clause_len;

  DBUF trace_clause; 
  int trace_flags;

} PEG_PARSER;

int PEG_PARSER_init( PEG_PARSER *parser, struct tagAST_BASE *program, size_t lookahead_size, PEG_PARSER_DATA_SRC data_source, void *data_src_ctx,  struct tagEVAL_CTX *eval_ctx );
 

int PEG_PARSER_run( PEG_PARSER *parser, PP_BASE_INFO *rinfo );


#endif

