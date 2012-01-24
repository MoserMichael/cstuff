#include "parsectx.h"
#include "ast.h"
#include "parser.h"


int CHECKER_init( CHECKERCTX *ctx)
{  
  ctx->is_left_hand_side = 0; 
}

int CHECK_function_scope( AST_FUNC_DECL *fdecl,  PARSECONTEXT *ctx );

int make_binding( AST_VALUE_TYPE value_type, YYLOCATION *location, const char *name )
{
   BINDING_ENTRY *binding;


   binding = (BINDING_ENTRY *) malloc( sizeof( BINDING_ENTRY ) );
   if (!binding) {
     return -1;
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
        return -1;
      }
      break;
    case S_VAR_LIST:
      if (ARRAY_init( &binding->data.value.array_value, 0, sizeof( BINDING_DATA ) ) ) {
        return -1;
      }
      break;
   }
   return 0;
}

BINDING_HASH_entry *lookup_binding( const char *name, AST_FUNC_DECL *decl )
{
   BINDING_ENTRY *binding; 
    
   if (name[0] != '#') {
	 return  (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, name, -1);
   } else {
	while ( (decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs ) ) != 0 ) {
	  entry = (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, name, -1);
          if (entry) {
	    return entry;
	  }
	}
   }
   return 0;
}
int is_scalar_var_type( AST_VAR_TYPE value_type )
{
   if (value_type == VAR_ANY) {
     return 0;
   }
   return value_type & (VAR_INT | VAR_DOUBLE | VAR_STRING ) != 0;
}
 
int is_not_numeric_var_type( AST_VAR_TYPE value_type )
{
   if (value_type == VAR_ANY) {
     return 1;
   }
   return value_type & (VAR_INT | VAR_DOUBLE ) == 0;
}
 
int CHECK_reference( AST_EXPRESSION *expr, PARSECONTEXT *ctx )
{
   const char *name = expr->val.ref.lhs;
   BINDING_ENTRY *entry;

   entry = lookup_binding( expr->val.ref.lhs, ctx->current );

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
   if (ctx->chkctx.is_left_hand_side) { 
     if (expr->exp_type != S_EXPR_LIST_VALUES) {
       // single value assignment
       make_binding( expr->value_type, &expr->base.location, expr->val.ref.lhs );
     } else {
       // multi value assignment
     }

   }
   return 0;
}

int CHECK_expression( AST_EXPRESSION *expr, PARSECONTEXT *ctx )
{
  switch( expr->exp_type ) {
   case S_EXPR_BINARY:
     return AST_EXPRESSION_binary_op_check_types( ctx, expr, );
   
   case S_EXPR_UNARY:
     return AST_EXPRESSION_unary_op_check_types( ctx, expr );
   
   case S_EXPR_FUNCALL:
     //CHECK_fun_call( expr->val.fcall, ctx );
     break;
   case S_EXPR_HASH_INDEX:
     break;
   case S_EXPR_ARRAY_INDEX:
     break;
   case S_EXPR_LIST_VALUES:
     break;
   case S_EXPR_HASH_VALUES:
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
      case S_FUN_DECL:
        CHECK_function_scope( (AST_FUNC_DECL *) tmp,  ctx );
        break;

      case S_ASSIGNMENT: {
        AST_ASSIGNMENT *ass;

	ass = (AST_ASSIGNMENT *) tmp;
        if (!CHECK_expression( ass->right_side, ctx )) {
	  AST_VAR_TYPE right_type =  ass->right_side->value_type; 


	  if (ass->left_side.type == S_EXPRESSION) {
	      AST_EXPRESSION *lhs = (AST_EXPRESSION *) ass->left_side; 
	      
	      if (right_type != S_VAR_ANY) {
		if (lhs->value_type == S_VAR_HASH || lhs->value_type == S_VAR_LIST) {
		  if (lhs->value_type != right_type) {
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

	}
        break;
      case S_IF: {
        AST_COND *cond;
	int first_clause = 1;

        for( cond = (AST_COND *)tmp; cond; cond = cond->elsecond ) {
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

        loop = (AST_FOR_LOOP *) tmp;
	CHECK_expression( loop->loop_expr, ctx );
        CHECK_statement_list( loop->block, ctx );
	
	}
        break;

      case S_WHILE: {
        AST_WHILE_LOOP *wloop;

	wloop = (AST_WHILE_LOOP *) tmp;

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
      case S_FUN_CALL: {
        AST_FUNC_CALL *fcall = (AST_FUNC_CALL *) tmp;
	CHECK_fun_call(fcall, ctx);
	}
	break;
      case S_RETURN:
      case S_YIELD: {
        AST_RETURN *ret = (AST_RETURN *) tmp;
	CHECK_expression( ret->rvalue, ctx );
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
