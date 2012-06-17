#include "parsectx.h"
#include "ast.h"
#include "parser.h"

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

BINDING_ENTRY *find_binding( AST_FUNC_DECL *fdecl, const char *name )
{
  return  (BINDING_ENTRY *)  HASH_find( &fdecl->scope_map_name_to_binding, (void *) name, -1);
}

#if 0

int CHECK_statement_list( AST_BASE_LIST *body, PARSECONTEXT *ctx );
int CHECK_expression( AST_EXPRESSION *expr, PARSECONTEXT *ctx );


int CHECK_function_scope( AST_FUNC_DECL *fdecl,  PARSECONTEXT *ctx );

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

int CHECK_statement( AST_BASE *base, PARSECONTEXT *ctx )
{
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

int CHECK_statement_list( AST_BASE_LIST *body, PARSECONTEXT *ctx )
{
  DRING *tmp;
  AST_BASE *base;

  DRING_FOREACH( tmp, &body->statements ) {
    base = _OFFSETOF( tmp, AST_BASE, entry );
    CHECK_statement( base, ctx );
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
#endif

typedef struct tagFUNCTION_ENTRY {
  HASH_Entry entry;
  AST_FUNC_DECL *fdecl;

} FUNCTION_ENTRY;



M_INLINE int function_hash_compare( HASH_Entry  *entry, void * key, ssize_t key_length)
{
  FUNCTION_ENTRY *lhs;	

  (void) key_length;

  lhs = (FUNCTION_ENTRY *) entry;

  if (strcmp( lhs->fdecl->f_name, key) == 0) {
    return 0;
  }
  return 1;
}

#if 0
AST_FUNC_DECL * CHECKER_find_function( CHECKERCTX *ctx, const char *name )
{
   FUNCTION_ENTRY *entry = HASH_find( &ctx->scope_map_name_to_function, name, (size_t) -1);  
   if (!entry) {
     return 0;
   }
   return entry->fdecl;
}

int CHECKER_add_function( CHECKERCTX *ctx,  FUNC_DECL *decl )
{
  FUNCTION_ENTRY *func;
  
  func = (FUNCTION_ENTRY *) malloc( sizeof( FUNCTION_ENTRY ) );
  if (!func) {
    return -1;
  }
  func->fdecl = decl;

  return HASH_insert( &ctx->scope_map_name_to_function, &func->entry, name, (size_t) -1);
}
 
int CHECKER_define_function( CHECKERCTX *ctx, AST_FUNC_DECL *fdecl )
{
  AST_FUNC_DECL *fcheck;
	  
  fcheck = CHECKER_find_function( ctx, fdecl->f_name );
  if (fcheck) {
    do_yyerror( &fdecl->base.location, "Function %s is defined twice", fdecl->f_name );
    do_yyerror( &fcheck->base.location, "First definition of %s is here", fdecl->f_name );
    return -1;
  }
  if (CHECKER_add_function( ctx, fdec )) {
    do_yyerror( &fcheck->base.location, "Internal error: can't add function declaration" );
    return -1;
  }
  return 0;
}
#endif


int CHECKER_init( CHECKERCTX *ctx)
{  
  ctx->is_left_hand_side = 0; 
  ctx->current_function  = 0;

#if 0
  if (HASH_init( &ctx-> scope_map_name_to_function, 10, 0, function_hash_compare, 0 )) {
    return -1;
  }
#endif  
  return 0;
}

void  CHECKER_pass( PARSECONTEXT *ctx, AST_BASE *base );

AST_FUNC_CALL_PARAM *CHECKER_find_param_by_label( AST_VECTOR *vect, const char *param_name )
{
    size_t i;
    AST_FUNC_CALL_PARAM *param;
    
    for( i = 0; i < AST_VECTOR_size( vect ); i++ )
    {
	param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( vect, i );
	if (strcmp( param->label_name, param_name ) == 0) {
	    return param;
	}
    }
    return 0;
}

AST_EXPRESSION *CHECKER_find_param_by_label2( AST_VECTOR *vect, const char *param_name )
{
    size_t i;
    AST_EXPRESSION *param;
    
    for( i = 0; i < AST_VECTOR_size( vect ); i++ )
    {
	param = (AST_EXPRESSION *) AST_VECTOR_get( vect, i );
	if (strcmp( param->val.ref.lhs, param_name ) == 0) {
	    return param;
	}
    }
    return 0;
}

AST_XFUNC_PARAM_DECL *CHECKER_xfind_param_by_label( AST_XFUNC_DECL *xfunc , const char *param_name )
{
    size_t i;
    AST_XFUNC_PARAM_DECL *param;
    
    for( i = 0; i < MAX_XFUNC_PARAM; i++ ) {
	param = &xfunc->params[ i ];

	if (!param->param_name) {
	  break;
	}
	if (strcmp( param->param_name, param_name ) == 0) {
	    return param;
	}
    }
    return 0;
}


int  CHECKER_check_func_call( PARSECONTEXT *ctx, AST_FUNC_CALL *fcall )
{
    AST_EXPRESSION *f_name;
    const char *func_name;
    AST_BASE *func_def; 
    size_t i;
    AST_FUNC_CALL_PARAM *param;
    f_name =  fcall->f_name;

    CHECKER_pass( ctx, (AST_BASE *) fcall->f_name );
    //fprintf( out, " , " );
    CHECKER_pass( ctx, (AST_BASE *) fcall->call_params );

    if ( f_name->exp_type != S_EXPR_REFERENCE )
    {
       // function that returns function value - can't check during compilation / type checking.
       return 0;
    }

    func_name = f_name->val.ref.lhs;  

    func_def = PARSECONTEXT_find_function_def( ctx, func_name ); 
    if (!func_def) {
      do_yyerror( &fcall->base.location, ctx, "The function %s is not defined, can't call a function that is not defined", func_name );
      return -1;
    }

    fcall->func_decl = func_def;

    if (func_def->type == S_FUN_DECL) {
       AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) func_def;
       AST_EXPRESSION *parame;

       for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	 param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
	 if (CHECKER_find_param_by_label2( fdecl->func_params, param->label_name ) == 0) {
           do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s", fdecl->f_name, param->label_name );
	 }
       }	 
       for( i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
	 parame = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
	 if (CHECKER_find_param_by_label( fcall->call_params, parame->val.ref.lhs ) == 0) {
           do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the mandatory parameter ~%s", parame->val.ref.lhs );
	 }
       }	 
    } else {
       AST_XFUNC_DECL *xfunc = (AST_XFUNC_DECL *) func_def;
       AST_XFUNC_PARAM_DECL *xparam;
       
       for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	 param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
  	
	 if (CHECKER_xfind_param_by_label( xfunc, param->label_name ) == 0) {
           do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s",  xfunc->f_name, param->label_name );
	 }
       }
     
       for( i = 0; i < MAX_XFUNC_PARAM; i++ ) {
 	 xparam = &xfunc->params[ i ];
	 if (!xparam->param_name) {
	   break;
	 }
	 if (CHECKER_find_param_by_label( fcall->call_params, xparam->param_name ) == 0) {
	       do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the mandatory parameter ~%s", xparam->param_name );
	 }
       }
    }
    
    return 0;
}



