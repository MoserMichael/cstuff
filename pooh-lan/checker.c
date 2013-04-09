#include "parsectx.h"
#include "ast.h"
#include "parser.h"

int can_assign( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type, AST_VAR_TYPE *offending_type);


BINDING_ENTRY *find_binding( AST_FUNC_DECL *fdecl, const char *name )
{
  //fprintf( stderr, ">>lookup %s %p\n", name, fdecl );

  return  (BINDING_ENTRY *)  HASH_find( &fdecl->scope_map_name_to_binding, (void *) name, -1);
}

BINDING_ENTRY * make_binding2( PARSECONTEXT *ctx, AST_FUNC_DECL *decl, AST_VAR_TYPE value_type, YYLTYPE *location, const char *name, int has_value, REF_SCOPE scope  )
{
   BINDING_ENTRY *binding;
   int add_offset = 0;
 
   binding = (BINDING_ENTRY *) malloc( sizeof( BINDING_ENTRY ) );
   if (!binding) {
     return 0;
   }

   if (scope == REF_SCOPE_GLOBAL && decl->base.parent == 0)
	add_offset = 1; // global heap - start of heap has some fixed values (const NULL, etc )


   if (location)
	binding->def_location = *location;
   else 
        memset( &binding->def_location, 0, sizeof( YYLTYPE ) );
   
   binding->value_type = value_type;
   binding->has_value = has_value;
   binding->binding_name = name;
   binding->stack_offset = add_offset + ( ++decl->last_stack_offset );
   binding->scope = scope;
   binding->source = 0;
   binding->scope_nesting = -1;
   //fprintf( stderr,">>add %s %p\n", name, decl );

   if (HASH_insert( &decl->scope_map_name_to_binding, &binding->entry, (void *)  name, -1  )) {
      BINDING_ENTRY * old_b = find_binding( decl, name );
      assert(old_b);
      if (old_b) {
          do_yyerror( location, ctx, "%s is already defined. ", name );
	  do_yyerror( &old_b->def_location, ctx, "First definition of %s is here", name );
      }
      
      assert(0);
   }	

   return binding;
}

BINDING_ENTRY * make_binding( PARSECONTEXT *ctx, AST_VAR_TYPE value_type, YYLTYPE *location, const char *name, int has_value, REF_SCOPE scope  )
{
   AST_FUNC_DECL *decl;
   
   assert( ctx->chkctx.current_function != 0 );
   decl = ctx->chkctx.current_function;

   if (scope == REF_SCOPE_LOCAL && decl->base.parent == 0) {
     scope = REF_SCOPE_GLOBAL;
   }
   return make_binding2( ctx, decl, value_type, location, name, has_value, scope  );
 
}

BINDING_ENTRY *lookup_binding( PARSECONTEXT *ctx, const char *name,  REF_SCOPE scope, int *rscope )
{  
   BINDING_ENTRY *rval;
   int nsc;
   AST_FUNC_DECL *decl; 

   assert( ctx->chkctx.current_function != 0 );
   decl = ctx->chkctx.current_function;

   switch( scope ) {
     case  REF_SCOPE_LOCAL: 
       *rscope = 0;
       return  find_binding( decl, name );
     case  REF_SCOPE_GLOBAL: 
       while( decl->funcs.parent != 0 ) {
	  decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs );
       }	
       *rscope = -1;
       return  find_binding( decl, name );

    case REF_SCOPE_CLOSURE:
       nsc = 0;
       while( decl->funcs.parent != 0 ) {
	  
	  decl = _OFFSETOF( decl->funcs.parent,  AST_FUNC_DECL, funcs );
	 
	  rval = find_binding( decl, name );
	  if (rval) {
	    *rscope = nsc;
	    return rval;
	  }
	  nsc ++;
       } 
       return 0; 	
    
    default:
	return 0;
    }
}



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


int CHECKER_init( CHECKERCTX *ctx)
{  
  ctx->is_left_hand_side = 0; 
  ctx->current_function  = 0;
  ctx->is_function_name = 0;
  ctx->pass = -1;

#if 0
  if (HASH_init( &ctx-> scope_map_name_to_function, 10, 0, function_hash_compare, 0 )) {
    return -1;
  }
#endif  
  return 0;
}

void  CHECKER_pass( PARSECONTEXT *ctx, AST_BASE *base );

#if 0
const char *get_var_name( AST_EXPRESSION *expr, REF_SCOPE *scope   )
{
  AST_VECTOR *indexes;
  AST_EXPRESSION *idx;

  if (strcmp(expr->val.ref.lhs,"this") == 0) {
    *scope = REF_SCOPE_THIS;
  } else if (strcmp(expr->val.ref.lhs,"global") == 0) {
    *scope = REF_SCOPE_GLOBAL;  
  } else if (strcmp(expr->val.ref.lhs,"outer") == 0) {
    *scope = REF_SCOPE_CLOSURE;
  } else {
    *scope = REF_SCOPE_LOCAL;
    return expr->val.ref.lhs;
  }

  indexes = expr->val.ref.indexes; 

  if (*scope == REF_SCOPE_THIS && indexes == 0) {
    return 0;
  }
  assert( indexes != 0 );
  idx = (AST_EXPRESSION *) AST_VECTOR_get( indexes, 0 );
  assert( idx->base.type == S_EXPRESSION && idx->exp_type == S_EXPR_HASH_INDEX );
  idx = idx->val.index_expr;
  assert( idx && idx->base.type == S_EXPRESSION && idx->exp_type == S_EXPR_CONSTANT );
 
  return idx->val.const_value.string_value; 
}
#endif

