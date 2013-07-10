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
#include <cutils/circbufn.h>
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

// ******************)************************************************
//
typedef struct tagPEG_MEMOIZE_TERMS_ENTRY {
  HASH_Entry   hentry;
  DRING	       dentry;
  AST_PP_RULE *rule;
  PP_BASE_INFO info;
} PEG_MEMOIZE_TERMS_ENTRY;

typedef struct tagPEG_MEMOIZE_TERMS {
  BHASH map_offset_to_entry;	// map offset to entry
  DRING root;	// link all entries sorted by offset (in order to discard passed parses)
} PEG_MEMOIZE_TERMS; 

// init cache
int PEG_MEMOIZE_TERMS_init( PEG_MEMOIZE_TERMS * cache );

int peg_parser_memoize_hash_compare( HASH_Entry  *entry, const void * key, ssize_t key_length)
{
  PEG_MEMOIZE_TERMS_ENTRY *lhs;	

  (void) key_length;

  lhs = (PEG_MEMOIZE_TERMS_ENTRY *) entry;

  return lhs->info.start_idx.lookahead_offset == (size_t) key;
}

int PEG_MEMOIZE_TERMS_init( PEG_MEMOIZE_TERMS * cache )
{
  HASH_init( &cache->map_offset_to_entry, 10, 0, peg_parser_memoize_hash_compare, 0 );
}

// add memoization of parsing of given rule.
int PEG_MEMOIZE_TERMS_add( PEG_MEMOIZE_TERMS * cache, AST_PP_RULE *rule, PP_BASE_INFO *info );

int PEG_MEMOIZE_TERMS_add( PEG_MEMOIZE_TERMS * cache, AST_PP_RULE *rule, PP_BASE_INFO *info )
{
  PEG_MEMOIZE_TERMS_ENTRY *entry;

  entry = (PEG_MEMOIZE_TERMS_ENTRY *) malloc( sizeof(PEG_MEMOIZE_TERMS_ENTRY) );
  if (!entry)
    return -1;
    
    
  return -1;    
}
// lookup memoization at given offset.
int PEG_MEMOIZE_TERMS_lookup( PEG_MEMOIZE_TERMS *cache, size_t offset, PP_BASE_INFO **info );

// discards all items at offsets smaller then offset argument
int PP_MEMOIZE_TERMS_discard( PEG_MEMOIZE_TERMS *cache, size_t offset );

// ******************************************************************

#define PEG_PARSER_TRACE_RULES  1
#define PEG_PARSER_TRACE_TOKENS 2

typedef struct tagPEG_PARSER {
  AST_PP_BASE *root;		// root of parsed grammar
  
  PP_CHAR ch; 
  CIRCBUF lookahead;		// lookahead buffer for input about to be parsed.
  PEG_MEMOIZE_TERMS memoize;	// memoization cache - don't parse the same rule twice - try to reuse / cache parses.
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

