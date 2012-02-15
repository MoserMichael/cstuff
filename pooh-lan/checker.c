#include "parsectx.h"
#include "ast.h"
#include "parser.h"


int CHECKER_init( CHECKERCTX *ctx)
{  
  ctx->is_left_hand_side = 0; 
  ctx->current_function  = 0;
}

int CHECK_function_scope( AST_FUNC_DECL *fdecl,  PARSECONTEXT *ctx );

BINDING_ENTRY * make_binding( AST_VAR_TYPE value_type, YYLTYPE *location, const char *name )
{
   BINDING_ENTRY *binding;

   binding = (BINDING_ENTRY *) malloc( sizeof( BINDING_ENTRY ) );
   if (!binding) {
     return 0;
   }

   binding->data.def_location = *location;
   binding->data.value_type = value_type;
   binding->binding_name = name;

   switch( value_type ) {
    case S_VAR_INT:
      binding->data.value.long_value = 0;
      break;
    case S_VAR_DOUBLE:
      binding->data.value.double_value = 0;
      break;
    case S_VAR_STRING:
      binding->data.value.string_value = 0;
      break;
    case S_VAR_CODE:
      break;
    case S_VAR_HASH:
      if (HASH_init( &binding->data.value.hash_value, 10, 0, binding_hash_compare, 0 ) ) {
        free(binding);
        return 0;
      }
      break;
    case S_VAR_LIST:
      if (ARRAY_init( &binding->data.value.array_value, 0, sizeof( BINDING_DATA ) ) ) {
        free(binding);
        return 0;
      }
      break;
    case S_VAR_ANY:
      ;
   }
   return binding;
}

BINDING_ENTRY *lookup_binding( const char *name, AST_FUNC_DECL *decl )
{
   BINDING_ENTRY *entry; 
    
   if (name[0] != '#') {
	 return  (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, (void *) name, -1);
   } else {
	while ( decl->funcs.parent != 0 && (decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs ) ) != 0 ) {
	  entry = (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, (void *) name, -1);
          if (entry) {
	    return entry;
	  }
	}
   }
   return 0;
}


int is_scalar_var_type( AST_VAR_TYPE value_type )
{
   if (value_type == S_VAR_ANY) {
     return 0;
   }
   return value_type & (S_VAR_INT | S_VAR_DOUBLE | S_VAR_STRING );
}
 
int is_not_numeric_var_type( AST_VAR_TYPE value_type )
{
   if (value_type == S_VAR_ANY) {
     return 1;
   }
   return value_type & (S_VAR_INT | S_VAR_DOUBLE );
}
 
int CHECK_reference( AST_EXPRESSION *expr, PARSECONTEXT *ctx )
{
   const char *name = expr->val.ref.lhs;
   BINDING_ENTRY *entry;

   if (expr->ref.binding != 0) {
     return 0;
   }

   entry = lookup_binding( expr->val.ref.lhs, ctx->current );
   if (!entry) {
      if (ctx->chkctx.is_left_hand_side) { 
         entry->ref.binding = make_binding( expr->value_type, &expr->base.location, expr->val.ref.lhs );
	 return 0;
      }
      // a variable is used that is not yet defined.
      return -1;

   } else {
     entry->ref.binding = entry;
   }

   if (expr->value_type == S_VAR_CODE) {
      return 
   }
   
   if (expr->value_type == S_VAR_HASH) {
     if (entry) {
        if (entry->data.value_type  == S_VAR_ANY) {
	  entry->data.value_type = S_VAR_HASH;
	  entry->data.def_location = expr->base.location;
	  return 0;
	}
	if (entry->data.value_type != S_VAR_HASH) {
	  do_yyerror( expr->base.location, ctx, "The variable %s is used as a table but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( expr->value_type ),
	     entry->data.def_location.first_line,
	     entry->data.def_location.first_column );
	  return -1;
	}
      } 
   } else if (expr->value_type == S_VAR_LIST) {
     if (entry) {
        if (entry->value_type  == S_VAR_ANY) {
	  entry->data.value_type = S_VAR_LIST;
	  entry->data.def_location = expr->base.location;
	  return 0;
	}

	if (entry->value_type != S_VAR_HASH) {
	  do_yyerror( expr->base.location, ctx, "The variable %s is used as a array but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( expr->data.value_type ),
	     entry->data.def_location.first_line,
	     entry->data.def_location.first_column );
	  return -1;
	}
     }    
  } else if (is_scalar_var_type( expr->value_type ) ) {
     if (entry) {
        if (entry->data.value_type  == S_VAR_ANY) {
	  entry->data.value_type = expr->value_type;
	  entry->data.def_location = expr->base.location;
	  return 0;
	}

	if (!is_scalar_var_type( entry->data.value_type ) ) {
	  do_yyerror( expr->base.location, ctx, "The variable %s is used as scalar but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( expr->data.value_type ),
	     entry->data.def_location.first_line,
	     entry->data.def_location.first_column );
	  return -1;
	}
     }
   }
  return 0;
}

