#include "parsectx.h"
#include "ast.h"
#include "parser.h"

int CHECK_expression( AST_EXPRESSION *expr, PARSECONTEXT *ctx );

int CHECKER_init( CHECKERCTX *ctx)
{  
  ctx->is_left_hand_side = 0; 
  ctx->current_function  = 0;
  return 0;
}

int CHECK_function_scope( AST_FUNC_DECL *fdecl,  PARSECONTEXT *ctx );

BINDING_ENTRY * make_binding( AST_VAR_TYPE value_type, YYLTYPE *location, const char *name, AST_FUNC_DECL *decl )
{
   BINDING_ENTRY *binding;

   binding = (BINDING_ENTRY *) malloc( sizeof( BINDING_ENTRY ) );
   if (!binding) {
     return 0;
   }

   binding->def_location = *location;
   binding->value_type = value_type;
   binding->binding_name = name;
   binding->stack_offset = ++decl->last_stack_offset ;
  
   return binding;
}

BINDING_ENTRY *lookup_binding( const char *name, AST_FUNC_DECL *decl, REF_SCOPE scope, int *pscope )
{  
   BINDING_ENTRY *rval;

   switch( scope ) {
     case  REF_SCOPE_LOCAL: 
       *pscope = 0;
       return  (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, (void *) name, -1);
     case  REF_SCOPE_GLOBAL: 
       while( decl->funcs.parent != 0 )
       {
	  decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs );
       }	
       *pscope = -1;
       return  (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, (void *) name, -1);

    case REF_SCOPE_CLOSURE:
       while( decl->funcs.parent != 0 ) {
	  
	  scope ++;
	  decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs );
	  
	  rval = (BINDING_ENTRY *)  HASH_find( &decl->scope_map_name_to_binding, (void *) name, -1);
	  if (rval) {
	    *pscope = decl->funcs.parent != 0 ? scope : -1;
	    return rval;
	  }
       } 
       return 0; 	
    default:
	return 0;
    }

      
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

   if (expr->val.ref.binding != 0) {
     return 0;
   }

   // can't find type of class members (at this stage)
   if (strcmp(name,"this") == 0) {
     return 0;
   }


   if (strcmp(name, "global") == 0) {
     entry = lookup_binding( name, ctx->current, REF_SCOPE_GLOBAL, &expr->val.ref.scope );
   } 
   else if (strcmp(name, "close") == 0) {
     // find local variable binding 
     entry = lookup_binding( name, ctx->current, REF_SCOPE_CLOSURE, &expr->val.ref.scope );

   } else {
     // find local variable binding 
     entry = lookup_binding( name, ctx->current, REF_SCOPE_LOCAL, &expr->val.ref.scope );
   }
   if (!entry) {
      if (expr->val.ref.scope != 0) {
         if (ctx->chkctx.is_left_hand_side) {
	    do_yyerror( &expr->base.location, ctx, "Can't define the new %s variable %s in a function",
		      expr->val.ref.scope == -1 ? "global" : "non-local (closure)", name );
	 } else {
	    do_yyerror( &expr->base.location, ctx, "Can't access undefined %s variable %s. can't reference undefined variables",
		      expr->val.ref.scope == -1 ? "global" : "non-local (closure)", name );

	 }
	 return -1;
      }
      if (!ctx->chkctx.is_left_hand_side) { 
         // variable name must be assigned a value before use.
         do_yyerror( &expr->base.location, ctx, "You must assign a value to variable %s before trying to use its value. can't reference undefined variables",
			name );
	 return -1;
      }	 
      expr->val.ref.binding = make_binding( expr->value_type, &expr->base.location, name, ctx->chkctx.current_function  );
      return 0;
   } else {
      expr->val.ref.binding = entry;
   }

   if (expr->value_type == S_VAR_CODE) {
      return 0;
   }
   
   if (expr->value_type == S_VAR_HASH) {
     if (entry) {
        if (entry->value_type  == S_VAR_ANY) {
	  entry->value_type = S_VAR_HASH;
	  entry->def_location = expr->base.location;
	  return 0;
	}
	if (entry->value_type != S_VAR_HASH) {
	  do_yyerror( &expr->base.location, ctx, "The variable %s is used as a table but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( entry->value_type ),
	     entry->def_location.first_line,
	     entry->def_location.first_column );
	  return -1;
	}
      } 
   } else if (expr->value_type == S_VAR_LIST) {
     if (entry) {
        if (entry->value_type  == S_VAR_ANY) {
	  entry->value_type = S_VAR_LIST;
	  entry->def_location = expr->base.location;
	  return 0;
	}

	if (entry->value_type != S_VAR_HASH) {
	  do_yyerror( &expr->base.location, ctx, "The variable %s is used as a array but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( entry->value_type ),
	     entry->def_location.first_line,
	     entry->def_location.first_column );
	  return -1;
	}
     }    
  } else if (is_scalar_var_type( expr->value_type ) ) {
     if (entry) {
        if (entry->value_type  == S_VAR_ANY) {
	  entry->value_type = expr->value_type;
	  entry->def_location = expr->base.location;
	  return 0;
	}

	if (!is_scalar_var_type( entry->value_type ) ) {
	  do_yyerror( &expr->base.location, ctx, "The variable %s is used as scalar but has previously been defined as a %s at line %d column %d",
	     expr->val.ref.lhs,
	     get_type_name( entry->value_type ),
	     entry->def_location.first_line,
	     entry->def_location.first_column );
	  return -1;
	}
     }
   }
  return 0;
}