#if  0
AST_EXPRESSION *get_first_index( AST_EXPRESSION *expr   )
{
  AST_VECTOR *indexes;
  int nidx;

  nidx = 0;
  if (strcmp(expr->val.ref.lhs,"this") == 0 ||
      strcmp(expr->val.ref.lhs,"global") == 0 ||
      strcmp(expr->val.ref.lhs,"outer") == 0) {
    nidx = 1;
  }

  indexes = expr->val.ref.indexes;
  if (!indexes ) {
    return 0;
  }
  return (AST_EXPRESSION *) AST_VECTOR_get( indexes, nidx );
}
#endif

int is_indexed_ref( AST_EXPRESSION *expr )
{
  AST_VECTOR *indexes;
#if 0  
  size_t nidx;
#endif

  if (expr->exp_type != S_EXPR_REFERENCE) {
    return 0; 
  }
  
  indexes = expr->val.ref.indexes;
  if (!indexes ) {
    return 0;
  }
 #if 0
  nidx = 0;
  if (strcmp(expr->val.ref.lhs,"this") == 0 ||
      strcmp(expr->val.ref.lhs,"global") == 0 ||
      strcmp(expr->val.ref.lhs,"outer") == 0) {
    nidx = 1;
  }

 return AST_VECTOR_size( indexes ) > nidx;
#endif  
  return AST_VECTOR_size( indexes ) > 0;
}

const char *get_func_name( AST_BASE *func_def)
{
  const char *rval;

  if (func_def->type == S_FUN_DECL) {
    rval = ((AST_FUNC_DECL *) func_def)->f_name;
  } else {
    rval = ((AST_XFUNC_DECL *) func_def)->f_name;
  }
  if (rval == 0) {
    return "Anonymous function";
  }
  return rval;
}

void assign_value( AST_EXPRESSION *lhs, AST_VAR_TYPE value_type, YYLTYPE *location )
{
    BINDING_ENTRY *binding;

    if (lhs->exp_type == S_EXPR_CONSTANT) {
      return;
    }

    if ( is_indexed_ref( lhs ) && AST_EXPRESSION_type( lhs ) == S_VAR_ANY ) {
      return;
    }  

    binding = lhs->val.ref.binding;
    if (binding) { // assigning values to a parameter (for the first time).
	binding->value_type = value_type;
        if (location) {
	 binding->def_location = *location;
        }
        if (binding->has_value == 0) {
	      binding->has_value = 1;
        }      
    }
    lhs->value_type = value_type;
} 


