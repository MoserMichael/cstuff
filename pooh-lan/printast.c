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
	    fprintf( out, "%ld", expr->val.const_value.long_value );
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

}

void AST_print( FILE *out, AST_BASE *base )
{
  size_t i;
  int newline = 0;

  fflush( out );
  assert(base );

  fprintf( out, "{%d,%d-%d,%d} ", base->location.first_line, base->location.first_column, base->location.last_line, base->location.last_column );
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
    fprintf( out, "%s , ", scl->type == CP_VALUE ? "assign-copy" : "assign-ref" );
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
   } else {
      assert( cond->block == 0 );
      assert( cond->elsecond != 0 ); 
      
      fprintf( out , "else , " );
      AST_print( out, (AST_BASE *) cond->elsecond  );
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
  default:
    fflush(out);
    assert(0);
  }
  fprintf( out, ")" );
  if (newline) {
    fprintf( out, "\n" );
  }
   
}
