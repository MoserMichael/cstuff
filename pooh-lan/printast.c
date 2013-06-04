#include "printast.h"

void AST_print_expr( FILE *out, AST_EXPRESSION *expr)
{ 
  switch( expr->exp_type ) 
  {
    case S_EXPR_BINARY:
      fprintf(out, "binop-%s , ", get_op_name( expr->val.expr.op  ) ); 
      AST_print( out, (AST_BASE *) expr->val.expr.expr_left );
      fprintf( out, " , " );
      AST_print( out, (AST_BASE *) expr->val.expr.expr_right );
      break;

    case S_EXPR_UNARY:
      fprintf(out, "unop-%s , ", get_op_name( expr->val.unary.op  ) ); 
      AST_print( out, (AST_BASE *) expr->val.unary.expr );
      break;

    case S_EXPR_REFERENCE:
      fprintf(out, "ref , %s", expr->val.ref.lhs ); 
      if (expr->val.ref.indexes ) {
         fprintf( out, " , " );
         AST_print( out, (AST_BASE *) expr->val.ref.indexes );
      }
      break;

    case S_EXPR_CONSTANT:
      fprintf( out, "const-%s , " , get_type_name( expr->value_type) );
      switch( expr->value_type )
      {
        case S_VAR_INT:
#if POOH_INT_SIZE == 4    
	    fprintf( out, "%ld", expr->val.const_value.long_value );
#else
	    fprintf( out, "%lld", expr->val.const_value.long_value );
#endif	    
	    break;
	case S_VAR_DOUBLE:
	    fprintf( out, "%f", expr->val.const_value.double_value );
	    break;
	case S_VAR_STRING:
	    fprintf( out, "\"%s\"", expr->val.const_value.string_value );
	    break;
	case S_VAR_NULL:
	    fprintf( out, "Null" );
	    break;
        case S_VAR_GRAMMAR:
	    fprintf( out, "Grammar: " );
            AST_print( out, expr->val.const_value.grammar_value );
            break;
	default:
	    assert(0);
      }
      break;

    case S_EXPR_FUNCALL:
      fprintf( out, "expr-funcall , " );
      AST_print( out, (AST_BASE *) expr->val.fcall );
      break;

    case S_EXPR_HASH_INDEX:
      fprintf( out, "hash-idx , ");
      AST_print( out, (AST_BASE *) expr->val.index_expr );
      break;
  
    case S_EXPR_ARRAY_INDEX:
      fprintf( out, "array-idx , ");
      AST_print( out, (AST_BASE *) expr->val.index_expr );
      break;
  
    case S_EXPR_LIST_VALUES:
      fprintf(out, "list-constructor , " ); 
      AST_print( out, (AST_BASE *) expr->val.index_expressions );
      break;
 
    case S_EXPR_HASH_VALUES:
      fprintf(out, "hash-constructor , " ); 
      AST_print( out, (AST_BASE *) expr->val.index_expressions );
      break;

    case S_EXPR_PLACEHOLDER:
      fprintf( out, "_" );
      break;

    case  S_EXPR_LAMBDA:
      fprintf(out, "anonymous-func , " ); 
      AST_print( out, (AST_BASE *) expr->val.fdecl );
      break;

    case S_EXPR_LAMBDA_RESOLVED: // ref to named function after lookup (avoid recursion trick)
      fprintf(out, "ref-to-named-function-part-of-fcall , %s", ((AST_FUNC_DECL *) expr->val.fdecl)->f_name );
      break;
    
    case S_EXPR_LAMBDA_RESOLVED_REF: // ref to named function after lookup (avoid recursion trick)
      fprintf(out, "ref-to-named-function-not-part-of-fcall , %s", ((AST_FUNC_DECL *) expr->val.fdecl)->f_name );
      break;

     case S_EXPR_ERROR:
      fprintf(out, "error-expression , " ); 
      break;

    default:
      fflush(out);
      assert(0);
  }

  fprintf( out, " , exp-type 0x%x", expr->exp_type );

}


void print_pp_base( FILE *out, AST_PP_BASE *scl)
{
    if (scl->from != scl->to && scl->from != 1 ) {
        
	fprintf( out, " %d", scl->from );
	if (scl->to != -1) {
	    fprintf( out, "-%d", scl->to );
	} else {
	    fprintf( out, "-infinity-and-beyond" );
	}
    }
}