int CHECK_func_call( PARSECONTEXT *ctx, AST_EXPRESSION *expr )
{
    AST_FUNC_CALL *fcall;
    AST_FUNC_DECL *fdecl;
    AST_VECTOR *call_params;
    AST_EXPRESSION **call_expr;
    size_t i;

    fcall = expr->val.fcall;

    // check call expressions
    call_params = fcall->call_params;
    for( i = 0; i < ARRAY_size( &call_params->refs ); i++) {
	call_expr = (AST_EXPRESSION**) ARRAY_at( &call_params->refs, i );
	CHECK_expression( *call_expr, ctx );
    }

    if (fcall->func_decl == 0) {
      AST_EXPRESSION *fcall_lhs;

      fcall_lhs = fcall->f_name;

      if (fcall_lhs->exp_type == S_EXPR_LAMBDA) {
	fdecl = fcall_lhs->val.fdecl;
      } else if (fcall_lhs->exp_type == S_EXPR_REFERENCE &&  fcall_lhs->val.ref.indexes == 0) {
        // try to find function definition.
	fdecl = PARSECONTEXT_find_function_def( ctx, fcall_lhs.val.ref.lhs );
	if (!fdecl) {
	    // find if this is reference to Lambda expression.
	}
      }

      if (fdecl == 0) {
	expr->value_type = S_VAR_ANY;    
	return;
      }

      fcall_lhs->val.fdecl = fdecl;	    
    }


}

int CHECK_expression( AST_EXPRESSION *expr, PARSECONTEXT *ctx )
{
  switch( expr->exp_type ) {
   case S_EXPR_BINARY:
     return AST_EXPRESSION_binary_op_check_types( ctx, expr, );
   
   case S_EXPR_UNARY:
     return AST_EXPRESSION_unary_op_check_types( ctx, expr );
   
   case S_EXPR_FUNCALL:
     CHECK_func_call( ctx, expr );
     break;

   case S_EXPR_HASH_INDEX: {
     AST_EXPRESSION *check_expr = expr->val.index_expr; 
     CHECK_expression( check_expr, ctx );
     if (check_expr->value_type != S_VAR_ANY && ! is_numeric_or_string_type( check_expr->value_type ) ) {
       do_yyerror( check_expr->base.location, ctx, "Index expression of table must be integer or string value, instead is %s",
		get_type_name( check_expr->value_type ) );
     }
     }
     break;
   case S_EXPR_ARRAY_INDEX: {
     AST_EXPRESSION *check_expr = expr->val.index_expr; 
     CHECK_expression( check_expr, ctx );
     if (check_expr->value_type != S_VAR_ANY && ! is_numeric_type( check_expr->value_type ) ) {
       do_yyerror( check_expr->base.location, ctx, "Index expression of table must be integer, instead is %s",
		get_type_name( check_expr->value_type ) );
      }
     }
     break;
   case S_EXPR_LIST_VALUES: {
     AST_VECTOR *check_expr = (AST_VECTOR *) expr->val.index_expr; 
     AST_EXPRESSION **cexpr;
     size_t i;
     
     for( i=0; i< ARRAY_size( &check_expr->refs ); i++ ) {
	cexpr = (AST_EXPRESSION **) ARRAY_at( &check_expr->refs, i );
	CHECK_expression( *cexpr, ctx );
     }
     }
     break;
   case S_EXPR_HASH_VALUES: {
     AST_VECTOR *check_expr = (AST_VECTOR *) expr->val.index_expr; 
     AST_EXPRESSION **cexpr;
     AST_EXPRESSION *lhs,*rhs;
     size_t i;
     
     for( i=0; i< ARRAY_size( &check_expr->refs ); i++ ) {
	cexpr = (AST_EXPRESSION **) ARRAY_at( &check_expr->refs, i );
	lhs = (*cexpr)->val.expr.expr_left;
	rhs = (*cexpr)->val.expr.expr_right;

	CHECK_expression( rhs, ctx );
     	CHECK_expression( lhs, ctx );
        if (lhs->value_type != S_VAR_ANY && ! is_numeric_or_string_type( lhs->value_type ) ) {
          do_yyerror( lhs->base.location, ctx, "Index expression of table must be integer or string, instead is %s",
		get_type_name( lhs->value_type ) );
        }
     }
     }
     break;
   case S_EXPR_REFERENCE:
     CHECK_reference( expr, ctx );
     break;
   case S_EXPR_CONSTANT:
     break;
   case S_EXPR_ERROR:
     break;
   case S_EXPR_PLACEHOLDER:
     break;
  }   
  return 0;
}