void CHECKER_expr( PARSECONTEXT *ctx, AST_EXPRESSION *expr)
{ 
  switch( expr->exp_type ) 
  {
    case S_EXPR_BINARY:
      CHECKER_expr( ctx, (AST_EXPRESSION *) expr->val.expr.expr_left );
      CHECKER_expr( ctx, (AST_EXPRESSION *) expr->val.expr.expr_right );
      AST_EXPRESSION_binary_op_check_types( ctx, expr );
      break;

    case S_EXPR_UNARY:
      CHECKER_expr( ctx, (AST_EXPRESSION *) expr->val.unary.expr );
      AST_EXPRESSION_unary_op_check_types( ctx, expr );
      break;

    case S_EXPR_REFERENCE: {
      BINDING_ENTRY *binding;
      int scope_nesting;
      REF_SCOPE scope_type; 
      const char *name;

      if (expr->val.ref.indexes ) {
         CHECKER_pass( ctx, (AST_BASE *) expr->val.ref.indexes );
      }
    
#if 0      	
      name = get_var_name( expr, &scope_type );
      if (name == 0) {
	 // don't check - reference to this as is
	 return;
      }
#endif

      name = expr->val.ref.lhs;
      scope_type = expr->val.ref.scope;

      // check if name is defined as variable, if yes return definition of variable (binding)
      binding = lookup_binding( ctx, name, scope_type, &scope_nesting );

      if (!binding )  {         
        AST_BASE *func_def = 0;

	// local scope and reference to global function have the same syntax, if not defined as local, check if this is a named function ref.
	if ( scope_type == REF_SCOPE_LOCAL ) {  
          func_def = PARSECONTEXT_find_function_def( ctx, name ); 
        }
	
	if (func_def) {

           if (ctx->chkctx.is_left_hand_side) {
	      do_yyerror( &expr->base.location, ctx, "%s is a function name, so this name can't be assigned a value", name );
	      return;
	   }
	   
           // the expression is turned into a function reference,
	   if (expr->exp_type == S_EXPR_REFERENCE) {
             expr->exp_type = S_EXPR_LAMBDA_RESOLVED_REF;
             expr->value_type = S_VAR_CODE;
             expr->val.fdecl = func_def;
	   }
           return ;
        }

        if (ctx->chkctx.is_function_name) {
          do_yyerror( &expr->base.location, ctx, "the function %s is not defined, can't call a function that is not defined", name );
          return ;
        }       
      } else {
	expr->val.ref.binding = binding;
      }

      if (binding && binding->has_value) { // variable already defined, check that usage is according to definition
	    
#if 0	    
	AST_EXPRESSION *first_index =  get_first_index( expr );
#endif	
	AST_EXPRESSION *first_index = 0; 	
	int is_error = 0;
	
	if (expr->val.ref.indexes != 0)
	    first_index = (AST_EXPRESSION *) AST_VECTOR_get( expr->val.ref.indexes, 0 ); 

	if (first_index != 0 ) {
	   
	    if (first_index->exp_type == S_EXPR_ARRAY_INDEX) {	    // variable used as array
	      if (binding->value_type == S_VAR_ANY) {		    //   if bining can have any value then ...
	        binding->value_type = S_VAR_LIST;		    //     this is first usage. define binding as array
		expr->value_type = S_VAR_ANY;			    //   an expression can any value (result of array lookup)
	      } else						    //     has already been used as something
	      if ((binding->value_type & S_VAR_LIST) == 0) {	    //       check if current usage is consistent with what we know
	        is_error = 1;
	      }
	    } else if (first_index->exp_type == S_EXPR_HASH_INDEX) {   // variable used as table
	      if  (binding->value_type == S_VAR_ANY) {		    //   if binding can have any value then ...
		binding->value_type = S_VAR_HASH;	            //     this is first usage. define binding as hash.
		expr->value_type = S_VAR_ANY;			    // an exoression can have any value (result of hash lookup)
	      } else						    //   hash already been used as something
	      if  ((binding->value_type & S_VAR_HASH) == 0) {	    //     check if current usage is consisten with what we know
	        is_error = 2;
	      }	
	    }	
	} else {						    // variable is used as scalar
	    if (binding->value_type != S_VAR_ANY) {		    //   if variable is already used as something.
	      expr->value_type = binding->value_type;		    //     expression type is according to variable type.
	    }
	}


#if 0	
	if ( is_scalar_var_type( binding->value_type ) ) {
	   if (first_index != 0) {
	      is_error = 1;
	   }
           expr->value_type = binding->value_type;
	} else if (binding->value_type == S_VAR_LIST) {
	   if (first_index == 0) {
	      expr->value_type = S_VAR_LIST;
	      return;
	  } else if (first_index->exp_type != S_EXPR_ARRAY_INDEX) {
	      is_error = 3;
	  } else {
	      expr->value_type = S_VAR_ANY;
	  }
	} else if (binding->value_type == S_VAR_HASH) {
	  if (first_index == 0) {
	      expr->value_type = S_VAR_HASH;
	      return;
	  } else if (first_index->exp_type != S_EXPR_HASH_INDEX) {
	      is_error = 5;
	  } else {
	      expr->value_type = S_VAR_ANY;
	  }
        }
#endif	
	if (is_error) {
	    do_yyerror( &expr->base.location, ctx, "Variable %s is defined as %s, but used as %s", 
		    name,
		    get_type_name( binding->value_type ),
		    first_index == 0 ? "scalar" :  (first_index->exp_type == S_EXPR_HASH_INDEX ? "table" : "array" ) );

	    if (binding) {
	      do_yyerror( &binding->def_location, ctx, "First definition of variable %s", name );
	    }
	}
      }

#if 0
      if (expr->val.ref.scope == REF_SCOPE_THIS) {
         if (strcmp( expr->val.ref.lhs, "this" ) == 0)
           expr->value_type = S_VAR_HASH;
	 else 
           expr->value_type = S_VAR_ANY;
      }
#endif

      switch( scope_type ) {
        case REF_SCOPE_THIS:
	  return;
	case REF_SCOPE_GLOBAL: 
          if (!binding) {
	    do_yyerror( &expr->base.location, ctx, "Global variable %s is used without having been assigned a value", name );
	    return;
	  }
	  break;
        case REF_SCOPE_CLOSURE: {
	  ARRAY *outer_refs; 
	  BINDING_ENTRY *entry, *new_entry;
	  size_t i;
	 
          if (!binding) {
	    do_yyerror( &expr->base.location, ctx, "Closure variable %s is used without having been assigned a value", name );
 	    return;
	  }
	  assert( ctx->chkctx.current_function ); 

	  // find if closure hsas already been defined.
	  outer_refs = &ctx->chkctx.current_function->outer_refs;  
	  
	  for( i = 0; i < ARRAY_size( outer_refs ); i++ ) {
	    void **tmp = (void **) ARRAY_at( outer_refs, i ); 
	    entry = (BINDING_ENTRY *) *tmp;
	    assert( entry->scope == REF_SCOPE_CLOSURE );
	    if (entry->source == binding) {
	      break;
	    }
	  }
	  if (i < ARRAY_size( outer_refs )) {
            expr->val.ref.binding = entry;
	    return;
	  }
	  new_entry = make_binding( ctx, binding->value_type, &expr->base.location, name, 1, REF_SCOPE_CLOSURE  );
	  new_entry->source = binding;
	  new_entry->stack_offset = i;
	  new_entry->scope_nesting = scope_nesting;  
	  
	  ARRAY_push_back( outer_refs, &new_entry, sizeof( void * ) );
	  }
	  break;
	case REF_SCOPE_LOCAL: {
          AST_VAR_TYPE binding_type = S_VAR_ANY;
	  AST_EXPRESSION *idx;

          if (binding == 0) {
	    if (ctx->chkctx.pass != 0) {
	      return;
	    }
	    if (ctx->chkctx.is_left_hand_side) {
	      // ... but for locals only.

	      if (expr->val.ref.indexes) {

		idx = (AST_EXPRESSION *) AST_VECTOR_get( expr->val.ref.indexes, 0 );
	        assert( idx->base.type == S_EXPRESSION && (idx->exp_type == S_EXPR_HASH_INDEX || idx->exp_type == S_EXPR_ARRAY_INDEX) );
		if (idx->exp_type == S_EXPR_ARRAY_INDEX) {
		  binding_type = S_VAR_LIST;
                } else {
		  binding_type = S_VAR_HASH;
		}
		expr->value_type = S_VAR_ANY;  // collection reference can be of any value.

	      } else {
		// its a scalar value, so it can be either string or number ???
		binding_type = S_VAR_ANY;
	      }

	      binding =  make_binding(  ctx, binding_type, &expr->base.location, expr->val.ref.lhs, 1, REF_SCOPE_LOCAL  );
      	    //scope = 0;
            } else {
	      do_yyerror( &expr->base.location, ctx, "local variable %s is used without having been assigned a value", expr->val.ref.lhs );
	      return;
	    } 	
	  } else { // binding not null - already defined or parameter
	    if (binding->has_value == 0) { // parameter value is used for the first time.
	      if (expr->val.ref.indexes != 0) {
		idx = (AST_EXPRESSION *) AST_VECTOR_get( expr->val.ref.indexes, 0 );
	        assert( idx->base.type == S_EXPRESSION && (idx->exp_type == S_EXPR_HASH_INDEX || idx->exp_type == S_EXPR_ARRAY_INDEX) );
		if (idx->exp_type == S_EXPR_ARRAY_INDEX) {
		  binding_type = S_VAR_LIST;
                } else {
		  binding_type = S_VAR_HASH;
		}
	      } else {
		binding_type = S_VAR_ANY; // can still assign any value to this here.
	      }
	      expr->value_type = S_VAR_ANY;
              binding->value_type = binding_type;
	      binding->def_location = expr->base.location;
	      binding->has_value = 1;
	    }
	  }
	  }
	  break;
      }
      expr->val.ref.binding = binding;
#if 0      
      expr->val.ref.scope = scope;
#endif      
      }
      break;

    case S_EXPR_CONSTANT:
      if (expr->value_type == S_VAR_GRAMMAR) {
        CHECKER_pass( ctx, expr->val.const_value.grammar_value );
      }
      break;

    case S_EXPR_FUNCALL: {
      AST_FUNC_CALL *func = expr->val.fcall;
      AST_BASE *fdecl;       
      AST_VAR_TYPE func_type;
      int is_error = 0;
      
      CHECKER_pass( ctx, &func->base );

      if (func->f_name->exp_type != S_EXPR_LAMBDA && func->f_name->exp_type != S_EXPR_LAMBDA_RESOLVED ) {
        return;
      }
      
      fdecl = func->func_decl = func->f_name->val.fdecl;
      if (!fdecl) {
        return;
      }

      if (func->base.type == S_FUN_DECL) {
        func_type = ((AST_FUNC_DECL *) fdecl)->return_type_value;
      } else {
        func_type = ((AST_XFUNC_DECL *) fdecl)->return_type_value;
      }
 
      // ugly - test is true if in expression context, but not true for FOR statements.
      if (func->base.parent == 0 || func->base.parent->parent == 0 || func->base.parent->parent->type != S_FOR) {

      if (ctx->chkctx.pass == 0) {
	expr->value_type = func_type == 0 ? S_VAR_ANY : func_type;
      } else {
        AST_VAR_TYPE offending_type;

	if (func_type == 0) {
	    do_yyerror( &expr->base.location, ctx, "Function %s does not return a value, but is used as %s", get_func_name(fdecl), get_type_name( expr->value_type ) );
	    is_error = 1;
	} else if (can_assign( expr->value_type, func_type, &offending_type)) {
	    do_yyerror( &expr->base.location, ctx, "Function %s returns %s but is used as %s", get_func_name(fdecl), get_type_name( func_type ), get_type_name2( expr->value_type ));
	    is_error = 1;
	}

	if (is_error && fdecl->type != S_XFUN_DECL)  {
	    do_yyerror( &fdecl->location, ctx, "Function %s is defined here", get_func_name(fdecl) );
	}
	}
       }
      }
      break;

    case S_EXPR_HASH_INDEX:
      ///fprintf( out, "hash-idx , ");
      CHECKER_pass( ctx, (AST_BASE *) expr->val.index_expr );
      break;
  
    case S_EXPR_ARRAY_INDEX: {
      AST_EXPRESSION *index_expr;
      AST_VAR_TYPE index_expr_type;

      CHECKER_pass( ctx, (AST_BASE *) expr->val.index_expr );

      index_expr = expr->val.index_expr;
      index_expr_type =  AST_EXPRESSION_type( index_expr ); 

      if (! is_numeric_type( index_expr_type ) ) {
	  do_yyerror( &expr->base.location, ctx, "Array expects number as index value, instead has %s", get_type_name(  index_expr_type ) ); 
      }
      }
      break;
  
    case S_EXPR_LIST_VALUES:
      //fprintf(out, "list-constructor , " ); 
      CHECKER_pass( ctx, (AST_BASE *) expr->val.index_expressions );
      break;
 
    case S_EXPR_HASH_VALUES:
      //fprintf(out, "hash-constructor , " ); 
      CHECKER_pass( ctx, (AST_BASE *) expr->val.index_expressions );
      break;

    case S_EXPR_PLACEHOLDER:
      //fprintf( out, "_" );
      break;

    case S_EXPR_LAMBDA:
      //fprintf(out, "anonymous-func , " ); 
      CHECKER_pass( ctx, (AST_BASE *) expr->val.fdecl );
      break;
    
    case S_EXPR_LAMBDA_RESOLVED:  
    case S_EXPR_LAMBDA_RESOLVED_REF:  
       break;
    case S_EXPR_ERROR:
      //fprintf(out, "error-expression , " ); 
      break;

    default:
      assert(0);
  }
}



