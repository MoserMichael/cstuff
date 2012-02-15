#ifndef __PARSE_CTX_H_Y__
#define __PARSE_CTX_H_Y__


#include "lexer.h"
#include "checker.h"

#include <cutils/bhash.h>

struct tagAST_BASE;
struct tagAST_FUNC_DECL;

//--------------------------------------------------------
typedef struct tagFUNCTION_HASH_entry {
  HASH_Entry entry;
  struct tagAST_FUNC_DECL *decl; 
} FUNCTION_HASH_entry;

//--------------------------------------------------------

typedef struct tagPARSECONTEXT {
	
  LEXCONTEXT lexctx;
  CHECKERCTX chkctx;

  /*  *** error status *** */
  int report_errors;
  int my_yy_is_error;
  
  struct tagAST_BASE * my_ast_root;

  struct tagAST_FUNC_DECL *root_ctx;
  struct tagAST_FUNC_DECL *current;  
 
  HASH map_function_defs; // maps function name to function definition. (AST_FUNC_DECL *)

} PARSECONTEXT;

int  PARSECONTEXT_init( PARSECONTEXT *ctx );
void PARSECONTEXT_free( PARSECONTEXT *ctx );


struct tagAST_FUNC_DECL; 

int PARSECONTEXT_add_function_def( PARSECONTEXT *ctx, struct tagAST_FUNC_DECL *decl ); 
struct tagAST_FUNC_DECL * PARSECONTEXT_find_function_def( PARSECONTEXT *ctx, const char *fname );

//--------------------------------------------------------

int parse_string( LEXCONTEXT *pc );
int parse_string_oneline( LEXCONTEXT *pc );

#endif