void AST_print( FILE *out, AST_BASE *base )
{
  size_t i;
  int newline = 0;

  fflush( out );
  assert(base );

#if 0
  fprintf( out, "{%d,%d-%d,%d} ", base->location.first_line, base->location.first_column, base->location.last_line, base->location.last_column );
#endif 
  fprintf( out, "(" );
  
  switch( base->type ) {
  case S_EXPRESSION:
    AST_print_expr( out, (AST_EXPRESSION *) base );
    break;

  case S_AST_LIST: {
    AST_BASE_LIST *scl  = (AST_BASE_LIST *) base;
    DRING *cur;

    fprintf( out, "stmt-list" );

    DRING_FOREACH( cur, &scl->statements ) {
	fprintf(out, " , " );
	AST_print( out,  _OFFSETOF( cur, AST_BASE, entry ) );
    }
    }
    break;

  case S_AST_VECTOR: {
    AST_VECTOR *vect = (AST_VECTOR *)base;

    fprintf( out, "stmt-vector" );

    for( i = 0; i < AST_VECTOR_size( vect ); i++) {
	fprintf(out, " , " );
	AST_print( out, AST_VECTOR_get( vect, i ));
    }
    }
    break;
  
  case S_ASSIGNMENT:
  {
    AST_ASSIGNMENT *scl = (AST_ASSIGNMENT *) base;
  
    switch( scl->type ) {
      case CP_VALUE:
        fprintf( out, "%s , ", "assign-copy");
        break;
      case CP_REF:
        fprintf( out, "%s , ", "assign-ref");
        break;
      case CP_MOVE:
        fprintf( out, "%s , ", "assign-move");
        break;
    }
    AST_print( out, (AST_BASE *) scl->left_side );
    fprintf( out, " , ");
    AST_print( out, (AST_BASE *) scl->right_side );
    newline = 1;
    }
    break;
  
  case S_IF: {
    AST_COND *cond = (AST_COND *) base;
    
    if (cond->condition) {  
      fprintf( out, "cond-IF , " );
      AST_print( out, (AST_BASE *) cond->condition );
      fprintf( out, " , " );
      AST_print( out, (AST_BASE *) cond->block );
      fprintf( out, " , " );

      if (cond->elsecond) {
	 AST_print( out, (AST_BASE *) cond->elsecond );
      }
   } else {
      assert( cond->block != 0 );
      assert( cond->elsecond == 0 ); 
      
      fprintf( out , "else , " );
      AST_print( out, (AST_BASE *) cond->block  );
    }
#if 0
    if (cond->condition) {  
      fprintf( out, "cond-IF , " );
      AST_print( out, (AST_BASE *) cond->condition );
      fprintf( out, " , " );
      AST_print( out, (AST_BASE *) cond->block );
      fprintf( out, " , " );
   } else {
      assert( cond->block );
      AST_print( out, (AST_BASE *) cond->block );
      assert( cond->elsecond == 0 ); 
   }
   if (cond->elsecond ) {
      fprintf( out , "else , " );
      AST_print( out, (AST_BASE *) cond->elsecond  );
    }
#endif    
    newline = 1;
    }
    break;
 
  case S_WHILE: {
    AST_WHILE_LOOP *scl = (AST_WHILE_LOOP *) base;
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
    fprintf( out, "%s , ", lp);

    if (scl->condition) {
      AST_print( out, (AST_BASE *) scl->condition );
      fprintf( out, " , ");
    }

    AST_print( out, (AST_BASE *) scl->block );
    newline = 1;
    }
    break;

  case S_FOR:  {
    AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
    fprintf(out,"for , ");
    AST_print( out, (AST_BASE *) scl->loop_var );
    fprintf( out, " , ");
    AST_print( out, (AST_BASE *) scl->loop_expr );
    fprintf( out, " , ");
    AST_print( out, (AST_BASE *) scl->block );
    newline = 1;
    }
    break;
  
  case S_FUN_CALL_PARAM: {
    AST_FUNC_CALL_PARAM *scl = (AST_FUNC_CALL_PARAM *) base;
    fprintf( out, "func-param, %s , ", scl->label_name );
    AST_print( out, (AST_BASE *) scl->expr );
    }
    break;

  case S_FUN_CALL: {
    AST_FUNC_CALL *scl = (AST_FUNC_CALL *) base;
    fprintf( out, "fun-call, " );
    
    AST_print( out, (AST_BASE *) scl->f_name );
    fprintf( out, " , " );
    AST_print( out, (AST_BASE *) scl->call_params );
    newline = 1;
    }
    break;

  case S_FUN_DECL: {
    AST_FUNC_DECL *scl = (AST_FUNC_DECL *) base;
    fprintf( out, "fun-decl , %s , ", scl->f_name );
    if (scl->func_params != 0) {
      AST_print ( out, (AST_BASE *) scl->func_params );
      fprintf( out, " ,\n" );
    }
    AST_print ( out, (AST_BASE *) scl->func_body );
    newline = 1;
    }
    break;
  
  case S_XFUN_DECL: {
    AST_XFUNC_DECL *scl = (AST_XFUNC_DECL *) base;

    fprintf( out, "xfun-decl , %s , " , scl->f_name );

    for(i = 0; i < scl->nparams; i++) {
	fprintf(out, "( func-param , %s ) ", scl->params[ i ] . param_name );
    }
    newline = 1;
    }
    break;
  
  case S_RETURN: {
    AST_RETURN *scl = (AST_RETURN *) base;
    fprintf( out, "return" );
    if (scl->rvalue) {
      fprintf( out, " , " );
      AST_print( out, (AST_BASE *) scl->rvalue );
    }
    }
    break;

  case S_NEXT:
    fprintf( out, "next");
    break;
  
  case S_BREAK:
    fprintf( out, "break" );
    break;
  
  case S_NULL:
    break;

  case S_PP_RULE: {
    AST_PP_RULE *scl = (AST_PP_RULE *) base;  
    fprintf( out,"rule rule-name=%s , ", scl->rule_name );
    AST_print( out, (AST_BASE *) &scl->rhs );
    print_pp_base( out, &scl->base );
    }
    break;

  case S_PP_ALTERNATIVE: {
    AST_PP_ALTERNATIVE *scl = (AST_PP_ALTERNATIVE *) base; 
    DRING *cur;
    AST_BASE *ccur;
    i = 0;

    fprintf( out, "alternative ( " );
    DRING_FOREACH( cur, &scl->alternatives ) {
	
	if (i) {
	    fprintf( out, " | " );
	}

	ccur = _OFFSETOF( cur, AST_BASE, entry ); 
	AST_print( out, ccur );
	++i;
    }
    fprintf( out, " )" );
    print_pp_base( out, &scl->base );
    }
    break;

  case S_PP_RULE_REF: {
    AST_PP_RULE_REF *scl = (AST_PP_RULE_REF *) base;
    fprintf( out, "rule-ref <%s>", scl->rule_name );
    print_pp_base(  out, &scl->base );
    }
    break;
  
  case S_PP_RULE_CONSTANT: {
    AST_PP_CONSTANT *scl = (AST_PP_CONSTANT *) base;

    fprintf( out, "'%s'", scl->const_string );
    print_pp_base(  out, &scl->base );
  }
    break;

  case S_PP_CHAR_CLASS: {
    AST_PP_CHAR_CLASS *scl = (AST_PP_CHAR_CLASS *) base;  
    AST_PP_RANGE *cur;

    fprintf( out, "/" );
    for( i = 0; i < ARRAY_size( &scl->ranges ); i++ ) {
	cur = (AST_PP_RANGE *) ARRAY_at( &scl->ranges, i ); 
	if (cur->to == (uint32_t) -1 ) {
	    fprintf(out, "%c", (char) cur->from );
	} else {
	    fprintf(out, "%c-%c", (char) cur->from, (char) cur->to );
	}
    }
    fprintf( out, "/" );

    print_pp_base(  out, &scl->base );
  }
    break;

  case S_PP_SCRIPT: {
    AST_PP_SCRIPT *scl = (AST_PP_SCRIPT *) base;
    AST_FUNC_CALL *fcall = (AST_FUNC_CALL *) scl->rule_script;

    fprintf( out, "[script], " );
    AST_print( out, (AST_BASE *) scl->rule_script );
    AST_print( out, (AST_BASE *) fcall->func_decl );

  }
    break;

  default:
    fflush(out);
    assert(0);
  }
  fprintf( out, ")" );
  if (newline) {
    fprintf( out, "\n" );
  }
   
}