int CHECK_fun_call( AST_FUNC_CALL *fcall, PARSECONTEXT *ctx )
{
  FUNCTION_HASH_entry *entry;
  size_t num_call_params, num_def_params;

  if (fcall->func_decl != 0) {
    // already resolved.
    return 0;
  }


  // check if function is defined.
  if (fcall->f_name) {
    entry = (FUNCTION_HASH_entry *) HASH_find( &ctx->map_function_defs, (void *) fcall->f_name, -1 );
    if (!entry && fcall->) {
      do_yyerror( &fcall->base.location, ctx, "The function %s is not defined, can't call an undefined function", fcall->f_name );
      return -1;
    }
  }

  fcall->func_decl = entry->decl;

  num_call_params = ARRAY_size( &fcall->call_params->refs ); 
  num_def_params = ARRAY_size( &entry->decl->func_params->refs );

  // check if number of parameters is the same as in prototype.
  if (num_call_params != num_def_params) {
	 do_yyerror( &fcall->base.location, ctx, "The function %s has %d parameters, wherease the function call specifies %d parameters",
			fcall->f_name, num_def_params,  num_call_params );
     return -1;
  }

  // check if parameter types are compatible
  // TODO

  return 0;
} 

int CHECK_statement_list( AST_BASE_LIST *body, PARSECONTEXT *ctx )
{
  DRING *tmp;
  AST_BASE *base;

  DRING_FOREACH( tmp, &body->statements )
    base = _OFFSETOF( tmp, AST_BASE, entry );
    switch(base->type) {
      case S_FUN_DECL: {
        AST_FUNC_DECL *prev,*fdecl;
	
	fdecl = (AST_FUNC_DECL *) base;
        
	prev = ctx->current_function;
	ctx->current_function  = fdecl;

        ctx->current_function->var_type = 0;

        CHECK_function_scope( fdecl,  ctx );


	ctx->current_function = prev;

	}
        break;

      case S_ASSIGNMENT: {
        AST_ASSIGNMENT *ass;
	AST_EXPRESSION *lhs;
	

	ass = (AST_ASSIGNMENT *) base;
        CHECK_expression( ass->right_side, ctx );

	
	lhs = (AST_EXPRESSION *) ass->left_side; 
	rhs = (AST_EXPRESSION *) ass->right_side;
    
	if (rhs->value_type != S_VAR_ANY) {

	    if (lhs->value_type == S_VAR_HASH || lhs->value_type == S_VAR_LIST) {
	       if (lhs->value_type != rhs->value_type) {
		    do_yyerror( &ass->right_side.base.location, ctx,
			"%s expected in assignment, instead there is the %s type in right hand side", 
			  get_type_name( right_type ),
			  get_type_name( lhs->value_type ) );
		  }
		  lhs->value_type = right_type;
  
		}
      	      }
	  } else {
	     if (right_type != S_VAR_ANY && right_type != S_VAR_LIST) {
		do_yyerror( &ass->right_side.base.location, ctx,
		    "List value expected in assignment, instead there is the %s type in right hand side", 
		      get_type_name( cond->condition.value_type )
	        );
	     }
	  }
	}

        ctx->chkctx.is_left_hand_side = 1; 
	CHECK_expression( ass->left_side, ctx );
        ctx->chkctx.is_left_hand_side = 0; 
	}
        break;
      case S_IF: {
        AST_COND *cond;
	int first_clause = 1;

        for( cond = (AST_COND *)base; cond; cond = cond->elsecond ) {
	  CHECK_expression( cond->condition,ctx );
 	  if ( is_not_numeric_var_type( cond->condition.value_type ) ) {
	    do_yyerror( &cond->condition.base.location, ctx,
	      "The condition of the %s clause does not evaluate to a numeric type, instead it evaluates to %s type", 
	      first_clause ? "if" : "elsif",
	      get_type_name( cond->condition.value_type )
	    );
	  }
	  first_clause = 0;
	
	  CHECK_statement_list( cond->block, ctx );
	}
	}
	break;
      case S_FOR: {
        AST_FOR_LOOP *loop;

        loop = (AST_FOR_LOOP *) base;
	CHECK_expression( loop->loop_expr, ctx );
        CHECK_statement_list( loop->block, ctx );
	
	}
        break;

      case S_WHILE: {
        AST_WHILE_LOOP *wloop;

	wloop = (AST_WHILE_LOOP *) base;

	CHECK_expression( wloop->condition, ctx );
	if ( ! is_not_numeric_var_type( wloop->condition.value_type ) ) {
	  do_yyerror( &wloop->condition.base.location, ctx,
	    "The condition of the while loop does not evaluate to a numeric type, instead it evaluates to %s type", 
	    get_type_name( wloop->condition.value_type )
	  );
	}

	CHECK_statement_list( wloop->block, ctx );
	}
        break; 

      case S_BREAK:
      case S_NEXT: {
	  AST_BASE *cur;
	  int nexted_in_loop = 0;

	  for( cur = base->parent; cur != 0; cur = cur->parent) {
	    if (cur->type == S_FOR || cur->type == S_WHILE) {
	      nested_in_loop = 1;
	      break;
	    }
	    if (cur->type == S_FUN_DECL) {
	      break;
	    }  
	  }
	  if (!nested_in_loop) {
	   do_yyerror( &base->location, ctx,
	    "The %s statement is not nested in a for or while loop",
	    base->type == S_NEXT ? "next" : "break";
	   );  
	  }
        }
	break;
      case S_FUN_CALL: {
        AST_FUNC_CALL *fcall = (AST_FUNC_CALL *) base;
	CHECK_fun_call(fcall, ctx);
	}
	break;
      case S_RETURN:
      case S_YIELD: {
        AST_RETURN *ret = (AST_RETURN *) base;
	if (! CHECK_expression( ret->rvalue, ctx )) {
	
	  if (ret->rvalue.value_type != S_VAR_TYPE_ANY) {
	    if (ctx->current_function->var_type == 0) {
	      ctx->current_function->var_type = ret->rvalue.value_type; 
	    } else if (ctx->current_function->var_type != ret->rvalue.value_type) {
	      // second return type differs from a prior return type
              ctx->current_function->var_type = S_VAR_TYPE_ANY;     
	    }
	  }
	}
	}
        break;
      default:
        ;
   }
   return 0;
}

int CHECK_function_scope( AST_FUNC_DECL *fdecl,  PARSECONTEXT *ctx )
{
  AST_BASE_LIST *body = fdecl->func_body;
  AST_EXPRESSION *expr;
  size_t i;

  ctx->chkctx.is_left_hand_side = 1; 
  for( i = 0; i < ARRAY_size( fdecl->func_params.refs ); i++ ) {
    expr = (AST_EXPRESSION *) AST_VECTOR_get( &fdecl->func_params, i );
    CHECK_expression( ass->left_side, 1 );
  }
  ctx->chkctx.is_left_hand_side = 0; 
  
  return CHECK_statement_list( body, ctx );
}

int CHECKER_run( CHECKERCTX *ctx, struct tagAST_BASE_LIST *program)
{
  PARSECTX *pctx = _OFFSETOF( ctx, PARSECTX, chkctx );
  
  return CHECK_statement_list( program, pctx );
}