int CHECK_func_call( PARSECONTEXT *ctx, AST_FUNC_CALL *fcall )
{
    
    AST_BASE *fdecl;
    AST_FUNC_DECL *ffdecl;
    AST_XFUNC_DECL *fxdecl;
    AST_VECTOR *call_params;
    AST_EXPRESSION **call_expr;
    size_t i;
    size_t num_call_params, num_def_params;


    //fcall = expr->val.fcall;

    // check function call expressions
    call_params = fcall->call_params;
    for( i = 0; i < ARRAY_size( &call_params->refs ); i++) {
	call_expr = (AST_EXPRESSION**) ARRAY_at( &call_params->refs, i );
	CHECK_expression( *call_expr, ctx );
    }

    if (fcall->func_decl == 0) {
      AST_EXPRESSION *fcall_lhs;

      fcall_lhs = fcall->f_name; // varRef

      CHECK_reference( fcall->f_name, ctx );


      if (fcall_lhs->value_type == S_VAR_CODE) {
	fdecl = fcall_lhs->val.fdecl;
      } else if (fcall_lhs->exp_type == S_EXPR_REFERENCE &&  fcall_lhs->val.ref.indexes == 0) {
	
	const char *function_name = fcall_lhs->val.ref.lhs;

	// if fcall_lhs is this reference, then we can't find the function. Check if this is thecase
        // can't find type of class members (at this stage)
	if (strcmp( function_name, "this" ) == 0) {
	  return 0;
	}

	fdecl = PARSECONTEXT_find_function_def( ctx, function_name );
	if (!fdecl) {
	  do_yyerror( &fcall->base.location, ctx, "The function %s is not defined, can't call undefined functions",
			function_name );
	  return -1;			
	}
      }

      fcall_lhs->val.fdecl = fdecl;	    
    }

    if (fdecl->type == S_FUN_DECL ) {
     
      ffdecl = (AST_FUNC_DECL *) fdecl;
      fxdecl = 0;

    } else {
      assert( fdecl->type == S_XFUN_DECL );

      fxdecl = (AST_XFUNC_DECL *) fdecl; 
      ffdecl = 0;
    }
   
    num_call_params = ARRAY_size( &fcall->call_params->refs ); 
    num_def_params = ffdecl ? ARRAY_size( &ffdecl->func_params->refs ) : fxdecl->nparams;

    // check if number of parameters is the same as in prototype.
    if (num_call_params != num_def_params) {
	 do_yyerror( &fcall->base.location, ctx, "The function %s has %d parameters, wherease the function call has %d parameters",
			fcall->f_name, num_def_params,  num_call_params );
         return -1;
      }


      // check if parameter types agree
      for( i = 0; i < num_call_params; i++) {
	AST_EXPRESSION *call_p = (AST_EXPRESSION *) ARRAY_at( &fcall->call_params->refs, i );
	AST_EXPRESSION *fdef_p = (AST_EXPRESSION *) ARRAY_at( &ffdecl->func_params->refs, i );
	
	if (call_p->value_type != S_VAR_ANY && fdef_p->value_type != S_VAR_ANY) {
	    int is_ok = 0;

	    if (is_scalar_var_type( call_p->value_type ) && is_scalar_var_type( fdef_p->value_type )) {    
		is_ok = 1;
	    } else {
		is_ok = call_p->value_type == fdef_p->value_type;
	    }

	    if (!is_ok) {
		do_yyerror( &fcall->base.location, ctx, "The function call parameter #%d is of type %s the function parameter is of type %s",
				    get_type_name( call_p->value_type ),
				    get_type_name( fdef_p->value_type ) );
        		
	    }
	}
      }
    return 0;
}