AST_FUNC_CALL_PARAM *CHECKER_find_param_by_label( AST_VECTOR *vect, const char *param_name)
{
    size_t i;
    AST_FUNC_CALL_PARAM *param;
    
    if (!vect) {
      return 0;
    }

    for( i = 0; i < AST_VECTOR_size( vect ); i++ ) {
	param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( vect, i );
	if (strcmp( param->label_name, param_name ) == 0) {
	    return param;
	}
    }
    return 0;
}

AST_EXPRESSION *CHECKER_find_param_by_label2( AST_VECTOR *vect, const char *param_name, size_t *num_param)
{
    size_t i;
    AST_EXPRESSION *param;
   
    if (!vect) {
      return 0;
    }

    for( i = 0; i < AST_VECTOR_size( vect ); i++ ) {
	param = (AST_EXPRESSION *) AST_VECTOR_get( vect, i );
	if (strcmp( param->val.ref.lhs, param_name ) == 0) {
            *num_param = i;
	    return param;
	}
    }
    return 0;
}

AST_XFUNC_PARAM_DECL *CHECKER_xfind_param_by_label( AST_XFUNC_DECL *xfunc , const char *param_name, size_t *num_param )
{
    size_t i;
    AST_XFUNC_PARAM_DECL *param;
    
    for( i = 0; i < MAX_XFUNC_PARAM; i++ ) {
	param = &xfunc->params[ i ];

	if (!param->param_name) {
	  break;
	}
	if (strcmp( param->param_name, param_name ) == 0) {
            *num_param = i;
	    return param;
	}
    }
    return 0;
}

