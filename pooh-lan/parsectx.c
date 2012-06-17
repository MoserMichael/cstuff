#include "parsectx.h"
#include "parser.h"
#include "yystype.h"
#include "pars.h"
#include "ast.h"

int MY_YY_input();

static int hash_compare(HASH_Entry *entry, const void * key, ssize_t key_length)
{
  FUNCTION_HASH_entry *lhs;	

  (void) key_length;

  lhs = (FUNCTION_HASH_entry *) entry;

  if (strcmp( ((AST_FUNC_DECL *) lhs->decl)->f_name,key) == 0) {
    return 0;
  }
  return 1;
}

//------------------------------------------------------------------------

STRING_PART *STRING_PART_init( int is_expression, YYLTYPE *location  )
{
  STRING_PART *ret;

  ret = (STRING_PART *) malloc( sizeof( STRING_PART ) );
  if (!ret) {
    return 0;
  }

 
  ret->is_expression = is_expression;
  ret->loc = *location;
  
  DBUF_init( &ret->part_data, 0 );
 
  return ret;
}

void STRING_PART_free( STRING_PART *part  )
{ 
  DBUF_free( &part->part_data );
  free(part);
}

//------------------------------------------------------------------------

int PARSECONTEXT_init(PARSECONTEXT *ctx)
{
  YYLTYPE stam;

  if (  LEXER_init( &ctx->lexctx ) ) {
    return -1;
  }

  if ( CHECKER_init( &ctx->chkctx ) ) {
    return -1;
  }

   if (HASH_init( &ctx->map_function_defs, 10, 0, hash_compare, 0 ) ) {
    return -1;
  }

  ctx->my_yy_is_error = 0;
  ctx->my_ast_root = 0; 
  ctx->report_errors = 1;
  ctx->stmt_not_closed = 0;

  YYLTYPE_init( &stam );   
  ctx->root_ctx = AST_FUNC_DECL_init( 0, 0, 0, &stam );
  ctx->current = ctx->root_ctx;
 
  return 0;
}

void PARSECONTEXT_free( PARSECONTEXT *ctx )
{
  LEXER_free( &ctx->lexctx );
}

void PARSECONTEXT_add_function_def2(  PARSECONTEXT *ctx, struct tagAST_FUNC_DECL *decl )
{
  if ( decl->f_name) {
    PARSECONTEXT_add_function_def( ctx, (AST_XFUNC_DECL *) decl );
  }

  TREE_insert_child( &ctx->current->funcs, &decl->funcs, TREE_INSERT_LAST, 0 );
  ctx->current  = decl;
}

void PARSECONTEXT_add_function_def(  PARSECONTEXT *ctx, struct tagAST_XFUNC_DECL  *decl )
{
  FUNCTION_HASH_entry *entry;
  AST_FUNC_DECL *fdecl;

  assert( decl->base.type == S_FUN_DECL || decl->base.type == S_XFUN_DECL );
  
  fdecl = (AST_FUNC_DECL *) decl;
  entry = (FUNCTION_HASH_entry *) HASH_find(  &ctx->map_function_defs, (void *) fdecl->f_name, -1 );
  if (entry) {
    do_yyerror( &decl->base.location, ctx, "The function %s has been defined twice. ",
		    fdecl->f_name );

    if (entry->decl->base.type == S_FUN_DECL) {		    
      do_yyerror( &entry->decl->base.location, ctx  , "First definition of function %s was here" );
    }  
    return ;
  }

  entry = (FUNCTION_HASH_entry *) malloc( sizeof( FUNCTION_HASH_entry ) );
  if (!entry) {
    return ;
  }
  
  entry->decl = decl;
//entry->name = fdecl->f_name;

  if (HASH_insert( &ctx->map_function_defs, &entry->entry, (void *) fdecl->f_name, (ssize_t) -1  )) {
    return ;
  }

  return ;
}

struct tagAST_BASE * PARSECONTEXT_find_function_def( PARSECONTEXT *ctx, const char *fname ) 
{
  FUNCTION_HASH_entry *entry;
  
  entry = (FUNCTION_HASH_entry *) HASH_find(  &ctx->map_function_defs, (void *) fname, -1 );
  if (entry) {
    return (AST_BASE *) entry->decl;
  }
  return 0;
}



