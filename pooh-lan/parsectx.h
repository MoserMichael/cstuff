#ifndef __PARSE_CTX_H_Y__
#define __PARSE_CTX_H_Y__


#include "lexer.h"
#include "checker.h"
#include "grammar.h"

#include <cutils/bhash.h>

struct tagAST_BASE;
struct tagAST_FUNC_DECL;

//--------------------------------------------------------
typedef struct tagFUNCTION_HASH_entry {
  HASH_Entry entry;
  struct tagAST_XFUNC_DECL *decl; 
} FUNCTION_HASH_entry;

//--------------------------------------------------------

typedef struct tagPARSECONTEXT {
	
  LEXCONTEXT lexctx;
  CHECKERCTX chkctx;

  GRAMMARCHECKERCTX *grctx;
  ARRAY grammar_contexts;
 
  /*  *** error status *** */
  int report_errors;
  int my_yy_is_error;

  struct tagAST_BASE * my_ast_root;

  struct tagAST_FUNC_DECL *current,*root_ctx;
  struct tagAST_BASE_LIST *stmt_list,*main_stmt_list;
  
  HASH map_function_defs; // maps function name to function definition. (AST_FUNC_DECL *)

  int stmt_not_closed;
  
  ARRAY extension_libs;

} PARSECONTEXT;

int  PARSECONTEXT_init( PARSECONTEXT *ctx, INC_PATH *inc_path , AST_FUNC_DECL *current  );
void PARSECONTEXT_free( PARSECONTEXT *ctx );


typedef void (*SHOW_SOURCE_LINE) (YYLTYPE *loc, PARSECONTEXT *parse_context );

struct tagAST_FUNC_DECL; 
struct tagAST_XFUNC_DECL; 

void PARSECONTEXT_add_function_def2(  PARSECONTEXT *ctx, struct tagAST_FUNC_DECL *decl );
void PARSECONTEXT_add_function_def( PARSECONTEXT *ctx, struct tagAST_XFUNC_DECL *decl ); 
struct tagAST_BASE * PARSECONTEXT_find_function_def( PARSECONTEXT *ctx, const char *fname );

//--------------------------------------------------------

int parse_string( LEXCONTEXT *pc , char start_char );
int parse_string_oneline( LEXCONTEXT *pc );
int parse_expression_only( LEXCONTEXT *pc, DBUF *parent );
int do_parse_string_no_expression( LEXCONTEXT *pc, char start_char );


#endif