int CHECK_expression( AST_EXPRESSION *expr, PARSECONTEXT *ctx )
{
  switch( expr->exp_type ) {
   case S_EXPR_BINARY:
     CHECK_expression( expr->val.expr.expr_left, ctx ); 
     CHECK_expression( expr->val.expr.expr_right, ctx ); 
       
     return AST_EXPRESSION_binary_op_check_types( ctx, expr );
   
   case S_EXPR_UNARY:
     CHECK_expression( expr->val.unary.expr, ctx ); 
     return AST_EXPRESSION_unary_op_check_types( ctx, expr );
   
   case S_EXPR_FUNCALL:
     CHECK_func_call( ctx,  expr->val.fcall );
     break;

   case S_EXPR_HASH_INDEX: {
     AST_EXPRESSION *check_expr = expr->val.index_expr; 
     CHECK_expression( check_expr, ctx );
     if (check_expr->value_type != S_VAR_ANY && ! is_numeric_or_string_type( check_expr->value_type ) ) {
       do_yyerror( &check_expr->base.location, ctx, "Index expression of table must be integer or string value, instead is %s",
		get_type_name( check_expr->value_type ) );
     }
     }
     break;
   case S_EXPR_ARRAY_INDEX: {
     AST_EXPRESSION *check_expr = expr->val.index_expr; 
     CHECK_expression( check_expr, ctx );
     if (check_expr->value_type != S_VAR_ANY && ! is_numeric_type( check_expr->value_type ) ) {
       do_yyerror( &check_expr->base.location, ctx, "Index expression of table must be integer, instead is %s",
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
          do_yyerror( &lhs->base.location, ctx, "Index expression of table must be integer or string, instead is %s",
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
   case S_EXPR_LAMBDA:
     break;
  }   
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
	prev = ctx->chkctx.current_function;
	ctx->chkctx.current_function  = fdecl;


        CHECK_function_scope( fdecl,  ctx );

	ctx->chkctx.current_function = prev;

	}
        break;

      case S_ASSIGNMENT: {
        AST_ASSIGNMENT *ass;
	AST_EXPRESSION *lhs,*rhs;
	

	ass = (AST_ASSIGNMENT *) base;
        CHECK_expression( ass->right_side, ctx );

        ctx->chkctx.is_left_hand_side = 1; 
	CHECK_expression( ass->left_side, ctx );
        ctx->chkctx.is_left_hand_side = 0; 
	
	lhs = (AST_EXPRESSION *) ass->left_side; 
	rhs = (AST_EXPRESSION *) ass->right_side;
    
	if (rhs->value_type != S_VAR_ANY) {

	    if (lhs->value_type == S_VAR_HASH || lhs->value_type == S_VAR_LIST) {
	        if (lhs->value_type != rhs->value_type) {
		    do_yyerror( &ass->right_side->base.location, ctx,
			"%s expected in assignment, instead there is the %s type in right hand side", 
			  get_type_name( rhs->value_type ),
			  get_type_name( lhs->value_type ) );
		}
		lhs->value_type = rhs->value_type;
      	      }
	  } else {
	     if (rhs->value_type != S_VAR_ANY && rhs->value_type != S_VAR_LIST) {
		do_yyerror( &ass->right_side->base.location, ctx,
		    "List value expected in assignment, instead there is the %s type in right hand side", 
		      get_type_name( rhs->value_type )
	        );
	     }
	  }
	}
        break;
      case S_IF: {
        AST_COND *cond;
	int first_clause = 1;

        for( cond = (AST_COND *)base; cond; cond = cond->elsecond ) {
	  CHECK_expression( cond->condition,ctx );
 	  if ( is_not_numeric_var_type( cond->condition->value_type ) ) {
	    do_yyerror( &cond->condition->base.location, ctx,
	      "The condition of the %s clause does not evaluate to a numeric type, instead it evaluates to %s type", 
	      first_clause ? "if" : "elsif",
	      get_type_name( cond->condition->value_type )
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
	if ( ! is_not_numeric_var_type( wloop->condition->value_type ) ) {
	  do_yyerror( &wloop->condition->base.location, ctx,
	    "The condition of the while loop does not evaluate to a numeric type, instead it evaluates to %s type", 
	    get_type_name( wloop->condition->value_type )
	  );
	}

	CHECK_statement_list( wloop->block, ctx );
	}
        break; 

      case S_BREAK:
      case S_NEXT: {
	  AST_BASE *cur;
	  int nested_in_loop = 0;

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
	    base->type == S_NEXT ? "next" : "break"
	   );  
	  }
        }
	break;
      case S_FUN_CALL: {
        AST_FUNC_CALL *fcall = (AST_FUNC_CALL *) base;
	CHECK_func_call(ctx, fcall);
	}
	break;
      case S_RETURN: {
        AST_RETURN *ret = (AST_RETURN *) base;
	if (! CHECK_expression( ret->rvalue, ctx )) {
	  if (ret->rvalue->value_type != S_VAR_ANY) {
	    if (ctx->chkctx.current_function->return_type_value == 0) {
	      ctx->chkctx.current_function->return_type_value = ret->rvalue->value_type; 
	    } else if (ctx->chkctx.current_function->return_type_value != ret->rvalue->value_type) {
	      // second return type differs from a prior return type
              ctx->chkctx.current_function->return_type_value = S_VAR_ANY;     
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
  for( i = 0; i < ARRAY_size( &fdecl->func_params->refs ); i++ ) {
    expr = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
    CHECK_expression( expr, ctx );
  }
  ctx->chkctx.is_left_hand_side = 0; 
  
  return CHECK_statement_list( body, ctx );
}

int CHECKER_run( CHECKERCTX *ctx, struct tagAST_BASE_LIST *program)
{
  PARSECONTEXT *pctx = _OFFSETOF( ctx, PARSECONTEXT, chkctx );
  
  return CHECK_statement_list( program, pctx );
}