int  CHECKER_check_func_call_params( PARSECONTEXT *ctx, int pass, AST_FUNC_CALL *fcall, AST_BASE *func_def )
{
    size_t i, num_param;
    AST_FUNC_CALL_PARAM *param;
    AST_EXPRESSION *param_expr;
    int rt = 0;

    if (func_def->type == S_FUN_DECL) {
       AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) func_def;
       AST_EXPRESSION *parame;
       AST_EXPRESSION *fparam;

       // variable argument function accepts any arguments.
       if (fdecl->var_arguments) {
         return 0;
       }
       if (pass == 0) { // ctx->chkctx.pass == 0) {
        if (fcall->call_params != 0)
	 for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
	   if ((parame = CHECKER_find_param_by_label2( fdecl->func_params, param->label_name, &num_param )) == 0) {
             do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s", fdecl->f_name, param->label_name );
	     rt = -1;
	   }
	   param->param_num = num_param;
           param->param_decl = parame;
           if (parame) {
	     param->param_spec = parame->value_type & S_VAR_OPT_PARAM_MASK;  
           }
	}	
        if (fdecl->func_params)
         for( i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
	   parame = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
	   if (CHECKER_find_param_by_label( fcall->call_params, parame->val.ref.lhs ) == 0 && (parame->value_type  & S_VAR_PARAM_OPTIONAL) == 0 ) {
             do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the required parameter ~%s", parame->val.ref.lhs );
	     rt = -1;
	   }
         }
       } else { // pass 1
        if (fcall->call_params)
	 for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   AST_VAR_TYPE var_type;
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
           param_expr = param->expr;
           //fparam = CHECKER_find_param_by_label2( fdecl->func_params, param->label_name );
           fparam = (AST_EXPRESSION *) param->param_decl;
	   if (fparam->val.ref.binding) {
	      BINDING_ENTRY *entry = fparam->val.ref.binding;

	      if (entry->has_value) {
	        var_type = entry->value_type;
	      }	
	   } else {
	     var_type = fparam->value_type;
	   }
            
           if ( fparam &&  can_assign( param_expr->value_type, var_type & S_VAR_ALL_TYPES , 0)) {
             do_yyerror( &param->base.location, ctx, "parameter ~%s is called as %s but defined as %s", param->label_name, get_type_name( param_expr->value_type), get_type_name2( fparam->value_type ) );
	     rt = -1;
           } 
	   if (param_expr->exp_type == S_EXPR_REFERENCE) { 
    	     assign_value( param_expr, fparam->value_type, &param->base.location );
	   }  
         }
       }
     } else { // xfunc
       AST_XFUNC_DECL *xfunc = (AST_XFUNC_DECL *) func_def;
       AST_XFUNC_PARAM_DECL *xparam;
        
       if (pass == 0) { //if (ctx->chkctx.pass == 0) {
      
         if (fcall->call_params) 
          for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
  	
	   xparam = CHECKER_xfind_param_by_label( xfunc, param->label_name, &num_param ); 
	   if (xparam == 0  ) {
             do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s",  xfunc->f_name, param->label_name );
	     rt = -1;
	   }
	   param->param_num = num_param;
           param->param_decl = xparam;
           if (xparam) {
	     param->param_spec = xparam->var_type & S_VAR_OPT_PARAM_MASK;  
           }  
          }
     
         for( i = 0; i < MAX_XFUNC_PARAM; i++ ) {
 	   xparam = &xfunc->params[ i ];
	   if (!xparam->param_name) {
	     break;
	   } 
	 
	   if (CHECKER_find_param_by_label( fcall->call_params, xparam->param_name ) == 0 && (xparam->var_type & S_VAR_PARAM_OPTIONAL) == 0 ) {
	     do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the required parameter ~%s", xparam->param_name );
	     rt = -1;
	   }
         }
       
       } else { // pass 1

         if (fcall->call_params) 
          for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
           param_expr = param->expr;
	 //xparam = CHECKER_xfind_param_by_label( xfunc, param->label_name ); 
           xparam = (AST_XFUNC_PARAM_DECL *) param->param_decl;
	
	   if (xparam &&  can_assign( param_expr->value_type, xparam->var_type & S_VAR_ALL_TYPES,  0)) {
	     do_yyerror( &param->base.location, ctx, "parameter ~%s is called as %s but defined as %s", param->label_name, get_type_name( param_expr->value_type), get_type_name2( xparam->var_type ) );
	     rt = -1;
	   }

	   if (param_expr->exp_type == S_EXPR_REFERENCE) { 
 
		assign_value( param_expr, xparam->var_type, &param_expr->base.location );
	   }
	 }
       }
    }
    return rt;
}