void  CHECKER_pass( PARSECONTEXT *ctx, AST_BASE *base )
{
  size_t i;

  assert(base );

  ////fprintf( out, "{%d,%d-%d,%d} ", base->location.first_line, base->location.first_column, base->location.last_line, base->location.last_column );
  ////fprintf( out, "(" );
  
  switch( base->type ) {
  case S_EXPRESSION:
    //CHECKER_pass_expr( out, (AST_EXPRESSION *) base );
    break;

  case S_AST_LIST: {
    AST_BASE_LIST *scl  = (AST_BASE_LIST *) base;
    DRING *cur;

    ////fprintf( out, "stmt-list" );

    DRING_FOREACH( cur, &scl->statements ) {
	////fprintf(out, " , " );
	CHECKER_pass( ctx,  _OFFSETOF( cur, AST_BASE, entry ) );
    }
    }
    break;

  case S_AST_VECTOR: {
    AST_VECTOR *vect = (AST_VECTOR *)base;

    ////fprintf( out, "stmt-vector" );

    for( i = 0; i < AST_VECTOR_size( vect ); i++) {
	////fprintf(out, " , " );
	CHECKER_pass( ctx, AST_VECTOR_get( vect, i ));
    }
    }
    break;
  
  case S_ASSIGNMENT:
  {
    AST_ASSIGNMENT *scl = (AST_ASSIGNMENT *) base;
    ////fprintf( out, "%s , ", scl->type == ASSIGNMENT_DEEP_COPY ? "assign-copy" : "assign-ref" );
    CHECKER_pass( ctx, (AST_BASE *) scl->left_side );
    ////fprintf( out, " , ");
    CHECKER_pass( ctx, (AST_BASE *) scl->right_side );
    //newline = 1;
    }
    break;
  
  case S_IF: {
    AST_COND *cond = (AST_COND *) base;

    if (cond->condition) {  
      ////fprintf( out, "cond-IF , " );
      CHECKER_pass( ctx, (AST_BASE *) cond->condition );
      ////fprintf( out, " , " );
      CHECKER_pass( ctx, (AST_BASE *) cond->block );
      ////fprintf( out, " , " );
   } else {
      //assert( cond->block );
      CHECKER_pass( ctx, (AST_BASE *) cond->block );
      //assert( cond->elsecond == 0 ); 
   }
   if (cond->elsecond ) {
      ////fprintf( out , "else , " );
      CHECKER_pass( ctx, (AST_BASE *) cond->elsecond  );
    } 
    //newline = 1;
    }
    break;
 
  case S_WHILE: {
    AST_WHILE_LOOP *scl = (AST_WHILE_LOOP *) base;
#if 0
    const char *lp = 0;

    switch( scl->type ) {
       case  LOOP_POSTCOND_WHILE:
         lp = "loop-precondition-while";
         break;
       case LOOP_PRECOND_WHILE:
         lp = "loop-postcondition-while";
         break;
       case LOOP_INFINITE:
         lp = "loop-inifinite";
         break;
    }
    //fprintf( out, "%s , ", lp);
#endif

    if (scl->condition) {
      CHECKER_pass( ctx, (AST_BASE *) scl->condition );
      //fprintf( out, " , ");
    }

    CHECKER_pass( ctx, (AST_BASE *) scl->block );
    }
    break;

  case S_FOR:  {
    AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
    //fprintf(out,"for , ");
    CHECKER_pass( ctx, (AST_BASE *) scl->loop_var );
    //fprintf( out, " , ");
    CHECKER_pass( ctx, (AST_BASE *) scl->loop_expr );
    //fprintf( out, " , ");
    CHECKER_pass( ctx, (AST_BASE *) scl->block );
    }
    break;
  
  case S_FUN_CALL_PARAM: {
    AST_FUNC_CALL_PARAM *scl = (AST_FUNC_CALL_PARAM *) base;
    //fprintf( out, "func-param, %s , ", scl->label_name );
    CHECKER_pass( ctx, (AST_BASE *) scl->expr );
    }
    break;

  case S_FUN_CALL: {
    AST_FUNC_CALL *scl = (AST_FUNC_CALL *) base;
    //fprintf( out, "fun-call, " );
    
    CHECKER_check_func_call( ctx, scl );
    }
    break;

  case S_FUN_DECL: {
    AST_FUNC_DECL *scl = (AST_FUNC_DECL *) base;
    //fprintf( out, "fun-decl , %s , ", scl->f_name );
    if (scl->func_params != 0) {
      CHECKER_pass ( ctx, (AST_BASE *) scl->func_params );
      //fprintf( out, " ,\n" );
    }
    CHECKER_pass ( ctx, (AST_BASE *) scl->func_body );
    }
    break;
  
  case S_XFUN_DECL: {
    AST_XFUNC_DECL *scl = (AST_XFUNC_DECL *) base;

    //fprintf( out, "xfun-decl , %s , " , scl->f_name );

    for(i = 0; i < scl->nparams; i++) {
	//fprintf(out, "( func-param , %s ) ", scl->params[ i ] . param_name );
    }
    }
    break;
  
  case S_RETURN: {
    AST_RETURN *scl = (AST_RETURN *) base;
    //fprintf( out, "return" );
    if (scl->rvalue) {
      //fprintf( out, " , " );
      CHECKER_pass( ctx, (AST_BASE *) scl->rvalue );
    }
    }
    break;

  case S_NEXT:
    ////fprintf( out, "next");
    break;
  
  case S_BREAK:
    ////fprintf( out, "break" );
    break;

  //default:
    //fflush(out);
    //assert(0);
  }
  
  ////fprintf( out, ")" );
  //if (newline) {
  //  //fprintf( out, "\n" );
  //}
   
}

