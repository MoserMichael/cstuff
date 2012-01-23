#include "parsectx.h"
#include "parser.h"
#include "yystype.h"
#include "pars.h"
#include "ast.h"

int MY_YY_input();

static int hash_compare(HASH_Entry *entry, void * key, ssize_t key_length)
{
  FUNCTION_HASH_entry *lhs;	

  (void) key_length;

  lhs = (FUNCTION_HASH_entry *) entry;

  if (strcmp(lhs->decl->f_name,key) == 0) {
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

  YYLTYPE_init( &stam );   
  ctx->root_ctx = AST_FUNC_DECL_init( 0, 0, 0, &stam );
  ctx->current = ctx->root_ctx;
 
  return 0;
}

void PARSECONTEXT_free( PARSECONTEXT *ctx )
{
  LEXER_free( &ctx->lexctx );
}

int PARSECONTEXT_add_function_def(  PARSECONTEXT *ctx, struct tagAST_FUNC_DECL *decl )
{
  FUNCTION_HASH_entry *entry;
  
  entry = (FUNCTION_HASH_entry *) HASH_find(  &ctx->map_function_defs, (void *) decl->f_name, -1 );
  if (entry) {
    do_yyerror ( &decl->base.location,  ctx, "The function has been defined twice. first definiton at line %d column %d", entry->decl->base.location.first_line, entry->decl->base.location.first_column);
    return -1;
  }

  entry = (FUNCTION_HASH_entry *) malloc( sizeof( FUNCTION_HASH_entry ) );
  if (!entry) {
    return -1;
  }

  if (HASH_insert( &ctx->map_function_defs, &entry->entry, (void *) decl->f_name, (ssize_t) -1  )) {
    return -1;
  }

  TREE_insert_child( &ctx->current->funcs, &decl->funcs, TREE_INSERT_LAST, 0 );
  ctx->current  = decl;
  return 0;
}