int  CHECKER_check_func_call( PARSECONTEXT *ctx, AST_FUNC_CALL *fcall )
{
    AST_EXPRESSION *f_name;
    const char *func_name;
    AST_BASE *func_def = 0; 
   
    f_name =  fcall->f_name;

    ctx->chkctx.is_function_name = 1;
    CHECKER_pass( ctx, (AST_BASE *) fcall->f_name );
    ctx->chkctx.is_function_name = 0;
     
    CHECKER_pass( ctx, (AST_BASE *) fcall->call_params );

    if (f_name->exp_type == S_EXPR_LAMBDA || f_name->exp_type == S_EXPR_LAMBDA_RESOLVED || f_name->exp_type == S_EXPR_LAMBDA_RESOLVED_REF ) {
      func_def = f_name->val.fdecl;
    }


    if (f_name->exp_type == S_EXPR_LAMBDA_RESOLVED_REF) {
      f_name->exp_type = S_EXPR_LAMBDA_RESOLVED; // now we know it is part of function call. 
    }


    if (!func_def) {
      return 0;
    }
    func_name =  ((AST_FUNC_DECL *) func_def)->f_name;
    
    fcall->func_decl = func_def;

    CHECKER_check_func_call_params( ctx, ctx->chkctx.pass, fcall, func_def );

#if 0
    if (func_def->type == S_FUN_DECL) {
       AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) func_def;
       AST_EXPRESSION *parame;
       AST_EXPRESSION *fparam;

       // variable argument function accepts any arguments.
       if (fdecl->var_arguments) {
         return 0;
       }
       if (ctx->chkctx.pass == 0) {
        if (fcall->call_params != 0)
	 for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
	   if (CHECKER_find_param_by_label2( fdecl->func_params, param->label_name ) == 0) {
             do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s", fdecl->f_name, param->label_name );
	   }
	   param->param_num = i;
         }	
        if (fdecl->func_params)
         for( i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
	   parame = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
	   if (CHECKER_find_param_by_label( fcall->call_params, parame->val.ref.lhs ) == 0) {
             do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the mandatory parameter ~%s", parame->val.ref.lhs );
	   }
         }
       } else { // pass 1
        if (fcall->call_params)
	 for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
           param_expr = param->expr;
           fparam = CHECKER_find_param_by_label2( fdecl->func_params, param->label_name );
            
           if ( fparam &&  can_assign( param_expr->value_type, fparam->value_type & S_VAR_ALL_TYPES , 0)) {
             do_yyerror( &param->base.location, ctx, "parameter ~%s is called as %s but defined as %s", param->label_name, get_type_name( param_expr->value_type), get_type_name( fparam->value_type ) );
           } 
           assign_value( param_expr, fparam->value_type, &param->base.location );
         }
       }
     } else { // xfunc
       AST_XFUNC_DECL *xfunc = (AST_XFUNC_DECL *) func_def;
       AST_XFUNC_PARAM_DECL *xparam;
        
       if (ctx->chkctx.pass == 0) {
      
         if (fcall->call_params) 
          for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
  	
	   xparam = CHECKER_xfind_param_by_label( xfunc, param->label_name ); 
	   if (xparam == 0  ) {
             do_yyerror( &param->base.location, ctx, "function %s does not have a parameter ~%s",  xfunc->f_name, param->label_name );
	   }
	   param->param_num = i;
          }
     
         for( i = 0; i < MAX_XFUNC_PARAM; i++ ) {
 	   xparam = &xfunc->params[ i ];
	   if (!xparam->param_name) {
	     break;
	   } 
	 
	   if (CHECKER_find_param_by_label( fcall->call_params, xparam->param_name ) == 0 && (xparam->var_type & S_VAR_PARAM_OPTIONAL) == 0 ) {
	     do_yyerror( &fcall->call_params->base.location, ctx, "function is called without the required parameter ~%s", xparam->param_name );
	   }
         }
       } else {
         if (fcall->call_params) 
          for( i = 0; i < AST_VECTOR_size( fcall->call_params ); i++ ) {
	   param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( fcall->call_params, i );
           param_expr = param->expr;
	   xparam = CHECKER_xfind_param_by_label( xfunc, param->label_name ); 
	
	   if (xparam &&  can_assign( param_expr->value_type, xparam->var_type & S_VAR_ALL_TYPES,  0)) {
	     do_yyerror( &param->base.location, ctx, "parameter ~%s is called as %s fined as %s", param->label_name, get_type_name( param_expr->value_type), get_type_name( xparam->var_type ) );
	   }
 
           assign_value( param_expr, xparam->var_type, &param_expr->base.location );
	 }
       }
    }
#endif    
    return 0;
}

void CHECKER_func_decls( PARSECONTEXT *ctx, AST_VECTOR *func_decls );


