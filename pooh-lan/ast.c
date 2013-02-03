#include "ast.h"
#include "pars.h"
#include "parsectx.h"
#include "lexer.h"
#include "parser.h"


AST_EXPRESSION * AST_EXPRESSION_init_ref( const char *name, AST_VECTOR *indexes, YYLTYPE *location, PARSECONTEXT *parse_context)
{
  AST_EXPRESSION *scl;
  REF_SCOPE scope;

  scl = AST_EXPRESSION_init( S_EXPR_REFERENCE, S_VAR_ANY, location );
  if (!scl) {
    return 0;
  }

  scl->val.ref.lhs = strdup( name );
  scl->val.ref.binding = 0;

  scl->value_type = S_VAR_ANY;
 
  if (strcmp(name,"this") == 0) {
    scope =  REF_SCOPE_THIS; 
  } else if (strcmp(name,"global") == 0) {
    scope =  REF_SCOPE_GLOBAL; 
  } else if (strcmp(name,"outer") == 0) {
    scope = REF_SCOPE_CLOSURE; 
  } else {
    scope = REF_SCOPE_LOCAL;
  }
  scl->val.ref.scope = scope;

  if (scope == REF_SCOPE_GLOBAL || scope == REF_SCOPE_CLOSURE) {
     if (scope == REF_SCOPE_CLOSURE && indexes == 0) {
       do_yyerror( location, parse_context, "variable name must have the form: outer . name_of_outer_variable " );
       return 0;
     }
     if (scope == REF_SCOPE_GLOBAL && indexes == 0) {
       do_yyerror( location, parse_context, "variable name must have the form: global . name_of_outer_variable " );
       return 0;
     }

     if (indexes) {
	AST_EXPRESSION * index_expr = (AST_EXPRESSION *) AST_VECTOR_get( indexes, 0 );
	assert( index_expr->base.type == S_EXPRESSION );
	if (index_expr->exp_type == S_EXPR_HASH_INDEX &&  index_expr->val.index_expr->exp_type == S_EXPR_CONSTANT) {
	    AST_BASE *ptr;

  	    free( scl->val.ref.lhs );
	    scl->val.ref.lhs = index_expr->val.index_expr->val.const_value.string_value;
	    ptr = AST_VECTOR_shift( indexes );
            if (AST_VECTOR_size( indexes ) == 0) {
              AST_VECTOR_free( indexes );
              indexes = 0;
            }
	    free(ptr);
	} else {
	    assert( index_expr != 0 );
            do_yyerror( location, parse_context, "index epression not allowed on scope specifier" );
	    return 0;
	}
     }
  }

  if (indexes) {
    assert( indexes->base.type == S_AST_VECTOR );
    indexes->base.parent = &scl->base;
    
#if 0    
    first_idx = (AST_EXPRESSION *) AST_VECTOR_get( indexes, 0 );   
    if (first_idx) {
      if (first_idx->exp_type == S_EXPR_ARRAY_INDEX) {
	scl->value_type = S_VAR_LIST;
      } else {
	scl->value_type = S_VAR_HASH;
      }
    }
#endif    
  }
  scl->val.ref.indexes = indexes;
#if 0  
  scl->val.ref.scope = 0;
#endif  
  return scl;
}
 
void assign_value( AST_EXPRESSION *lhs, AST_VAR_TYPE value_type, YYLTYPE *location );
int is_indexed_ref( AST_EXPRESSION *expr );
int is_narrower_type( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type  );

