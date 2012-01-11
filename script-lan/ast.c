#include "ast.h"
#include "pars.h"
#include "parsectx.h"
#include "lexer.h"
#include "parser.h"

AST_BASE *compile_string( const char *string, int init_token, const char *file_name, YYLTYPE *location, int report_errors )
{
   PARSECONTEXT ctx;
   AST_BASE *rval = 0;
   
   if (PARSECONTEXT_init( &ctx) ) {
     return 0;
   }

   if (LEXER_open_string( &ctx.lexctx, string, init_token   ) ) {
     goto ret;
   } 

   LEXER_set_file_name( &ctx.lexctx, strdup( file_name  ) );
   LEXER_set_location(  &ctx.lexctx, location );

   if (!report_errors) {
     ctx.report_errors = 0;
   } 

   if ( PARSER_run( &ctx, &rval ) ) {
     rval = 0;
   }

ret:
   PARSECONTEXT_free( &ctx );
   return 0;
}

 
static AST_EXPRESSION *AST_compile_multi_part_string_impl( PARSECONTEXT *pc, size_t pos )
{
  STRING_PART **ptr, *cur;
  AST_EXPRESSION *lhs,*rhs, *scl; 
  AST_BASE *base;
  const char *current_file;


  ptr = (STRING_PART **) ARRAY_at( &pc->lexctx.string_parts, pos  );
  cur = *ptr;
   
  current_file = LEXER_get_current_file_name( &pc->lexctx );

  if (cur->is_expression) {
    base = compile_string( STRING_PART_get( cur ), TK_START_STATEMENT, current_file, &cur->loc, 0  ); 
    if (!base) {
      base = compile_string( STRING_PART_get( cur ), TK_START_EXPRESSION, current_file, &cur->loc, 0 );
    }

    if (base == 0) {
      compile_string( STRING_PART_get( cur ), TK_START_STATEMENT, current_file, &cur->loc, 1  ); 
      compile_string( STRING_PART_get( cur ), TK_START_EXPRESSION, current_file, &cur->loc, 1 );
      return 0;
    }
    
    if (base->type != S_EXPRESSION) {
      AST_FUNC_DECL *fdecl;
      AST_FUNC_CALL *fcall;
      AST_VECTOR *fparams;

      // make a function,
      fparams = AST_VECTOR_init( &cur->loc  );
      fdecl = AST_FUNC_DECL_init( 0, fparams, 0, &cur->loc  );
      AST_FUNC_DECL_set_body( fdecl, pc, (AST_BASE_LIST *) base );

      // make a function call
      fparams = AST_VECTOR_init( &cur->loc );
      fcall = AST_FUNC_CALL_init( 0, fparams, &cur->loc );
      fcall->func_decl = fdecl;

      // make a function call expression
      rhs  = AST_EXPRESSION_init( S_FUN_CALL, S_VAR_ANY, &cur->loc  ); 
      rhs->val.fcall = fcall;
    
    } else {
      rhs = (AST_EXPRESSION *) base;
    }
 
  } else {
    rhs = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, &cur->loc ); 
    rhs->val.const_value.string_value = strdup( STRING_PART_get( cur ) );
  }

  lhs = pos > 0 ? AST_compile_multi_part_string_impl( pc, pos - 1 ) : 0;
  if (!lhs) {
    return rhs;
  }
  scl = AST_EXPRESSION_init_binary( TK_OP_STR_CAT, lhs, rhs );
  scl->value_type = S_VAR_STRING;  
  return scl;

  
}

AST_EXPRESSION *AST_compile_multi_part_string( PARSECONTEXT *pc)
{
  size_t nparts = ARRAY_size( &pc->lexctx.string_parts );
  if (!nparts) {
    assert(0);
    return 0;
  }
  return AST_compile_multi_part_string_impl( pc, nparts - 1 ); 
}

// both operators must be scalars and cannot be string values.
int is_numeric_operator( int op )
{
  switch(op) {
    case TK_OP_NUM_SUBST:
    case TK_OP_NUM_ADD:
    case TK_OP_NUM_DIV:
    case TK_OP_NUM_MULT:
    case TK_OP_NUM_MOD:
    case TK_OP_NUM_EQ: 
    case TK_OP_NUM_NE: 
    case TK_OP_NUM_LT: 
    case TK_OP_NUM_GT: 
    case TK_OP_NUM_LE: 
    case TK_OP_NUM_GE: 
      return 1;
    default:
      return 0;
  }
}


// both operators can be either string or numbers; numbers are implicitly converted to string
int is_string_operator( int op )
{
  switch( op ) {
    case TK_OP_STR_CAT:  
    case TK_OP_STR_EQ:
    case TK_OP_STR_NE: 
    case TK_OP_STR_LT: 
    case TK_OP_STR_GT: 
    case TK_OP_STR_LE: 
    case TK_OP_STR_GE:           
      return 1;
    default:
      return 0;
  }
}

int AST_EXPRESSION_check_types( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs )
{
    M_UNUSED( scl );
    M_UNUSED( op );
    M_UNUSED( lhs );
    M_UNUSED( rhs );

    if (lhs->value_type ==  S_VAR_ANY && rhs->value_type == S_VAR_ANY) {
      return 0;
    }

    if (lhs->value_type ==  S_VAR_CODE || rhs->value_type == S_VAR_CODE) {
       // can't mix lambda function with operator
       do_yyerror (YYLTYPE *loc,  PARSECONTEXT *parse_context, const char  *format, ...);

 
       return -1;   
    }
    
    // if division - check that divisor is not constant with value 0
    if (op == TK_OP_NUM_DIV && rhs->exp_type == S_EXPR_CONSTANT) {
    
	if (rhs->value_type == S_VAR_INT && rhs->val.const_value.int_value == 0) {
	    // divide by zero error.
	}

	if (rhs->value_type == S_VAR_DOUBLE && rhs->val.const_value.double_value == 0) {
	    // divide by zero error.
	}
    }
 
    if (is_numeric_operator( op) ) {

	if (lhs->value_type != S_VAR_ANY &&  lhs->value_type != (S_VAR_INT | S_VAR_DOUBLE) ) {
	  // numeric operator expects numeric value
	}

	if (rhs->value_type != S_VAR_ANY &&  rhs->value_type != (S_VAR_INT | S_VAR_DOUBLE) ) {
	  // numeric operator expects numeric value
	}
    }
    if (is_string_operator( op) ) {

	if (lhs->value_type != S_VAR_ANY && lhs->value_type & (S_VAR_CODE | S_VAR_HASH | S_VAR_LIST) ) {
	   
	}

	if (rhs->value_type != S_VAR_ANY &&  rhs->value_type & (S_VAR_CODE | S_VAR_HASH | S_VAR_LIST) ) {
	  // numeric operator expects numeric value
	}
    }
    return 0;
}

int AST_EXPRESSION_fold_constants( AST_EXPRESSION *scl, int op,  AST_EXPRESSION *lhs, AST_EXPRESSION *rhs )
{
    M_UNUSED( scl );
    M_UNUSED( op );
    M_UNUSED( lhs );
    M_UNUSED( rhs );
#if 0
    if (lhs->exp_type != S_EXPR_CONSTANT || rhs->exp_type != S_EXPR_CONSTANT) {
      return -1;
    } 

    switch( op ) {
      case TK_ADD:
        break;
    }
#endif
    return 0;
   
}