void CHEKCER_func_decl( PARSECONTEXT *ctx, AST_FUNC_DECL *scl )
{
  AST_FUNC_DECL *prev;
  AST_EXPRESSION *param;
  BINDING_ENTRY *binding;
  AST_VECTOR *prev_func_decls;
  size_t i;

  prev = ctx->chkctx.current_function;
  ctx->chkctx.current_function = scl;

  prev_func_decls = ctx->chkctx.func_decls;
  ctx->chkctx.func_decls = 0;

  if (ctx->chkctx.pass == 0) {
    scl->return_type_value = 0;
  }

  if (scl->func_params != 0 && ctx->chkctx.pass == 0) {

    // create bindings for parameters (of unknown type so far).
    for( i = 0 ; i < AST_VECTOR_size( scl->func_params ); i ++ ) {
      param = (AST_EXPRESSION *) AST_VECTOR_get( scl->func_params, i );
      binding =  make_binding(  ctx, S_VAR_UNKNOWN, &param->base.location, param->val.ref.lhs, 0, REF_SCOPE_LOCAL );
      param->val.ref.binding = binding;
    }
  }
  CHECKER_pass ( ctx, (AST_BASE *) scl->func_body );
   
#if 0   
  // update parameter types
  if (scl->func_params) {
    for( i = 0 ; i < AST_VECTOR_size( scl->func_params ); i ++ ) {
       param = (AST_EXPRESSION *) AST_VECTOR_get( scl->func_params, i );
       binding = param->val.ref.binding;
       param->value_type = binding->value_type;
    }
  }
#endif  

  if (ctx->chkctx.func_decls != 0) {
    CHECKER_func_decls( ctx, ctx->chkctx.func_decls );
    AST_VECTOR_free( ctx->chkctx.func_decls );
  }

  ctx->chkctx.current_function = prev;
  ctx->chkctx.func_decls = prev_func_decls;

}

void CHECKER_func_decls( PARSECONTEXT *ctx, AST_VECTOR *func_decls )
{
  size_t i;
  AST_FUNC_DECL *scl;

  for( i = 0; i < AST_VECTOR_size( func_decls ); i++ ) {
    scl = (AST_FUNC_DECL *)  AST_VECTOR_get(func_decls, i );

    CHEKCER_func_decl( ctx, scl );
 }
}

int can_assign( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type, AST_VAR_TYPE *offending_type)
{
   if (lhs_type == S_VAR_ANY) {
     return 0;
   }

   if ((lhs_type & rhs_type) != 0) {
     return 0;
   }
   if (is_numeric_type( lhs_type) && is_numeric_type( rhs_type ) ) {
     return 0;
   }

   //??? Assign string to integer or integer to string.
   if (offending_type) {
     *offending_type = rhs_type & ~ lhs_type;
   }
   return -1;
}

int is_narrower_type( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type  )
{
   if ((lhs_type & rhs_type) == 0) {
     return 0;
   }
   if (lhs_type == rhs_type) {
     return 0;
   }
   if ((~ rhs_type & lhs_type) == 0) {
     return 0;
   }
   if (rhs_type & ~lhs_type) {
     return 0;
   }
   return 1;
}

int can_assign_const_value( AST_EXPRESSION * lhs, AST_EXPRESSION * rhs )
{
    BINDING_ENTRY *binding;

    assert( lhs->exp_type ==  S_EXPR_REFERENCE );

    if (rhs->exp_type !=  S_EXPR_CONSTANT && rhs->exp_type != S_EXPR_LIST_VALUES && rhs->exp_type != S_EXPR_HASH_VALUES) {
      return 0;
    }

    binding = lhs->val.ref.binding;
    return binding && binding->value_type == S_VAR_ANY && is_indexed_ref( lhs ) == 0; 
}      


