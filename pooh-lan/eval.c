
  

int EVAL_expression( PARSECONTEXT *ctx, EVAL_THREAD *thread, AST_EXPRESSION *expr)
{
    switch( expr->exp_type )
    {
      case S_EXPR_CONSTANT:
         // push constant value onto stack
	 break;

      case S_EXPR_BINARY: {
	    EVAL_expression( ctx, thread, expr->val.expr.expr_left );
	    EVAL_expression( ctx, thread, expr->val.expr.expr_right );

	    // now operands are on the stack; remove top two entries, perform operation and push the result.
         }
	 break;

      case S_EXPR_UNARY: {
	    EVAL_expression( ctx, thread, expr->val.unary.expr );

	    // perform operation and replace top of the stack with result.
         }
	 break;

      case S_EXPR_REFERENCE: {

        } 
	break;

      case S_EXPR_FUNCALL: {
        }
	break;

      default:
	assert(0);
    }
}

int EVAL_boolean_condition( PARSECONTEXT *ctx, EVAL_THREAD *thread, AST_EXPRESSION *expr)
{
    BINDING_DATA *data;
 
    EVAL_expression( ctx, thread, cond->condition );

    data = EVAL_THREAD_stack_at( thread, 0 );

    if (data->value_type == S_VAR_INT) {
	return data->value.long_value == 0;
    } else if (data->value_type == S_VAR_LONG) {
	return data->value.double_value == 0;
    } else {
	do_yyerror( &cond->base.location, ctx, "Numeric expression expected, instead got %s", get_type_name( data->value_type ) );
	exit(1);
    }
    return -1;
}    

#define STMT_LIST_RET_NEXT  1
#define STMT_LIST_RET_BREAK 2
#define STMT_LIST_RET_RETURN  3

int EVAL_statement_list( PARSECONTEXT *ctx, EVAL_THREAD *thread, AST_BASE_LIST *slist)
{
  // evaluate function body
  DRING_FOREACH( tmp, &slist->statements ) {
    base = _OFFSETOF( tmp, AST_BASE, entry );
    switch( base->type ) {
      case S_ASSIGNMENT: {
         AST_ASSIGNMENT *ass = (AST_ASSIGNMENT *) base;
	 BINDING_DATA *data;
 
	 EVAL_expression( ctx, thread, ass->right_side );
	 
	 data = EVAL_THREAD_stack_at( thread, 0 );

	 }
	 break; 
      case S_IF: {
	    AST_COND *cond = (AST_COND *) base;
	    
	    do {
		if (EVAL_boolean_condition( ctx, thread, cond->condition )) {
		  EVAL_statement_list( ctx, thread, &cond->block );
		  break;
		} else {
		  cond = cond->elsecond;
		}  
	    } while( cond != 0 );
	 }
	 break; 
      case S_WHILE: {
         AST_WHILE_LOOP *loop = (AST_WHILE_LOOP *) base;
         while( 1 ) {

	    if (!loop->post_condition) {
	        if (!EVAL_boolean_condition( ctx, thread, loop->condition )) {
	          break;
		}   	 
	    }
	    if (EVAL_statement_list( ctx, thread, &loop->block) >= STMT_LIST_RET_BREAK) {
	      break;
	    }
	    if (loop->post_condition) {
	        if (!EVAL_boolean_condition( ctx, thread, loop->condition )) {
	          break;
		}   	 
	    }
	 }
	 }
	 break; 
      case S_FOR:
	 break; 
      case S_FUN_CALL: {
	 BINDING_DATA data;
 
	 AST_FUNC_CALL *fcall = (AST_FUNC_CALL *) tmp;
         EVAL_function_call( ctx, thread, fcall);

	 // ignore the function return value here
	 ARRAY_pop_back( thread->binding_data_stack, &data, sizeof(data) );
	 }
	 break; 
      case S_FUN_DECL: 
	 break; 
      case S_RETURN: {
	 AST_RETURN * ret = (AST_RETURN *) tmp;
	 BINDING_DATA *rval, *rframe;
 
	 EVAL_expression( ctx, thread, ret->rvalue );

	 rval = EVAL_THREAD_stack_at( thread, 0 );
	 rframe = EVAL_THREAD_stack_frame_offset( thread, 0 );
	 
	 memcpy( rframe, rval, sizeof(BINDING_DATA) );

	 // ignore the expression evaluation result
	 ARRAY_pop_back( thread->binding_data_stack, &data, sizeof(data) );
	 }
	 return STMT_LIST_RET_RETURN;

      case S_YIELD:  
	 break; 
      case S_NEXT:
	 return STMT_LIST_RET_NEXT;
      case S_BREAK:
	 return STMT_LIST_RET_BREAK;
      default:
	 break; 
    }
  }
  return 0;
 
}

int EVAL_function_call( PARSECONTEXT *ctx, EVAL_THREAD *thread, AST_FUNC_CALL *fcall)
{
  BINDING_DATA rdata,*data;
  AST_FUNC_DECL *fdecl;

  // reserve return value on stack.
  rdata.value_type = 0
  ARRAY_push_back( &thread->binding_data_stack, &rec, sizeof( rec ) );

  fdecl = fcall->func_decl;
  if ( ! fdecl ) {
     // if no function declaration known, then evaluate the function expression, it must get a lAMBDA type value.
     EVAL_expression( ctx, thread, fcall->f_name );
     data = EVAL_THREAD_stack_at( thread, 0 );
     if (data->value_type != S_VAR_CODE) {
       do_yyerror( &fcall->fname->base.location, "Function call must evaluate to a function type, instead is %s", get_type_name( data->value_type ) );
       exit(0);	 
     }
     fdecl = data->value.func_value.fdecl;

     // pop expression function value.
     ARRAY_pop_back( &thread->binding_data_stack );
  }

  // evaluate function parameters
  for( i = 0; i < ARRAY_size( &fcall->call_params ); i++) {
     AST_EXPRESSION *expr = (AST_EXPRESSION *) ARRAY_at( &fcall->call_params, i );
     EVAL_expression( ctx, thread, expr );
  }

  // evaluate the function
  EVAL_function( ctx, thread, fdecl );
}
    
// -----------------------------    
// function frame:
//   [ 0 ] - return value
//   [ 1 ] - this pointer
//   [ 2 ] - function arguments
//   [ n ] 
//   [ n + 1 ] - local variables
//   [ n + m ]
// -------------------------------  
int EVAL_function( PARSECONTEXT *ctx, EVAL_THREAD *thread, AST_FUNC_DECL *fdecl)
{
  FUNC_ACTIVATION_RECORD rec;
  DRING *tmp;
  AST_BASE *base;

  // make new activation record.
  rec.first_stack = ARRAY_size( &thread->binding_data_stack );
  rec.last_stack = rec.first_stack + fdecl->last_stack_offset;
  rec.fdecl  = fdecl;

  ARRAY_push_back( &thread->func_activation_record_stack, &rec, sizeof( rec ) );

  // function parameters have been set on stack prior to calling EVAL_function.
  EVAL_statement_list( PARSECONTEXT *ctx, EVAL_THREAD *thread, &fdecl->func_body )


 // free up stack variables - until return value (but not return value );
}


int EVAL_run( PARSECONTEXT *ctx, EVAL_THREAD *thread )
{
  AST_BASE_LIST *body;
  AST_BASE *base;

}