AST_BASE *compile_string( const char *string, int init_token, const char *file_name, YYLTYPE *location, int report_errors )
{
   PARSECONTEXT ctx;
   AST_BASE *rval = 0;
   
   if (PARSECONTEXT_init( &ctx, 0) ) {
     return 0;
   }

   if (LEXER_open_string( &ctx.lexctx, string, init_token, location ) ) {
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
   return rval;
}

static AST_EXPRESSION * AST_compile_string_part( PARSECONTEXT *pc, size_t pos )
{
  STRING_PART **ptr, *cur;
  const char *current_file;
  AST_BASE *base;
  AST_EXPRESSION *ret;

  if (pos >= ARRAY_size( &pc->lexctx.string_parts )) {
    return 0;
  }

  ptr = (STRING_PART **) ARRAY_at( &pc->lexctx.string_parts, pos  );
  cur = *ptr;
   
  current_file = LEXER_get_current_file_name( &pc->lexctx );

  if (cur->is_expression) {
    base = compile_string( STRING_PART_get( cur ), TK_START_EXPRESSION,current_file, &cur->loc, 0  ); 
    if (!base) {
      base = compile_string( STRING_PART_get( cur ), TK_START_STATEMENT, current_file, &cur->loc, 0 );
      if (!base) {
	do_yyerror( &cur->loc, pc, "Failed to parse the following string - neither statement nor expression: %s", STRING_PART_get( cur ) );   
	return 0;
      }
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
      fcall->func_decl = (AST_BASE *) fdecl;

      // make a function call expression
      ret = AST_EXPRESSION_init( S_FUN_CALL, S_VAR_ANY, &cur->loc  ); 
      ret->val.fcall = fcall;
    
    } else {
      ret = (AST_EXPRESSION *) base;
    }
 
  } else {
    ret = AST_EXPRESSION_init( S_EXPR_CONSTANT, S_VAR_STRING, &cur->loc ); 
    ret->val.const_value.string_value = strdup( STRING_PART_get( cur ) );
  }
  return ret;
}

 
AST_EXPRESSION *AST_compile_multi_part_string( PARSECONTEXT *pc )
{
  AST_EXPRESSION *lhs,*rhs; 
  size_t pos;

  lhs = 0;
  pos = 0;

  while( 1 ) {
    if (lhs == 0) {
      lhs = AST_compile_string_part( pc, pos ++ );
      if (!lhs) {
        return 0;
      }
    }
    rhs = AST_compile_string_part( pc, pos ++ );
    if (!rhs) {
      break;
    }

    lhs = AST_EXPRESSION_init_binary( TK_OP_STR_CAT, lhs, rhs );
    lhs->value_type = S_VAR_STRING;  
  }

  return lhs;
}

int is_operator_boolean( int op )
{
  return op == TK_OP_LOGICAL_AND || TK_OP_LOGICAL_OR;
 
}
int is_operator_with_boolean_result( int op )
{
  switch(op) {
    case TK_OP_NUM_LT: 
    case TK_OP_NUM_GT: 
    case TK_OP_NUM_LE: 
    case TK_OP_NUM_GE:
    case TK_OP_NUM_EQ:
    case TK_OP_NUM_NE:
    case TK_OP_STR_EQ:
    case TK_OP_STR_NE: 
    case TK_OP_LOGICAL_AND:
    case TK_OP_LOGICAL_OR:
       return 1;
    default:
      return 0;
  }
}

int is_operator_with_number_args( int op )
{
  switch(op) {
    case TK_OP_NUM_SUBST:
    case TK_OP_NUM_ADD:
    case TK_OP_NUM_DIV:
    case TK_OP_NUM_MULT:
    case TK_OP_NUM_MOD:
    case TK_OP_NUM_POW:
    case TK_OP_NUM_LT: 
    case TK_OP_NUM_GT: 
    case TK_OP_NUM_LE: 
    case TK_OP_NUM_GE:
    case TK_OP_NUM_EQ:
    case TK_OP_NUM_NE:
 // case TK_OP_STR_EQ:
 // case TK_OP_STR_NE: 
    case TK_OP_LOGICAL_AND:
    case TK_OP_LOGICAL_OR:
           return 1;
    default:
      return 0;
  }
}


const char *operator_name( int op )
{
  switch(op) {
    case TK_OP_NUM_SUBST:
      return "-";
    case TK_OP_NUM_ADD:
      return "+";
    case TK_OP_NUM_DIV:
      return "/";
    case TK_OP_NUM_MULT:
      return "*";
    case TK_OP_NUM_MOD:
      return "%";

    case TK_OP_NUM_EQ: 
      return "==";
    case TK_OP_NUM_NE: 
      return "!=";
    case TK_OP_NUM_LT: 
      return "<";
    case TK_OP_NUM_GT: 
      return ">";
    case TK_OP_NUM_LE: 
      return "<=";
    case TK_OP_NUM_GE:
      return ">=";

    case TK_OP_STR_CAT:  
      return "..";
    
    case TK_OP_STR_EQ:
      return "eq";
    case TK_OP_STR_NE: 
      return "ne";
    case TK_OP_STR_LT: 
      return "lt";
    case TK_OP_STR_GT: 
      return "gt";
    case TK_OP_STR_LE: 
      return "le";
    case TK_OP_STR_GE:    
      return "ge";
  
    case TK_OP_LOGICAL_AND:
      return "and";
    case TK_OP_LOGICAL_OR:
      return "or";
    case TK_OP_LOGICAL_NEGATE:
      return "not";

    default:
      return "";


   }
}

int is_comparison_operator( int op )
{
  switch(op) {
    case TK_OP_NUM_EQ: 
    case TK_OP_NUM_NE: 
    case TK_OP_NUM_LT: 
    case TK_OP_NUM_GT: 
    case TK_OP_NUM_LE: 
    case TK_OP_NUM_GE:

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

// operator types must be numbers
int is_numeric_operator( int op )
{
  switch(op) {
    case TK_OP_NUM_SUBST:
    case TK_OP_NUM_ADD:
    case TK_OP_NUM_DIV:
    case TK_OP_NUM_MULT:
    case TK_OP_NUM_POW:

   case TK_OP_NUM_MOD:

    case TK_OP_NUM_EQ: 
    case TK_OP_NUM_NE: 
    case TK_OP_NUM_LT: 
    case TK_OP_NUM_GT: 
    case TK_OP_NUM_LE: 
    case TK_OP_NUM_GE: 

    case TK_OP_LOGICAL_AND:
    case TK_OP_LOGICAL_OR:

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

int AST_EXPRESSION_binary_op_check_types( PARSECONTEXT *parse_context, AST_EXPRESSION *scl )
{
    int op;
    AST_EXPRESSION *lhs;
    AST_EXPRESSION *rhs;  
    AST_VAR_TYPE lhs_type, rhs_type;  
    
    
    op = scl->val.expr.op;
  
    // string concatenation allows for any types.
    if (op == TK_OP_STR_CAT ) {
      scl->value_type = S_VAR_STRING; // always produces a string.
      return 0;
    }
    if (op ==  TK_HASH_IT) {
      return 0;
    }

    lhs = scl->val.expr.expr_left;
    rhs = scl->val.expr.expr_right;

    lhs_type = AST_EXPRESSION_type( lhs );
    rhs_type = AST_EXPRESSION_type( rhs );
    
    if (is_numeric_operator( op ) ) {	    // operator returns a number as value, and expects numeric arguments.

      // expception: one can compare anything with Null
      if (op == TK_OP_NUM_NE || op == TK_OP_NUM_EQ) {
	if (lhs_type == S_VAR_NULL || rhs_type == S_VAR_NULL) {
	  return 0;
	}
      }
 
#ifdef ONLY_NUMBERS_IN_ARITHMETIC_OPERATORS
     //if ( lhs_type != S_VAR_ANY && ! is_numeric_type( lhs_type ) ) { 
       if ( ! is_numeric_type( lhs_type ) ) { 

	  if (parse_context) {
	    do_yyerror( &lhs->base.location, parse_context, "Operator %s expects number argument , instead got %s", operator_name( op ), get_type_name( lhs_type ) );   
	  }
          return -1;
       }

     //if ( rhs_type != S_VAR_ANY && ! is_numeric_type( rhs_type ) ) {
       if ( ! is_numeric_type( rhs_type ) ) {
          if (parse_context) {
	    do_yyerror( &rhs->base.location, parse_context, "Operator %s expects number argument instead got %s", operator_name( op ), get_type_name( rhs_type )  );   
          }
	  return -1;
       }
#else
       if ( ! is_scalar( lhs_type ) ) { 

	  if (parse_context) {
	    do_yyerror( &lhs->base.location, parse_context, "Operator %s expects number or string argument , instead got %s", operator_name( op ), get_type_name( lhs_type ) );   
	  }
          return -1;
       }

     //if ( rhs_type != S_VAR_ANY && ! is_numeric_type( rhs_type ) ) {
       if ( ! is_scalar( rhs_type ) ) {
          if (parse_context) {
	    do_yyerror( &rhs->base.location, parse_context, "Operator %s expects number or string argument instead got %s", operator_name( op ), get_type_name( rhs_type )  );   
          }
	  return -1;
       }
#endif
       if (is_comparison_operator( op ) ) {
         scl->value_type = S_VAR_INT;
       } else {
	 if (rhs_type == S_VAR_DOUBLE ||  lhs_type == S_VAR_DOUBLE) {
	   scl->value_type = S_VAR_DOUBLE;
	 } else {
	   scl->value_type = S_VAR_INT;
	 }
       }
        
       // arguments.
       if (lhs->exp_type  == S_EXPR_REFERENCE && ! is_indexed_ref(lhs)) {
         if ( rhs_type == S_VAR_ANY ) { //is_narrower_type( lhs_type, S_VAR_NUMBER )) {
           assign_value( lhs, S_VAR_NUMBER, &lhs->base.location );
         }
       }
 
       if (rhs->exp_type == S_EXPR_REFERENCE && ! is_indexed_ref(rhs)) {
         if ( rhs_type == S_VAR_ANY ) { //is_narrower_type( rhs_type, S_VAR_NUMBER )) {
           assign_value( rhs, S_VAR_NUMBER, &rhs->base.location );
         }
       }
    }
    else 
    if (is_string_operator( op ) ) {
      if ( lhs_type != S_VAR_ANY && ! is_numeric_or_string_type( lhs_type ) ) {
          if (parse_context) {
	    do_yyerror( &lhs->base.location, parse_context, "Operator %s expects integer or string type, instead got %s", operator_name( op ),  get_type_name( lhs_type) );   
          }
	  return -1;
      }
      
      if ( rhs_type != S_VAR_ANY && ! is_numeric_or_string_type( rhs_type ) ) {
          if (parse_context) {
	    do_yyerror( &rhs->base.location, parse_context, "Operator %s expects integer or string type, instead got %s", operator_name( op ), get_type_name( rhs_type) );   
          }
	  return -1;
      }

      if (is_comparison_operator( op ) ) {
         scl->value_type = S_VAR_INT;
      } else {
         scl->value_type = S_VAR_STRING;
      }

      if (lhs->exp_type == S_EXPR_REFERENCE && ! is_indexed_ref(lhs)) {
         if (is_narrower_type( lhs_type, S_VAR_NUMBER )) {
           assign_value( lhs, S_VAR_SCALAR, &lhs->base.location ) ;
         }  
      }
 
      if (rhs->exp_type == S_EXPR_REFERENCE && ! is_indexed_ref(rhs)) {
         if (is_narrower_type( rhs->exp_type, S_VAR_NUMBER )) {
           assign_value( rhs, S_VAR_SCALAR, &rhs->base.location );
         }  
      }


    } else {
      if (parse_context) {
        do_yyerror( &scl->base.location, parse_context, "Unknown binary operator %s in expression", operator_name( op ));   
      }
      return -1;
    }
  
    // if division - check that divisor is not constant with value 0
    if (op == TK_OP_NUM_DIV && rhs->exp_type == S_EXPR_CONSTANT) {
	if ((rhs_type == S_VAR_INT && rhs->val.const_value.long_value == 0) || 
	    (rhs_type == S_VAR_DOUBLE && rhs->val.const_value.double_value == 0))  {
	    
	    // divide by zero error, still the types are in agreement
            if (parse_context) {
	      do_yyerror( &rhs->base.location, parse_context, "Can't divide by zero - right hand side evaluated to zero" );
	    }
	    return 0;
	}
    }
 
   return 0;
}


int AST_EXPRESSION_unary_op_check_types( PARSECONTEXT *parse_context, AST_EXPRESSION *scl )
{
    int op = scl->val.unary.op;
    AST_EXPRESSION *arg = scl->val.unary.expr;

    switch( op ) {
      case TK_OP_NUM_ADD:
      case TK_OP_NUM_SUBST:
      case TK_OP_LOGICAL_NEGATE: 
	break;	
      default:
        if (parse_context) {
          do_yyerror( &scl->base.location, parse_context, "Unknown unary operator %s in expression", operator_name( op ));   
        }
        break;
    }

    if ( arg->value_type != S_VAR_ANY && ! is_numeric_type( arg->value_type ) ) { 
          if (parse_context) {
	    do_yyerror( &arg->base.location, parse_context, "Operator %s expects integer type as argument, instead got %s", operator_name( op ), get_type_name( arg->value_type) );   
	  }
          return -1;
    }	
    return 0;
}

int AST_EXPRESSION_unary_fold_constants( AST_EXPRESSION *scl)
{
  int op = scl->val.unary.op;
  AST_EXPRESSION *arg = scl->val.unary.expr;
  long res_num = 0 ,arg_num;


   if (arg->exp_type != S_EXPR_CONSTANT || arg->exp_type != S_EXPR_CONSTANT) {
      return -1;
   }
   if (AST_EXPRESSION_unary_op_check_types( 0, scl )) {
     return -1;
   }

   arg_num = arg->value_type == S_VAR_DOUBLE ? arg->val.const_value.double_value : (double) arg->val.const_value.long_value;

   switch( op ) {
      case TK_OP_NUM_ADD:
       res_num =  arg_num;
       break;
      case TK_OP_NUM_SUBST:
        res_num =  - arg_num;
      case TK_OP_LOGICAL_NEGATE: 
        res_num = arg_num != 0;
	break;	
    }
    scl->exp_type = S_EXPR_CONSTANT;
    scl->value_type = S_VAR_INT;
    scl->val.const_value.long_value = res_num;
    
    return 0;
}

struct {
  unsigned  int mask;
  const char *name;
} TypeMasks[] = {
  { S_VAR_SCALAR, "scalar" },
  { S_VAR_INT | S_VAR_DOUBLE, "number" },
  { S_VAR_STRING, "string" },
  { S_VAR_CODE, "function" },
  { S_VAR_HASH, "table" },
  { S_VAR_LIST, "array" },
  { S_VAR_NULL, "Null" },
  { -1, 0 }
};

const char *get_type_name_r( AST_VAR_TYPE value_type, char *rval)
{
  int i = 0, vadded = 0, res;
  if (value_type == S_VAR_ANY) {
    return "any type";
  }
  strcpy(rval,"");
  for(i=0; TypeMasks[i].mask != (unsigned int) -1; i++ ) {
    if (i != 1) 
      res = (value_type & TypeMasks[i].mask) == TypeMasks[i].mask;
    else 
      res = (value_type & TypeMasks[i].mask);
    

    if (res) {
        if (vadded) {
	  strcat( rval, " , " );
	}
	strcat( rval, TypeMasks[i].name);
	value_type &= ~ TypeMasks[i].mask;
	vadded ++;
    }
  }
  return rval;
}

const char *get_type_name( AST_VAR_TYPE value_type)
{
  static char rval[ 256 ];

  return get_type_name_r( value_type, rval );
}

const char *get_type_name2( AST_VAR_TYPE value_type)
{
  static char rval[ 256 ];

  return get_type_name_r( value_type, rval );
}


const char *get_op_name( int op )
{
  switch(op) {
    case TK_OP_NUM_SUBST:
	return "-";
    case TK_OP_NUM_ADD:
	return "+";
    case TK_OP_NUM_DIV:
	return "/";
    case TK_OP_NUM_MULT:
	return "*";
    case TK_OP_NUM_MOD:
	return "%";
    case TK_OP_NUM_POW:
	return "^";
     case TK_OP_NUM_LT: 
	return "<";
    case TK_OP_NUM_GT: 
	return ">";
    case TK_OP_NUM_EQ: 
	return "==";
    case TK_OP_NUM_NE: 
	return "!=";
     case TK_OP_NUM_LE: 
	return "<=";
    case TK_OP_NUM_GE:
	return ">=";
    case TK_OP_STR_EQ:
	return "eq";
    case TK_OP_STR_NE: 
	return "ne";
    case TK_OP_STR_LT: 
	return "lt";
    case TK_OP_STR_GT: 
	return "gt";
    case TK_OP_STR_LE: 
	return "le";
    case TK_OP_STR_GE:
	return "ge";
    case TK_OP_STR_CAT:
	return "..";
    case TK_OP_LOGICAL_AND:
        return "and";
    case TK_OP_LOGICAL_OR:
        return "or";
    case TK_OP_LOGICAL_NEGATE:
        return "not";
    case TK_HASH_IT:
        return "cons";
    default:
	assert(0);
        return "<unkown>";
  }
}

#if 0
char *convert_to_string(AST_VAR_TYPE ty, Simple_value_type val)
{
  char fmt_buf[ 60 ];
  switch(ty) {
    case S_VAR_INT: 
      sprintf( fmt_buf, "%ld", val.long_value );
      return strdup( fmt_buf );
    case S_VAR_DOUBLE:
      sprintf( fmt_buf, "%f", val.double_value );
      return strdup( fmt_buf );
    case S_VAR_STRING:
      return strdup( val.string_value );
    default:
      return 0;
  }
}

int AST_EXPRESSION_binary_fold_constants( AST_EXPRESSION *scl )
{
    int op;
    AST_EXPRESSION *lhs;
    AST_EXPRESSION *rhs;  
    
    
    op = scl->val.expr.op;
    lhs = scl->val.expr.expr_left;
    rhs = scl->val.expr.expr_right;


    if (lhs->exp_type != S_EXPR_CONSTANT || rhs->exp_type != S_EXPR_CONSTANT) {
      return -1;
    }

    if ( AST_EXPRESSION_binary_op_check_types( 0, scl ) ) {
      return -1;
    }

    if (is_numeric_operator( op ) ) {
       if (lhs->value_type == S_VAR_DOUBLE || rhs->value_type == S_VAR_DOUBLE) {
         double res_num, rhs_num,lhs_num;

	 lhs_num = lhs->value_type == S_VAR_DOUBLE ? lhs->val.const_value.double_value : (double) lhs->val.const_value.long_value;
	 rhs_num =  rhs->value_type == S_VAR_DOUBLE ? rhs->val.const_value.double_value : (double) rhs->val.const_value.long_value;

	 switch(op) {
	    case TK_OP_NUM_SUBST:
		res_num = lhs_num - rhs_num;
		break;
	    case TK_OP_NUM_ADD:
		res_num = lhs_num + rhs_num;
		break;
	    case TK_OP_NUM_DIV:
		res_num = lhs_num / rhs_num;
		break;
	    case TK_OP_NUM_MULT:
		res_num = lhs_num * rhs_num;
		break;
	    case TK_OP_NUM_MOD:
		res_num = (long) lhs_num % (long) rhs_num;
		break;
	    case TK_OP_NUM_EQ: 
		res_num = lhs_num == rhs_num;
		break;
	    case TK_OP_NUM_NE: 
		res_num = lhs_num != rhs_num;
		break;
	    case TK_OP_NUM_LT: 
		res_num = lhs_num < rhs_num;
		break;
	    case TK_OP_NUM_GT: 
		res_num = lhs_num > rhs_num;
		break;
	    case TK_OP_NUM_LE: 
		res_num = lhs_num <= rhs_num;
		break;
	    case TK_OP_NUM_GE: 
  		res_num = lhs_num >= rhs_num;
		break;

	    case TK_OP_LOGICAL_AND:
		res_num = lhs_num && rhs_num;
		break;
	    case TK_OP_LOGICAL_OR:
		res_num = lhs_num || rhs_num;
		break;

	    default:
		;
	}

	scl->exp_type = S_EXPR_CONSTANT;
	scl->value_type = S_VAR_DOUBLE;
	scl->val.const_value.double_value = res_num;

       } else {
         long res_num,rhs_num,lhs_num;

	 lhs_num = lhs->val.const_value.long_value;
	 rhs_num = rhs->val.const_value.long_value;

	 switch(op) {
	    case TK_OP_NUM_SUBST:
		res_num = lhs_num - rhs_num;
		break;
	    case TK_OP_NUM_ADD:
		res_num = lhs_num + rhs_num;
		break;
	    case TK_OP_NUM_DIV:
		res_num = lhs_num / rhs_num;
		break;
	    case TK_OP_NUM_MULT:
		res_num = lhs_num * rhs_num;
		break;
	    case TK_OP_NUM_MOD:
		res_num = lhs_num % rhs_num;
		break;
	    case TK_OP_NUM_EQ: 
		res_num = lhs_num == rhs_num;
		break;
	    case TK_OP_NUM_NE: 
		res_num = lhs_num != rhs_num;
		break;
	    case TK_OP_NUM_LT: 
		res_num = lhs_num < rhs_num;
		break;
	    case TK_OP_NUM_GT: 
		res_num = lhs_num > rhs_num;
		break;
	    case TK_OP_NUM_LE: 
		res_num = lhs_num <= rhs_num;
		break;
	    case TK_OP_NUM_GE: 
  		res_num = lhs_num >= rhs_num;
		break;

	    case TK_OP_LOGICAL_AND:
		res_num = lhs_num && rhs_num;
		break;
	    case TK_OP_LOGICAL_OR:
		res_num = lhs_num || rhs_num;
		break;

	    default:
		;
	}

	scl->exp_type = S_EXPR_CONSTANT;
	scl->value_type = S_VAR_INT;
	scl->val.const_value.long_value = res_num;
       }
    
    } else  if (is_string_operator( op ) ) {
      char * rhs_str, *lhs_str;
      int free_rhs = 0, free_lhs = 0;

      if ( lhs->value_type == S_VAR_STRING) {
        lhs_str = lhs->val.const_value.string_value;
      } else {
	lhs_str = convert_to_string( lhs->value_type, lhs->val.const_value );
	free_lhs = 1;
      }

      if ( rhs->value_type == S_VAR_STRING) {
        rhs_str = rhs->val.const_value.string_value;
      } else {
	rhs_str = convert_to_string( rhs->value_type, rhs->val.const_value );
	free_rhs = 1;
      }

      	
      if (op == TK_OP_STR_CAT) {
	char *res_str;

	res_str = malloc( strlen(rhs_str) + strlen(lhs_str) + 1 );

	strcpy( res_str, lhs_str );
	strcat( res_str, rhs_str );

	scl->exp_type = S_EXPR_CONSTANT;
	scl->value_type = S_VAR_STRING;
	scl->val.const_value.string_value = res_str;
      } else {
	  int res_num;

	  switch( op ) {
	    case TK_OP_STR_EQ:
	      res_num = strcmp( lhs_str, rhs_str ) == 0;
	      break;  
	    case TK_OP_STR_NE: 
	      res_num = strcmp( lhs_str, rhs_str ) != 0;
	      break;  
	    case TK_OP_STR_LT: 
	      res_num = strcmp( lhs_str, rhs_str ) < 0;
	      break;  
	    case TK_OP_STR_GT: 
	      res_num = strcmp( lhs_str, rhs_str ) > 0;
	      break;  
	    case TK_OP_STR_LE: 
	      res_num = strcmp( lhs_str, rhs_str ) <= 0;
	      break;  
	    case TK_OP_STR_GE:           
	      res_num = strcmp( lhs_str, rhs_str ) >= 0;
	      break;  
	    default:
	      return 0;
	  }

	  scl->exp_type = S_EXPR_CONSTANT;
	  scl->value_type = S_VAR_INT;
	  scl->val.const_value.long_value = res_num;
       }
       if (free_lhs) {
         free( lhs_str );
       }
       if (free_rhs) {
         free( rhs_str );
       }
    }    

    scl->base.location.first_line = lhs->base.location.first_line;
    scl->base.location.first_column = lhs->base.location.first_column;
    scl->base.location.last_line = rhs->base.location.last_line;
    scl->base.location.last_column = rhs->base.location.last_column;
    
    return 0;
}
#endif