void  CHECKER_pass( PARSECONTEXT *ctx, AST_BASE *base )
{
  size_t i;
  assert(base );

  switch( base->type ) {

  case S_EXPRESSION:
    CHECKER_expr( ctx, (AST_EXPRESSION *) base );
    break;

  case S_AST_LIST: {
    AST_BASE_LIST *scl  = (AST_BASE_LIST *) base;
    DRING *cur;

    DRING_FOREACH( cur, &scl->statements ) {
	CHECKER_pass( ctx,  _OFFSETOF( cur, AST_BASE, entry ) );
    }
    }
    break;

  case S_AST_VECTOR: {
    AST_VECTOR *vect = (AST_VECTOR *)base;

    for( i = 0; i < AST_VECTOR_size( vect ); i++) {
	CHECKER_pass( ctx, AST_VECTOR_get( vect, i ));
    }
    }
    break;
  
  case S_ASSIGNMENT: {
    AST_ASSIGNMENT *scl = (AST_ASSIGNMENT *) base;
    AST_EXPRESSION *lhs,*rhs;
    AST_VAR_TYPE offending_type; 
    BINDING_ENTRY *binding; 
    ctx->chkctx.is_left_hand_side = 1;
    CHECKER_pass( ctx, (AST_BASE *) scl->left_side );
    ctx->chkctx.is_left_hand_side = 0; 
	
    CHECKER_pass( ctx, (AST_BASE *) scl->right_side );
    
    lhs = (AST_EXPRESSION *) scl->left_side; 
    rhs = (AST_EXPRESSION *) scl->right_side;

#if 0
    // assignment of array constructor
    if (rhs->exp_type == S_EXPR_HASH_VALUES || rhs->exp_type == S_EXPR_LIST_VALUES ) {
        // .. and this is not multi value assignment
        if (lhs->exp_type != S_EXPR_LIST_VALUES) {
           // means that constructe value of collection will be copied as is into left hand side
           scl->type = CP_MOVE; 
        }   
    }
#endif    
 
    if (lhs->exp_type != S_EXPR_LIST_VALUES) { // not multi value left hand side

        assert( lhs->exp_type ==  S_EXPR_REFERENCE );

	if (
            can_assign( AST_EXPRESSION_type( lhs ) , AST_EXPRESSION_type( rhs ), &offending_type ) &&
              !can_assign_const_value( lhs, rhs ) 
            ) {
	  do_yyerror( &lhs->base.location, ctx, "Can't assign the %s value to %s ", get_type_name( offending_type ), lhs->val.ref.lhs);
	
          binding = lhs->val.ref.binding;
	  if (binding) {
	      do_yyerror( & binding->def_location, ctx, "First definition of variable %s as %s", lhs->val.ref.lhs, get_type_name( AST_EXPRESSION_type( lhs ) ) ) ;
	  }
	  return;    
	} 

	// if type of right hand side is narrower than left hand side -> lhs get type of rhs
	if (is_narrower_type( AST_EXPRESSION_type( lhs) , AST_EXPRESSION_type( rhs ) ) ) {
	    assign_value( lhs, AST_EXPRESSION_type( rhs ), &scl->base.location );
	}
    }
    }
    break;
  
  case S_IF: {
    AST_COND *cond = (AST_COND *) base;
	
    if (cond->condition) {  
      CHECKER_pass( ctx, (AST_BASE *) cond->condition );
      CHECKER_pass( ctx, (AST_BASE *) cond->block );
    } else {
      CHECKER_pass( ctx, (AST_BASE *) cond->block );
    }
    if (cond->elsecond ) {
      CHECKER_pass( ctx, (AST_BASE *) cond->elsecond  );
    } 
    }
    break;
 
  case S_WHILE: {
    AST_WHILE_LOOP *scl = (AST_WHILE_LOOP *) base;
    
    if (scl->condition) {
      CHECKER_pass( ctx, (AST_BASE *) scl->condition );
    }

    CHECKER_pass( ctx, (AST_BASE *) scl->block );
    }
    break;

  case S_FOR:  {
    AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
    
    ctx->chkctx.is_left_hand_side = 1; 
    CHECKER_pass( ctx, (AST_BASE *) scl->loop_var );
    ctx->chkctx.is_left_hand_side = 0;

    CHECKER_pass( ctx, (AST_BASE *) scl->loop_expr );
    CHECKER_pass( ctx, (AST_BASE *) scl->block );
    }
    break;
  
  case S_FUN_CALL_PARAM: {
    AST_FUNC_CALL_PARAM *scl = (AST_FUNC_CALL_PARAM *) base;
    
    CHECKER_pass( ctx, (AST_BASE *) scl->expr );
    }
    break;

  case S_FUN_CALL: {
    AST_FUNC_CALL *scl = (AST_FUNC_CALL *) base;
    
    CHECKER_check_func_call( ctx, scl );
    }
    break;

  case S_FUN_DECL: {
    AST_FUNC_DECL *scl = (AST_FUNC_DECL *) base;
    YYLTYPE loc;

    if (ctx->chkctx.is_root_func) {
      ctx->chkctx.is_root_func  = 0;
      CHEKCER_func_decl( ctx, scl );
    } else {
      if ( !ctx->chkctx.func_decls) {
         ctx->chkctx.func_decls = AST_VECTOR_init( &loc );
      }
      AST_VECTOR_add( ctx->chkctx.func_decls, &scl->base );
    }
    }
    break;
  
  case S_XFUN_DECL: {
    }
    break;
  
  case S_RETURN: {
    AST_RETURN *scl = (AST_RETURN *) base;
    
    if (scl->rvalue) {
      CHECKER_pass( ctx, (AST_BASE *) scl->rvalue );
      ctx->chkctx.current_function->return_type_value |= AST_EXPRESSION_type( scl->rvalue );
    }
    
    }
    break;

  case S_NEXT:
    break;
  
  case S_BREAK:
    break;

  case S_PP_RULE_REF:
  case S_PP_ALTERNATIVE:
  case S_PP_RULE_CONSTANT:
  case S_PP_CHAR_CLASS:
  case S_PP_RULE: {
    AST_PP_BASE *scl = (AST_PP_BASE *) base;
    if (scl->rule_script) {
        CHECKER_pass( ctx, scl->rule_script );
    }        
    }
    break;
 
  case S_NULL:
    break;

  default:
    assert(0);
  }
  
}

int CHECKER_run( CHECKERCTX *ctx, struct tagAST_BASE *program)
{
  PARSECONTEXT *pctx = _OFFSETOF( ctx, PARSECONTEXT, chkctx );
  AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) program;

  if (program->type == S_FUN_DECL) { //temporary ?
  
  ctx->func_decls = 0;
  ctx->is_root_func = 1;

  // declare built in global variables.
  make_binding2( pctx, fdecl, S_VAR_LIST, 0, "ARGV", 1, REF_SCOPE_GLOBAL  );
  make_binding2( pctx, fdecl, S_VAR_HASH, 0, "ENV", 1, REF_SCOPE_GLOBAL  );
  }

  ctx->pass = 0;
  CHECKER_pass( pctx, &fdecl->base );

  if (pctx->my_yy_is_error) {
    return pctx->my_yy_is_error;
  }
  
  ctx->pass = 1;
  ctx->is_root_func = 1;

  CHECKER_pass( pctx, &fdecl->base );

  
  return pctx->my_yy_is_error;
}