int CHECKER_run( CHECKERCTX *ctx, struct tagAST_BASE *program)
{
  PARSECONTEXT *pctx = _OFFSETOF( ctx, PARSECONTEXT, chkctx );
  AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) program;

  assert(program->type == S_FUN_DECL); 

  pctx->chkctx.global_scope = fdecl;

  CHECKER_pass( pctx, &fdecl->func_body->base );

//CHECK_first_pass( ctx, (AST_BASE_LIST *) program );
  return pctx->my_yy_is_error;
}

#if 0    
void CHECK_first_pass( CHECKERCTX *ctx, AST_BASE_LIST *slist );
{
  DRING *tmp;
  AST_BASE *base;

  DRING_FOREACH( tmp, &body->statements ) {
    base = _OFFSETOF( tmp, AST_BASE, entry );
  
    switch( base->type ) 
    {
      case S_ASSIGNMENT:
	{
	}
	break;
      case S_IF:
        { 
	   AST_COND *cond = (AST_COND *) base;
	   for(;;) {
	     if (cond->block) {
	       if (CHECK_first_pass( ctx, &cond->block )) {
	         return -1;
	       }
	     }
	     if (cond->elsecond == 0) {
	       break;
	     }
	     cond = cond->elsecond;
	   }
	}
	break;
      case S_WHILE:
        {
	   AST_WHILE_LOOP *scl = (AST_WHILE_LOOP *) base;
	   if (scl->block) {
	     CHECK_first_pass( ctx, scl->block);
	   }
	}
	break;
      case S_FOR:
	{
	   AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
	   if (scl->block) {
	     CHECK_first_pass( ctx, scl->block);
	   }
	}
	break;
 
      case S_FUNC_DECL:
        {
	  AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) base;
	  AST_FUNC_DECL *fcheck;
	  
	  fcheck = CHECKER_find_function( ctx, fdecl->f_name );
	  if (fcheck) {
	    do_yyerror( &fdecl->base.location, "Function %s is defined twice", fdecl->f_name );
	    do_yyerror( &fcheck->base.location, "First definition of %s is here", fdecl->f_name );
	    return -1;
	  }
          if (CHECKER_add_function( ctx, fdec )) {
	    do_yyerror( &fcheck->base.location, "Internal error: can't add function declaration" );
	    return -1;
	  }
	}
        break;
      default:

  }
  return 0;
}
#endif     
