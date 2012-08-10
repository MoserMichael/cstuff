#include <pooh-lan/parser.h>
#include <pooh-lan/ast.h>
#include <pooh-lan/pars.h>
#include <pooh-lan/rtlib/rtlib.h>
#include <eval.h>
#include <stdarg.h>


int can_assign( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type, AST_VAR_TYPE *offending_type);

struct tagEVAL_CTX;

void EVAL_do_expr( struct tagEVAL_CTX *out, AST_EXPRESSION *expr);
int  EVAL_do_function( struct tagEVAL_CTX *out , AST_FUNC_CALL *scl );
void EVAL_do(  struct tagEVAL_CTX *out );

int  CHECKER_check_func_call_params( PARSECONTEXT *ctx, int pass, AST_FUNC_CALL *fcall, AST_BASE *func_def );

// ========================================================================================


void TRACE_OUT_init( TRACE_OUT *out, FILE *fout )
{ 
  out->level = 0;
  out->max_level = 20;
  out->out = fout;
}

void TRACE_println_ext( TRACE_OUT *out, int line_no,  const char *line , int line_length, const char *suffix )
{
  int indent;

  indent = out->level;
  if (indent > out->max_level) {
    indent = out->max_level-4;
    fprintf(out->out, "%.3d|>>%*s>>%.*s%s\n", line_no, indent, "",  line_length, line, suffix );
  } else {
    fprintf(out->out, "%.3d|%*s%.*s%s\n", line_no, indent, "", line_length, line, suffix );
  }
}

void TRACE_println( TRACE_OUT *out, int line_no,  const char *line, int line_length  )
{
  TRACE_println_ext( out, line_no, line, line_length,  "" );
}

int TRACE_inc( TRACE_OUT *out )
{ 
  return out->level ++;
}
int TRACE_set( TRACE_OUT *out, int level )
{ 
  int rt = out->level;
  out->level = level;
  return rt;
}

// ========================================================================================

typedef struct tagEVAL_TRACE_ENTRY {
  AST_BASE *root;
//AST_BASE *entry;
  DBUF text;
  int start;

} EVAL_TRACE_ENTRY;

EVAL_TRACE_ENTRY *EVAL_TRACE_ENTRY_init(AST_BASE *root )
{
  EVAL_TRACE_ENTRY *entry;

  entry = (EVAL_TRACE_ENTRY *) malloc( sizeof( EVAL_TRACE_ENTRY ) );
  if (!entry) {
    return 0;
  }
  entry->root = root;
  DBUF_init( &entry->text, 30 );
  return entry;
}

void EVAL_TRACE_ENTRY_reset( EVAL_TRACE_ENTRY *entry, AST_BASE *root )
{
  
  entry->root = root;
//entry->root = entry->entry = 0;
  DBUF_reset( &entry->text );
}

void EVAL_TRACE_ENTRY_free( EVAL_TRACE_ENTRY *entry )
{
  DBUF_free( &entry->text );
  free( entry );
}

// ========================================================================================

int EVAL_init( EVAL_CTX *out, PARSECONTEXT *ctx)
{
  size_t num_globals = ((AST_FUNC_DECL *) ctx->my_ast_root)->last_stack_offset + 1;
  if (EVAL_CONTEXT_init( &out->context, num_globals )) {
    return -1;
  }
  ARRAY_init( &out->trace_buffer_entries, sizeof( void * ), 0);
  out->loop_exit = LOOP_NO_BREAK; 
  TRACE_OUT_init( &out->trace_out, stderr );

  out->context.trace_on= 1;

  out->last_freed = out->top_trace = 0;
  out->ctx = ctx;
  return 0;
}

EVAL_TRACE_ENTRY *EVAL_CTX_new_trace(EVAL_CTX *ctx, AST_BASE *root)
{ 
  EVAL_TRACE_ENTRY *ret;

  if (ctx->last_freed) {
    ret = ctx->last_freed;
    ctx->last_freed = 0;
    ctx->top_trace = ret;
    EVAL_TRACE_ENTRY_reset( ret, root );
    ARRAY_push_back( &ctx->trace_buffer_entries, &ret, sizeof(void *));
    return ret;
  }
  ret = EVAL_TRACE_ENTRY_init( root );
  ARRAY_push_back( &ctx->trace_buffer_entries, &ret, sizeof(void *));
   ctx->last_freed = 0;  
  ctx->top_trace = ret;
   return ret;
}

typedef enum {
  SHOW,
  DISCARD,
} EVAL_CTX_FREE_ACTION;
  

void EVAL_CTX_show( EVAL_CTX *ctx, EVAL_TRACE_ENTRY *entry, const char *suffix )
{
   TRACE_println_ext( &ctx->trace_out, entry->root->location.first_line, (const char *) entry->text.buf, entry->text.buf_used, suffix  );
}


void EVAL_CTX_free_trace( EVAL_CTX *ctx, EVAL_CTX_FREE_ACTION show )
{
  EVAL_TRACE_ENTRY *trace;

  ARRAY_pop_back( &ctx->trace_buffer_entries, &trace, sizeof(void *));
 if (show  == SHOW) {
   TRACE_println( &ctx->trace_out, trace->root->location.first_line, (const char *) trace->text.buf, trace->text.buf_used  );
  }
  
  ARRAY_stack_top( &ctx->trace_buffer_entries,  &ctx->top_trace, sizeof(void *));
//ctx->top_trace = trace
 
  if (ctx->last_freed == 0) {
    ctx->last_freed = trace;
    return;
  }
  EVAL_TRACE_ENTRY_free( ctx->last_freed );
  ctx->last_freed = trace;
}

// ========================================================================================

void EVAL_error( EVAL_CTX *out, AST_BASE *ast, const char *format, ... )
{
  char msg[RUNTIME_ERROR_MSG_LEN];
  int len;
  va_list ap;
  EVAL_THREAD *cthread;

  va_start(ap, format);
  len = vsnprintf( msg, sizeof(msg) - 1, format, ap );

  if (ast == 0) {
     cthread = out->context.current_thread;
     ast = cthread->instr;
  }

  do_yyerror ( &ast->location, out->ctx, "%s", msg);

  EVAL_CONTEXT_runtime_error( &out->context, 0 );
}

BINDING_DATA * EVAL_int_num_op( EVAL_CTX *out, int op, BINDING_DATA *lhs, BINDING_DATA *rhs )
{
  long rval,lval,ret;
  BINDING_DATA *retval;
  EVAL_THREAD *cthread;
 
  cthread = out->context.current_thread;
  lval = lhs->b.value.long_value;
  rval = rhs->b.value.long_value;

  
  switch( op ) {

    case TK_OP_NUM_SUBST:
      ret = lval - rval;
      break;
    case TK_OP_NUM_ADD:
      ret = lval + rval;
      break;
    case TK_OP_NUM_DIV: {
      if (rval == 0) {
	 EVAL_error( out , 0,  "can't divide by zero" );
      }
      retval = EVAL_THREAD_push_stack( cthread, S_VAR_DOUBLE );  
      retval->b.value.double_value = (double) lval / rval;
      }
      break;
    case TK_OP_NUM_MULT:
      ret = lval * rval;
      break;
    case TK_OP_NUM_MOD:
      if (rval == 0) {
	 EVAL_error( out , 0 , "can't divide by zero" );
      }
      ret = lval % rval;
      break;
    case TK_OP_NUM_LT: 
      ret = lval < rval;
      break;
    case TK_OP_NUM_GT: 
      ret = lval > rval;
      break;
    case TK_OP_NUM_EQ: 
      ret = lval == rval;
      break;
    case TK_OP_NUM_NE: 
      ret = lval != rval;
      break;
    case TK_OP_NUM_LE: 
      ret = lval <= rval;
      break;
    case TK_OP_NUM_GE:
      ret = lval >= rval;
      break;
    default:  
      assert(0);
  }
  
  EVAL_THREAD_discard_pop_stack( cthread );
  EVAL_THREAD_discard_pop_stack( cthread );

  retval = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
  retval->b.value.long_value = ret;

  return retval;
}

BINDING_DATA * EVAL_cmp_null( EVAL_CTX *out, int op, BINDING_DATA *rhs )
{
    BINDING_DATA *retval;
    EVAL_THREAD *cthread;
    int rvalt;
 	
    cthread = out->context.current_thread;
 
    switch(op)
    { 
      case TK_OP_NUM_EQ:
	 rvalt = (rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL;
	 break;
       case TK_OP_NUM_NE:
       	 rvalt = (rhs->b.value_type & S_VAR_ALL_TYPES) != S_VAR_NULL;
	 break;
       default:
         assert(0);
         break;
    }	

    EVAL_THREAD_discard_pop_stack( cthread );
    EVAL_THREAD_discard_pop_stack( cthread );
    
    retval = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
    retval->b.value.long_value =  rvalt;
    return retval;
}

BINDING_DATA * EVAL_double_num_op( EVAL_CTX *out, int op, BINDING_DATA *lhs, BINDING_DATA *rhs )
{
  double rval = 0,lval = 0 ,ret;
  BINDING_DATA *retval;
  EVAL_THREAD *cthread;
  int rvalt;
 	
  cthread = out->context.current_thread;
 	
  if (op == TK_OP_NUM_EQ || op ==TK_OP_NUM_NE) {
    if ((lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
     return EVAL_cmp_null(  out, op, rhs );
    }
  
    if ((rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
      return EVAL_cmp_null(  out, op, lhs );
    }
  }
  
  BINDING_DATA_get_double( lhs, &lval );
  BINDING_DATA_get_double( rhs, &rval );
  
  rvalt = 1;
  switch( op ) {
    case TK_OP_NUM_SUBST:
      ret = lval - rval;
      break;
    case TK_OP_NUM_ADD:
      ret = lval + rval;
      break;
    case TK_OP_NUM_DIV:
      if (rval == 0) {
	 EVAL_error( out , 0,  "can't divide by zero" );
      }
      ret = lval / rval;
      break;
    case TK_OP_NUM_MULT:
      ret = lval * rval;
      break;
    case TK_OP_NUM_MOD:
      if (rval == 0) {
	 EVAL_error( out , 0 , "can't divide by zero" );
      }
      ret = (long) lval % (long) rval;
      break;
    case TK_OP_NUM_LT: 
      ret = lval < rval;
      rvalt = 0;
      break;
    case TK_OP_NUM_GT: 
      ret = lval > rval;
      rvalt = 0;
      break;
    case TK_OP_NUM_LE: 
      ret = lval <= rval;
      rvalt = 0;
      break;
    case TK_OP_NUM_GE:
      ret = lval >= rval;
      rvalt = 0;
      break;
    case TK_OP_NUM_EQ: 
      ret = lval == rval;
      rvalt = 0;
      break;
    case TK_OP_NUM_NE: 
      ret = lval != rval;
      rvalt = 0;
      break;
    default:  
      assert(0);
  }
  
  EVAL_THREAD_discard_pop_stack( cthread );
  EVAL_THREAD_discard_pop_stack( cthread );

  if (rvalt) {
    retval = EVAL_THREAD_push_stack( cthread, S_VAR_DOUBLE );   
    retval->b.value.double_value = ret;
  } else {
    retval = EVAL_THREAD_push_stack( cthread, S_VAR_INT );   
    retval->b.value.long_value = (long) ret;
  }
  
  return retval;
}

void tracer_add_boolean_value( EVAL_TRACE_ENTRY *tracer, int bval )
{
  const char *msg;
  if (bval) 
    msg = "true";
  else 
    msg = "false";
   DBUF_add(  &tracer->text, msg , strlen(msg) );
}
  
BINDING_DATA *  EVAL_boolean_op( EVAL_CTX *out,  AST_EXPRESSION *expr )
{
  int op = expr->val.expr.op;
  EVAL_THREAD *cthread;
  EVAL_TRACE_ENTRY *tracer;
  BINDING_DATA *val, *ret;
  double nval;
  const char *msg;

  cthread = out->context.current_thread;
  tracer = out->top_trace;
 
  EVAL_do_expr( out, expr->val.expr.expr_left );
  val = EVAL_THREAD_pop_stack( cthread );
  val = BINDING_DATA_follow_ref( val );
 
  BINDING_DATA_get_double( val, &nval );
  
  if ( op == TK_OP_LOGICAL_AND && nval == 0) {
	ret = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
        ret->b.value.long_value = 0;
        if (tracer) {
          msg = " and _ ):";
	  DBUF_add(  &tracer->text, msg , strlen(msg) );
	}
	return ret;
  }

  if ( op == TK_OP_LOGICAL_OR && nval == 1) {
	ret = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
        ret->b.value.long_value = 1;
        if (tracer) {
          msg = " or _ ):";
	  DBUF_add(  &tracer->text, msg , strlen(msg) );
	}
	return ret;
  }
  
  
  if (tracer)  {
    const char *op_name = get_op_name(op);  
    DBUF_add(  &tracer->text, " ", 1  );
    DBUF_add(  &tracer->text, op_name, strlen( op_name )  );
    DBUF_add(  &tracer->text, " ", 1  );
  }


  EVAL_do_expr( out, expr->val.expr.expr_right );
  val = EVAL_THREAD_pop_stack( cthread );
  val = BINDING_DATA_follow_ref( val );

  BINDING_DATA_get_double( val, &nval );
 
  ret = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
  ret->b.value.long_value = nval != 0 ? 1 : 0;
  return ret;
}  

BINDING_DATA *EVAL_string_op( int op, EVAL_THREAD *cthread, BINDING_DATA *lhs, BINDING_DATA *rhs )
{ 
  BINDING_DATA *retval; 
  int rn;
  DBUF tmpl, tmpr;
  
   if ((lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
    retval = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
    retval->b.value.long_value =  (rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL;
    return retval;
  }
  
  if ((rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
    retval = EVAL_THREAD_push_stack( cthread, S_VAR_INT );  
    retval->b.value.long_value =  (lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL;
    return retval;
  }
  
  //assert( is_string_op( op )); 
  
  if (op == TK_OP_STR_CAT) {
     BINDING_DATA tmpres;

     BINDING_DATA_init(&tmpres, S_VAR_STRING );

     if ((lhs->b.value_type & S_VAR_ANY) == S_VAR_STRING) {
	VALSTRING_copy( &tmpres.b.value.string_value, &lhs->b.value.string_value );
     } else {
        DBUF_init( &tmpl, 0);
	BINDING_DATA_prints( &tmpl, lhs, 0 );
        DBUF_add_null( &tmpl );
        
	VALSTRING_set( &tmpres.b.value.string_value, (const char *) tmpl.buf, tmpl.buf_used );
     }
     if ((rhs->b.value_type & S_VAR_ANY) == S_VAR_STRING) {
	VALSTRING_appends( &tmpres.b.value.string_value, (const char *) &rhs->b.value.string_value );
     } else {
        DBUF_init( &tmpl, 0);
	BINDING_DATA_prints( &tmpl, rhs, 0 );
        DBUF_add_null( &tmpl );
        
	VALSTRING_appends( &tmpres.b.value.string_value, (const char *) tmpl.buf );
     }
 
     EVAL_THREAD_discard_pop_stack( cthread );
     EVAL_THREAD_discard_pop_stack( cthread );

     retval = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );  
     BINDING_DATA_move( retval, &tmpres);
     return retval;
  }
 
  if ((rhs->b.value_type & S_VAR_ANY) == S_VAR_STRING && (lhs->b.value_type & S_VAR_ANY) == S_VAR_STRING) {
     rn = VALSTRING_cmp( &lhs->b.value.string_value, &rhs->b.value.string_value );
  } else {
     const char *sleft, *sright;
     size_t slen = -1;

     if ((lhs->b.value_type & S_VAR_ANY) != S_VAR_STRING) {  
       DBUF_init( &tmpl, 0);
       BINDING_DATA_prints( &tmpl, lhs, 0 );
       DBUF_add_null( &tmpl );
       sleft = (const char *) tmpl.buf;
     } else {
       sleft = lhs->b.value.string_value.string; 
       slen = lhs->b.value.string_value.length;
     }

     if ((rhs->b.value_type & S_VAR_ANY) != S_VAR_STRING) {  
       DBUF_init( &tmpr, 0);
       BINDING_DATA_prints( &tmpr, rhs, 0 );
       DBUF_add_null( &tmpr );
       sright = (const char *) tmpr.buf;
     } else {
       sright = rhs->b.value.string_value.string;  
       slen = rhs->b.value.string_value.length;
     }

     rn = slen == (size_t)  -1 ? strcmp( sleft, sright ) : strncmp( sleft, sright, slen );
  }
    
  switch( op ) {
	case TK_OP_STR_EQ:
	    rn = rn == 0;
	    break;
	case TK_OP_STR_NE: 
	    rn = rn != 0;
	    break;
	case TK_OP_STR_LT: 
	    rn = rn < 0;
	    break;
	case TK_OP_STR_GT: 
	    rn = rn > 0;
	    break;
	case TK_OP_STR_LE: 
	    rn = rn <= 0;
	    break;
	case TK_OP_STR_GE:
		rn = rn >= 0;
	    break;
	default:
	    assert(0);
  }
 
 
  EVAL_THREAD_discard_pop_stack( cthread );
  EVAL_THREAD_discard_pop_stack( cthread );

  retval = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );  
  BINDING_DATA_init(retval, S_VAR_INT );
  retval->b.value.long_value = rn;
 
  return retval;
}

BINDING_DATA *create_follow_data( S_EXPR_TYPE ty )
    {
   if (ty == S_EXPR_HASH_INDEX) {
     return BINDING_DATA_MEM_new( S_VAR_HASH );
   } else if (ty == S_EXPR_ARRAY_INDEX) {
     return BINDING_DATA_MEM_new( S_VAR_LIST );
   } else {
    assert(0);
   }
   return 0;
}

BINDING_DATA *EVAL_reference_scalar( EVAL_CTX *out, AST_EXPRESSION *expr, BINDING_DATA *new_value, CP_KIND copy_type)
{
    EVAL_THREAD *cthread;
    BINDING_DATA *data,*arec;
    BINDING_ENTRY *bentry;
    VALACTIVATION *activation_rec;
    VALFUNCTION *val_func;
    int scope = REF_SCOPE_THIS; 


#if 0
    scope = expr->val.ref.scope;
#endif    
    bentry = expr->val.ref.binding;
    if (bentry) {
      scope = bentry->scope;
    }

    if (scope == REF_SCOPE_GLOBAL) {
      ARRAY *glob;
   
      assert( strcmp(expr->val.ref.lhs, "this") != 0 ); // no data, so this must be 'this'.
      
      glob =  &out->context.bindings_global;	
      if (new_value) {
        data = (BINDING_DATA *) ARRAY_at( glob, bentry->stack_offset);
	assert( data != 0 );
	BINDING_DATA_copy( data, new_value, copy_type );
      } else {
        data = (BINDING_DATA *) ARRAY_at( glob, bentry->stack_offset ); 
#if 0
	if ((data->b.value_type & (S_VAR_HASH | S_VAR_LIST)) == 0) {
          EVAL_CONTEXT_runtime_error( &out->context , "Internal error: this must be array or hash, instead got %s", get_type_name( data->b.value_type ) );
	}
#endif        
    	return data;
      }
      return data;
    }

    cthread = out->context.current_thread;
    arec = EVAL_THREAD_stack_frame_offset( cthread, 1 );
    activation_rec = &arec->activation_record;
    assert( activation_rec );

    val_func = activation_rec->function_object;
 
#if 0
    switch( expr->val.ref.scope ) {
#else
    switch( scope ) {
#endif
    case REF_SCOPE_THIS:
      if (new_value != 0) {
        EVAL_error( out,  0, "Can' assign value to this." );
      } else {
        if (!val_func->this_environment) {
          EVAL_error( out,  0,  "not called via this." );
	}
	data = val_func->this_environment;
      }
      break;
    case REF_SCOPE_LOCAL:
      if (new_value) {
        data = (BINDING_DATA *) EVAL_THREAD_stack_frame_offset( cthread, bentry->stack_offset );
	assert( data != 0 );
	BINDING_DATA_copy( data, new_value, copy_type );
      } else {
	data = EVAL_THREAD_stack_frame_offset( cthread, bentry->stack_offset );
      }
      break;
    case REF_SCOPE_CLOSURE:
      assert(0);
      break;
    default:
      assert(0);
  }
  return data;
}
void EVAL_reference( EVAL_CTX *out, AST_EXPRESSION *expr, int in_assignment, CP_KIND copy_type)
{
  EVAL_THREAD *cthread;
  BINDING_ENTRY *entry;
  BINDING_DATA *data,*nvalue;
  EVAL_TRACE_ENTRY *tracer = 0;

  entry = expr->val.ref.binding;
  cthread = out->context.current_thread;
  
  if (EVAL_trace_on(out) ) {
      tracer =  out->top_trace;
  }
 
#if 1 
  if (tracer) { 
    DBUF_add(  &tracer->text, expr->val.ref.lhs, strlen( expr->val.ref.lhs ) );
  }
#endif  


  if (expr->val.ref.indexes == 0) { // no array indexes.
     if (in_assignment == 0) {
       nvalue = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );
       data = EVAL_reference_scalar( out, expr, 0, copy_type);

       // copy to stack what is easy to copy, put in ref otherwise.
       BINDING_DATA_copy( nvalue , data, data->b.value_type & (S_VAR_INT | S_VAR_DOUBLE | S_VAR_NULL) ? CP_VALUE : CP_REF ); // copy_type ); 
       if (tracer) { 
 	DBUF_add(  &tracer->text, ":", 1 );
	BINDING_DATA_prints( &tracer->text, data, 0 );
       }	
     } else {
       nvalue = EVAL_THREAD_get_stack_top( cthread );
       EVAL_reference_scalar( out, expr, nvalue, copy_type);
     } 
  } else {
     AST_VECTOR *vect = expr->val.ref.indexes;
     AST_EXPRESSION *index_expr,*follow_index_expr;
     BINDING_DATA *index_data, *next_data;
     size_t i;
     double nval;
     AST_VAR_TYPE collection_type;

     EVAL_THREAD_push_stack( cthread, S_VAR_NULL );
     data = EVAL_reference_scalar( out, expr, 0, copy_type);
     BINDING_DATA_copy( EVAL_THREAD_get_stack_top( cthread ), data, CP_REF ); 
     data = BINDING_DATA_follow_ref( data );
     collection_type = data->b.value_type;

     for( i = AST_VECTOR_size( vect ); ;) {
        index_expr = (AST_EXPRESSION *) AST_VECTOR_get( vect , i - 1 );
	follow_index_expr = i > 1 ? (AST_EXPRESSION *) AST_VECTOR_get( vect, i - 2 ) : 0;

	// check that collection type is what is expected by the type of index expression.
        if (index_expr->exp_type == S_EXPR_HASH_INDEX) {
	   if (collection_type != S_VAR_HASH) {
	     EVAL_error( out , &expr->base,  "Expects table value for %s ", expr->val.ref.lhs, get_type_name( data->b.value_type ) );
	   }
	} else if (index_expr->exp_type == S_EXPR_ARRAY_INDEX) {
       	   if (data->b.value_type  != S_VAR_LIST) {
	     EVAL_error( out, &expr->base, "Expects array value for %s ", expr->val.ref.lhs, get_type_name( data->b.value_type ) );
	   }
	} else {
	  assert(0);
	}

	// evaluate the inex expression.
	EVAL_do_expr( out, index_expr );
	index_data = EVAL_THREAD_pop_stack( cthread );

        // for array collection - check that the index is a number (must).
	if (index_expr->exp_type == S_EXPR_ARRAY_INDEX) {
	  if (! is_numeric_type( index_data->b.value_type ) ) {
	    EVAL_error( out, &expr->base, "Expects number as array index for %s instead got %s", expr->val.ref.lhs, get_type_name( data->b.value_type ) );
	  }
	}

        // perform the collection lookup
  	data = index_data - 1; 
        data = BINDING_DATA_follow_ref( data );
	    
	// if last index and function is used in assingnment - insert value into collection
	if ( follow_index_expr == 0 && in_assignment) {
	    if (index_expr->exp_type == S_EXPR_HASH_INDEX) {  // insert into hash 
	    	VALHASH_set( &data->b.value.hash_value, index_data, index_data - 2, CP_REF );	
            } else {					      // insert into vector
	        BINDING_DATA_get_double( index_data, &nval );
	        VALARRAY_set( &data->b.value.array_value, (long) nval - 1 , index_data - 2, CP_REF );
	    }
            EVAL_THREAD_pop_stack( cthread );

	} else { // not the last index - lookup collection value. 

            if (index_expr->exp_type == S_EXPR_HASH_INDEX) {
	      VALHASH_find( &data->b.value.hash_value , index_data, &next_data );
	      if (follow_index_expr != 0) {
	        if (next_data == 0) {  
	          next_data = create_follow_data( follow_index_expr->exp_type );
	      	  VALHASH_set( &data->b.value.hash_value, index_data, next_data, copy_type );	
		}  
	      }
	    } else {
	      BINDING_DATA_get_double( index_data, &nval );
	      next_data = VALARRAY_get_n( &data->b.value.array_value, (long) nval - 1 );
	      if (follow_index_expr != 0) {
	        if (next_data == 0) {
	          next_data = create_follow_data( follow_index_expr->exp_type );
	  	  VALARRAY_set( &data->b.value.array_value, (long) nval - 1, next_data, copy_type );
	        }
	      }
	      if (next_data == 0) {
		next_data = get_CONST_NULL();
	      }
	    }

    
	    if (next_data != 0) {
              BINDING_DATA_copy( EVAL_THREAD_get_stack_top( cthread ), next_data, CP_REF ); 
            } else {
              next_data = EVAL_THREAD_get_stack_top( cthread );
	      BINDING_DATA_init( next_data, S_VAR_NULL ); 
	    }
	}

	if (follow_index_expr == 0) 
	      break;

	i -=1;
	next_data = BINDING_DATA_follow_ref( next_data );
	collection_type = next_data->b.value_type;
     } // end for (indexes)

     if (tracer) { 
       DBUF_add(  &tracer->text, ":", 1 );
       data = EVAL_THREAD_get_stack_top( cthread ); 
       data = BINDING_DATA_follow_ref( data );
       BINDING_DATA_prints( &tracer->text, data, 0 );
      }	
     
  } // end else (collection)
  
}

void EVAL_do_expr( EVAL_CTX *out, AST_EXPRESSION *expr)
{ 
  EVAL_THREAD *cthread;
  BINDING_DATA *ret,*lhs,*rhs;
  size_t nlhs;
  EVAL_TRACE_ENTRY *tracer = 0;
  size_t i;

  cthread = out->context.current_thread;
  if (EVAL_trace_on(out)) {
      tracer =  out->top_trace;
      assert( tracer );
  }
  switch( expr->exp_type ) 
  {
    case S_EXPR_BINARY: {
      int op = expr->val.expr.op;
      int  is_op_num;
      int  is_op_bool = 0;
   
      if (tracer && op != TK_OP_STR_CAT)  {
        DBUF_add(  &tracer->text, "(", 1 );
      }

      if (op ==  TK_OP_LOGICAL_OR || op == TK_OP_LOGICAL_AND )  {
        is_op_bool = 1;
        ret = EVAL_boolean_op( out, expr );
      } else  {

        EVAL_do_expr( out, expr->val.expr.expr_left );
        nlhs = EVAL_THREAD_get_stack_top_pos( cthread );
	
	is_op_bool = is_operator_with_boolean_result( op );
 
        is_op_num = is_operator_with_number_args(op);
        if (is_op_num) {
	
	  lhs = EVAL_THREAD_get_stack_top( cthread );
	  lhs = BINDING_DATA_follow_ref( lhs );
	  if ( !is_scalar_var_or_null_type( lhs->b.value_type )) {
	    EVAL_error( out, &expr->base , "Operator %s expects number as left hand side, instead got %s", get_op_name( op ), get_type_name( lhs->b.value_type ) );
	  }

          if (tracer)  {
            const char *op_name = get_op_name(op);  
	    DBUF_add(  &tracer->text, " ", 1  );
            DBUF_add(  &tracer->text, op_name, strlen( op_name )  );
	    DBUF_add(  &tracer->text, " ", 1  );
          }

	  EVAL_do_expr( out, expr->val.expr.expr_right );
	  rhs = EVAL_THREAD_get_stack_top( cthread );
	  rhs = BINDING_DATA_follow_ref( rhs );
  
	  if ( ! is_scalar_var_or_null_type( rhs->b.value_type )) {
       	    EVAL_error( out , &expr->base  , "Operator %s expects number as left hand side, instead got %s", get_op_name( op ), get_type_name( rhs->b.value_type ) );
          }


	  lhs = EVAL_THREAD_stack_offset( cthread, nlhs );
	  lhs = BINDING_DATA_follow_ref( lhs );
  
	  if ((lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_INT && (rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_INT) {
            ret = EVAL_int_num_op( out, op,  lhs, rhs );
          } else {
            ret = EVAL_double_num_op( out, op,  lhs, rhs );
	  }
	} else {

	   if (tracer)  {
             const char *op_name = get_op_name(op);  
	     DBUF_add(  &tracer->text, " ", 1  );
             DBUF_add(  &tracer->text, op_name, strlen( op_name )  );
	     DBUF_add(  &tracer->text, " ", 1  );
           }

	   EVAL_do_expr( out, expr->val.expr.expr_right );
	   rhs =  EVAL_THREAD_get_stack_top( cthread );
	   rhs = BINDING_DATA_follow_ref( rhs );

       	   lhs = EVAL_THREAD_stack_offset( cthread, nlhs );
	   lhs = BINDING_DATA_follow_ref( lhs );
 
	   ret = EVAL_string_op( op, cthread, lhs, rhs );
	}
	}
	if (tracer && op != TK_OP_STR_CAT)  {
          DBUF_add(  &tracer->text, "):", 2 );
	  if (is_op_bool) {
	    tracer_add_boolean_value( tracer,  ret->b.value.long_value );
	  } else {
	    BINDING_DATA_prints( &tracer->text, ret, 0 ); 
	  }
        }
     }
#if 0	
      is_op_num = is_operator_with_number_args(expr->val.expr.op);
 
      EVAL_do_expr( out, expr->val.expr.expr_left );
      lhs = EVAL_THREAD_pop_stack( cthread );
      lhs = BINDING_DATA_follow_ref( lhs );
    
      if (tracer)  {
	    const char *op_name = get_op_name(op);  
        DBUF_add(  &tracer->text, op_name, strlen( op_name )  );
      }

      if (is_op_num && ! is_scalar_var_type( lhs->b.value_type )) {
       	EVAL_CONTEXT_runtime_error( &out->context , "Operator %s expects number as left hand side, instead got %s", get_op_name( op ), get_type_name( lhs->b.value_type ) );
      }
	
      if (is_operator_with_boolean_result( op )) {
          is_op_bool = 1;
	  if (!EVAL_boolean_op( out, op, 0, lhs, 1 )) {
	     return;
	  }
      }
       
      EVAL_do_expr( out, expr->val.expr.expr_right );
      rhs = EVAL_THREAD_pop_stack( cthread );
      rhs = BINDING_DATA_follow_ref( rhs );
      
      if (is_op_num && ! is_scalar_var_type( rhs->b.value_type )) {
       	EVAL_CONTEXT_runtime_error( &out->context , "Operator %s expects number as left hand side, instead got %s", get_op_name( op ), get_type_name( rhs->b.value_type ) );
      }
	 
      ret = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );  

      if (is_op_bool) {
	 EVAL_boolean_op( out, op, ret, lhs, 0 );
      } if ( is_op_num ) {
	if ((lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_INT && (rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_INT) {
          EVAL_int_num_op( out, op, ret, lhs, rhs );
        } else {
          EVAL_double_num_op( out, op, ret, lhs, rhs );
	}
      } else {
         EVAL_string_op( op, ret, lhs, rhs );
      }

      if (tracer)  {
        DBUF_add(  &tracer->text, "):", 2 );
	if (is_op_bool) {
	  tracer_add_boolean_value( tracer,  ret->b.value.long_value );
	} else {
	  BINDING_DATA_prints( &tracer->text, ret, 0 ); 
	}
      }
#endif      
      break;

    case S_EXPR_UNARY:
      EVAL_do_expr( out, expr->val.unary.expr );
      break;

    case S_EXPR_REFERENCE: {
      EVAL_reference( out, expr, 0, CP_VALUE );
      }
      break;

    case S_EXPR_CONSTANT: {

      ret = EVAL_THREAD_push_stack( cthread, expr->value_type );
      switch( expr->value_type ) {
        case S_VAR_INT:
	    ret->b.value.long_value = expr->val.const_value.long_value;
	    break;
	case S_VAR_DOUBLE:
	    ret->b.value.double_value = expr->val.const_value.double_value;
	    break;
	case S_VAR_STRING: {
	    const char *str = expr->val.const_value.string_value;
	    size_t nstr = strlen( str );
	    VALSTRING_set( &ret->b.value.string_value, str, nstr );
	    }
	    break;
	case S_VAR_NULL:
	    break;
	default:
	    assert(0);
      }
      if (tracer) {
        BINDING_DATA_prints( &tracer->text, ret, 0 ); 
      }
      }
      break;

    case S_EXPR_FUNCALL:
     EVAL_do_function( out , expr->val.fcall );
     break;

    case S_EXPR_HASH_INDEX:
      if (tracer)  {
        DBUF_add(  &tracer->text, "{", 1 );
      }
      EVAL_do_expr( out, expr->val.index_expr );
      if (tracer)  {
        DBUF_add(  &tracer->text, "}", 1 );
      }
      break;
  
    case S_EXPR_ARRAY_INDEX:
      if (tracer)  {
        DBUF_add(  &tracer->text, "[", 1 );
      }
      EVAL_do_expr( out, expr->val.index_expr );
       if (tracer)  {
        DBUF_add(  &tracer->text, "]", 1 );
      }
     break;
  
    case S_EXPR_LIST_VALUES: {
      AST_VECTOR *vect;
      VALARRAY *rval;
      size_t rpos;


      if (tracer)  {
        DBUF_add(  &tracer->text, "[ ", 2 );
      }
    
      ret = EVAL_THREAD_push_stack( cthread, S_VAR_LIST );
      rpos = EVAL_THREAD_get_stack_top_pos( cthread );
  
      vect = expr->val.index_expressions;
      for( i = 0; i < AST_VECTOR_size( vect ); i++ ) {
	 expr = (AST_EXPRESSION *) AST_VECTOR_get( vect, i );
    
	 if (tracer && i > 0) {
	    DBUF_add(  &tracer->text, " , ", 3 );
	 }
         EVAL_do_expr( out, expr );
         rhs = EVAL_THREAD_pop_stack( cthread );

	 ret = EVAL_THREAD_stack_offset( cthread, rpos );
         rval = &ret->b.value.array_value;

	 VALARRAY_set( rval,  i, rhs, CP_REF );
	 BINDING_DATA_free( rhs );
      }
  
      assert( rpos == ( ARRAY_size( &cthread->binding_data_stack ) - 1  ) );
 
      if (tracer)  {
        DBUF_add(  &tracer->text, "] ", 2 );
      }
      }
      break;
 
    case S_EXPR_HASH_VALUES: {
      AST_VECTOR *vect;
      VALHASH *rval;
      size_t rpos;

      if (tracer)  {
        DBUF_add(  &tracer->text, "{ ", 2 );
      }
  
      EVAL_THREAD_push_stack( cthread, S_VAR_HASH );
      rpos = EVAL_THREAD_get_stack_top_pos( cthread );
     
 
      vect = expr->val.index_expressions;
      for( i = 0; i < AST_VECTOR_size( vect ); i++ ) {
	 expr = (AST_EXPRESSION *) AST_VECTOR_get( vect, i );
	 if (tracer && i > 0) {
	    DBUF_add(  &tracer->text, " , ", 3 );
	 }
	 EVAL_do_expr( out, expr->val.expr.expr_left );
	 
	 if (tracer) {
	    DBUF_add(  &tracer->text, " : ", 3 );
	 }

	 EVAL_do_expr( out,  expr->val.expr.expr_right );
         rhs = EVAL_THREAD_pop_stack( cthread );
         lhs = EVAL_THREAD_pop_stack( cthread );
	
         ret = EVAL_THREAD_stack_offset( cthread, rpos );
         rval  = &ret->b.value.hash_value;	

	 VALHASH_set( rval, lhs , rhs, CP_REF );
         BINDING_DATA_free( rhs );
         BINDING_DATA_free( lhs );
      }

      assert( rpos == ( ARRAY_size( &cthread->binding_data_stack ) - 1  ) );

      if (tracer)  {
        DBUF_add(  &tracer->text, " }", 2 );
      }
      }
      break;

    case S_EXPR_PLACEHOLDER:
      break;

    case S_EXPR_LAMBDA:
    case S_EXPR_LAMBDA_RESOLVED_REF: {
      AST_BASE *fdecl;
      VALFUNCTION *valfunc;

      fdecl = expr->val.fdecl;
      assert( fdecl );
      
      ret = EVAL_THREAD_push_stack( cthread, S_VAR_CODE );
      valfunc = &ret->b.value.func_value;
      valfunc->fdecl = fdecl;

#if 0
      if ( fdecl->base.type == S_FUN_DECL ) {
      
        // for non local reference, create capture value.
        VALFUNCTION_init_outer_ref_tree( cthread, &ret->b.value.func_value, fdecl );
      }
#endif      
      }
      break;
    
    case S_EXPR_ERROR:
      break;

    default:
      assert(0);
  }
}

void EVAL_do_function_param( EVAL_CTX *out , EVAL_THREAD *cthread, AST_FUNC_CALL *scl, size_t frame_start )
{
      size_t i;
      AST_FUNC_CALL_PARAM *param;  
      EVAL_TRACE_ENTRY *tracer = 0;
  
      tracer = out->top_trace;

      // *** evaluate function parameters ***
      for(i = 0; i < AST_VECTOR_size(scl->call_params); i++ ) {
	param = (AST_FUNC_CALL_PARAM *) AST_VECTOR_get( scl->call_params, i );

	if (tracer) {
	  if (i > 0) {
	    DBUF_add(  &tracer->text, " ", 1 );
	  }
	  DBUF_add(  &tracer->text, "~", 1 );
	  DBUF_add(  &tracer->text, param->label_name, strlen(param->label_name ) );
	  DBUF_add(  &tracer->text, " ", 1 );
	}

//	EVAL_reference( out, param->expr, 0, CP_VALUE );
	EVAL_do_expr( out,  param->expr );

	BINDING_DATA_move( EVAL_THREAD_parameter_ref( cthread, frame_start, param->param_num ), EVAL_THREAD_pop_stack( cthread ) );
      }
}

int EVAL_do_function( EVAL_CTX *out , AST_FUNC_CALL *scl )
{
    AST_EXPRESSION *f_name = scl->f_name;
    AST_BASE *fdecl;
    VALFUNCTION *valfunc = 0;
    BINDING_DATA *data;
    EVAL_THREAD *cthread;
    size_t frame_start;
    EVAL_TRACE_ENTRY *tracer = 0;
    int prev;
    int has_return_value = 0;

    cthread = out->context.current_thread;
  

 
    tracer = out->top_trace;
 
    if (f_name->exp_type != S_EXPR_LAMBDA_RESOLVED) {
      // evaluate expression that must return the function value.
      
      EVAL_do_expr( out, f_name );
      data = EVAL_THREAD_pop_stack( cthread );
      
      data = BINDING_DATA_follow_ref( data );
 
      if (data->b.value_type != S_VAR_CODE) {
	EVAL_error( out, &f_name->base , "Expression must return function type, instead got %s", get_type_name( data->b.value_type ) );
      }
      valfunc = &data->b.value.func_value;
      fdecl = valfunc->fdecl;

      // check that all parameters are supplied.
      if (CHECKER_check_func_call_params( out->ctx, 0, scl, fdecl ) ||
          CHECKER_check_func_call_params( out->ctx, 0, scl, fdecl ) ) {
	EVAL_error( out, &scl->base,  "Internal error in function call" );
      }
    
    } else {
      fdecl = f_name->val.fdecl;
 
      if (tracer) {
        DBUF_add(  &tracer->text, f_name->val.ref.lhs, strlen( f_name->val.ref.lhs) );
      }
    }	
     
    if (tracer) {
        DBUF_add(  &tracer->text, "( ", 2 );
    }

 
    if (fdecl->type == S_FUN_DECL) {
      AST_FUNC_DECL *ffdecl = (AST_FUNC_DECL *) fdecl;

      
      frame_start = EVAL_THREAD_prepare_call( cthread, ffdecl );

      EVAL_do_function_param( out, cthread, scl, frame_start );

      if (tracer) {
        DBUF_add(  &tracer->text, ")", 1 );
        EVAL_CTX_show( out, tracer, "..." );
        prev = TRACE_inc( &out->trace_out );
      }


      EVAL_THREAD_call_func( cthread, frame_start, ffdecl, valfunc );
      
      // do the function body.
      cthread->instr = (AST_BASE *) ffdecl->func_body;
      EVAL_do( out );

      out->loop_exit = 0;
  
      if (tracer) {
	has_return_value = ffdecl->return_type_value != 0; 
        TRACE_set( &out->trace_out, prev );
      }
 
    } else {
      assert( fdecl->type == S_XFUN_DECL);
      
      frame_start = EVAL_THREAD_prepare_xcall( cthread, (AST_XFUNC_DECL *) fdecl );
      
      EVAL_do_function_param( out, cthread, scl, frame_start );
      
      if (tracer) {
        DBUF_add(  &tracer->text, " )", 2 );
        EVAL_CTX_show( out, tracer, "..." );
        prev = TRACE_inc( &out->trace_out );
      }
  
      EVAL_THREAD_call_xfunc( cthread, frame_start, (AST_XFUNC_DECL *) fdecl );
 
      if (tracer) {
        has_return_value = ((AST_XFUNC_DECL *) fdecl)->return_type_value != 0; 
        TRACE_set( &out->trace_out, prev );
      }
    }

    if (tracer && has_return_value ) {
     DBUF_add(  &tracer->text, ":", 1 );
     BINDING_DATA_prints( &tracer->text, EVAL_THREAD_get_stack_top( cthread ), 0 ); 
    }
    return has_return_value;
}

void EVAL_do(  EVAL_CTX *out )
{
  size_t i;
  EVAL_TRACE_ENTRY *trace = 0;
  EVAL_THREAD *cthread;
  AST_BASE *base;
  BINDING_DATA *data;
  int old_level;

  cthread = out->context.current_thread;
  base = cthread->instr;
  
  assert(base );

  
  switch( base->type ) {
  case S_EXPRESSION:
    EVAL_do_expr( out, (AST_EXPRESSION *) base );
    break;

  case S_AST_LIST: {
    AST_BASE_LIST *scl  = (AST_BASE_LIST *) base;
    DRING *cur;

    DRING_FOREACH( cur, &scl->statements ) {
	
	cthread->instr = _OFFSETOF( cur, AST_BASE, entry ); 
	EVAL_do( out );
	if (out->loop_exit != LOOP_NO_BREAK) {
	  break;
	}
    }
    }
    break;

  case S_AST_VECTOR: {
    AST_VECTOR *vect = (AST_VECTOR *)base;

    for( i = 0; i < AST_VECTOR_size( vect ); i++) {
	cthread->instr = AST_VECTOR_get( vect, i ); 
	EVAL_do( out );
    }
    }
    break;
  
  case S_ASSIGNMENT:
  {
    AST_ASSIGNMENT *scl = (AST_ASSIGNMENT *) base;
    EVAL_THREAD *cthread;
    EVAL_TRACE_ENTRY *trace_lhs;
    AST_EXPRESSION *lhs;
    AST_VAR_TYPE offending_type;
    BINDING_DATA *dref;

    if (EVAL_trace_on(out)) {
      trace = EVAL_CTX_new_trace( out, &scl->base);

      DBUF_add( &trace->text,"... = ", 6);
    }


    EVAL_do_expr( out, scl->right_side );
    
    // top of the stack is right hand value.
    cthread = out->context.current_thread;
    data = EVAL_THREAD_get_stack_top( cthread );
 
    lhs = (AST_EXPRESSION *) scl->left_side; 
    if (lhs->exp_type != S_EXPR_LIST_VALUES) { // not multi value left hand side
       assert( lhs->exp_type ==  S_EXPR_REFERENCE );

       dref = BINDING_DATA_follow_ref( data );     
       if (can_assign( AST_EXPRESSION_type( lhs ), dref->b.value_type, &offending_type)) {
	     EVAL_error( out , &lhs->base, "Can't assign the %s value to %s", get_type_name( dref->b.value_type ), lhs->val.ref.lhs);
       }

       if (trace) {
         // show current assignment 
	 trace_lhs = EVAL_CTX_new_trace( out, &scl->base);
       }


       EVAL_reference( out, scl->left_side, 1 , scl->type );
  
       if (trace) {
         const char *msg;

//       DBUF_add(  &trace_lhs->text, lhs->val.ref.lhs, strlen(lhs->val.ref.lhs) );
         DBUF_add(  &trace_lhs->text, " =", 2  );

	 msg = memchr( trace->text.buf, '=', trace->text.buf_used );
	 assert(msg != 0 );
         DBUF_add(  &trace_lhs->text, msg + 1,  trace->text.buf_used - (msg - (char *) trace->text.buf) - 1);
  
         EVAL_CTX_free_trace( out, SHOW ); 	  
         EVAL_CTX_free_trace( out, DISCARD );
       }

    } else { // multivalue assignment
       AST_VECTOR *values;
       BINDING_DATA *binding_rhs;


       if (data->b.value_type != S_VAR_LIST) {
	 EVAL_error( out , &scl->left_side->base,  "expects list to be returned, instead got %s", get_type_name( data->b.value_type ) );
       }


       if (EVAL_trace_on(out)) {
         // show current assignment 
	 trace_lhs = EVAL_CTX_new_trace( out, &scl->base);
         DBUF_add(  &trace_lhs->text, "[ ", 2 ); 
       }

       values = (AST_VECTOR *) lhs->val.index_expr;
       for( i = 0; i < AST_VECTOR_size( values ); i++) {
	 lhs = (AST_EXPRESSION *) AST_VECTOR_get( values, i );
	 binding_rhs = VALARRAY_get( &data->b.value.array_value, i );
       
         if (EVAL_trace_on(out)) {
           DBUF_add(  &trace_lhs->text, " , ", 3 ); 
         }

	 if ( lhs->exp_type  != S_EXPR_PLACEHOLDER) {
	   assert( lhs->exp_type == S_EXPR_REFERENCE );

	   // assign value to lhs.
	   EVAL_reference( out, lhs, 1, scl->type );
 	   

	   if (EVAL_trace_on(out)) {
             DBUF_add(  &trace_lhs->text, lhs->val.ref.lhs, strlen(lhs->val.ref.lhs) );
             DBUF_add(  &trace_lhs->text, ":", 1 );
             BINDING_DATA_prints( &trace_lhs->text, binding_rhs, 0 ); 
	   }

	 } else {
	   if (EVAL_trace_on(out)) {
	     DBUF_add(  &trace_lhs->text, "_", 1 ); 
           }
	 }
       }
       
       if (EVAL_trace_on(out)) {
         DBUF_add(  &trace_lhs->text, "] ", 2 ); 
       }
    }

    EVAL_THREAD_discard_pop_stack( cthread ); // right hand side gone now.
  }
    break;
  
  case S_IF: {
    AST_COND *cond,*last_cond;
    int is_first = 1;
   
    for( cond = (AST_COND *) base; cond; cond = cond->elsecond ) {
      last_cond = cond;
      if (cond->condition) {  
	if (EVAL_trace_on(out)) {
          trace = EVAL_CTX_new_trace( out, &cond->base);
	  if (is_first) {
	    DBUF_add( &trace->text,"if ", 3);
	    is_first = 0;
	  } else {
	    DBUF_add( &trace->text,"else if ", 8);
	  }
        }

        EVAL_do_expr( out, cond->condition );
        
        if (trace) {
          EVAL_CTX_free_trace( out, SHOW ); 	  
        }
    
        data = EVAL_THREAD_pop_stack( cthread );
        if (! is_numeric_type( data->b.value_type ) ) {
          EVAL_error( out , &cond->base,  "Condition requires numeric expression" );
        }
        if (BINDING_DATA_is_zero( data ) ) {
          continue;
        }
 
        if (trace) {
	  old_level = TRACE_inc( &out->trace_out );
        }

        cthread->instr = (AST_BASE *) cond->block; 
        EVAL_do( out );

        if (trace) {
	  TRACE_set( &out->trace_out, old_level );
        }
 
      } else {
       	assert( cond->block );
 
	if (EVAL_trace_on(out)) {
	  TRACE_println( &out->trace_out, cond->base.location.first_line ,  "else", 4 );
	  old_level = TRACE_inc( &out->trace_out );
	}
        
	cthread->instr = (AST_BASE *) cond->block;  
        EVAL_do( out );
        
        if (EVAL_trace_on(out)) {
	  TRACE_set( &out->trace_out, old_level );
	}
      }
      if (out->loop_exit != LOOP_NO_BREAK ) {
        break;
      }
    }
    if (EVAL_trace_on(out)) {
      TRACE_println( &out->trace_out, last_cond->base.location.last_line ,  "end # if", 8  );
    }
    }
    break;
 
  case S_WHILE: {
    AST_WHILE_LOOP *scl = (AST_WHILE_LOOP *) base;
    int do_loop;

    for(out->loop_exit = LOOP_NO_BREAK, do_loop = 1; ;) {
      if (EVAL_trace_on(out)) {
	 switch( scl->type ) {
          case LOOP_PRECOND_WHILE:
            trace = EVAL_CTX_new_trace( out, &scl->base);
	    DBUF_add( &trace->text,"while ", 6);
            break;
          case LOOP_POSTCOND_WHILE:
          case LOOP_INFINITE:
            TRACE_println( &out->trace_out, base->location.first_line ,  "loop", 4  );
            break;
	  }  
      }

      if (scl->type == LOOP_PRECOND_WHILE) {
        EVAL_do_expr( out, scl->condition );
        if (trace) {
          EVAL_CTX_free_trace( out, SHOW ); 	  
        }
        data = EVAL_THREAD_pop_stack( cthread );
        if (! is_numeric_type( data->b.value_type ) ) {
          EVAL_error( out , &scl->condition->base,  "Condition requires numeric expression" );
        }
        if (BINDING_DATA_is_zero( data ) ) {
          break;
	}
      }

      if (EVAL_trace_on(out)) {
        old_level = TRACE_inc( &out->trace_out );
      }

      out->context.current_thread->instr = (AST_BASE *) scl->block;  
      EVAL_do( out );
      out->context.current_thread->instr = base;

      if (EVAL_trace_on(out)) {
        TRACE_set( &out->trace_out, old_level );
      }

      if (out->loop_exit) {
        switch( out->loop_exit) {
         case LOOP_BREAK: 
	  out->loop_exit = LOOP_NO_BREAK;
	  do_loop = 0;
	  break;
         case LOOP_CONTINUE:
          if (EVAL_trace_on(out)) {
            TRACE_println( &out->trace_out, base->location.first_line ,  "next", 4  );
          }
 	  out->loop_exit = LOOP_NO_BREAK;
	  break;
	 case EXIT_FUNCTION:
	  do_loop = 0;
	  return;
	 default:
	  ;
        }
      }

      if (!do_loop) {
        break;
      }

      switch(scl->type) {
      
      case LOOP_POSTCOND_WHILE: {
        if (EVAL_trace_on(out)) {
          trace = EVAL_CTX_new_trace( out, &scl->base);
	  DBUF_add( &trace->text,"while ", 6);
	}
        EVAL_do_expr( out, scl->condition );
        if (trace) {
          EVAL_CTX_free_trace( out, SHOW ); 	  
        }
        data = EVAL_THREAD_pop_stack( cthread );
        if (! is_numeric_type( data->b.value_type ) ) {
          EVAL_error( out , &scl->condition->base,  "Condition requires numeric expression" );
        }
        if ((data->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL ) {
	  break;
        }
      }
      break;
      default:
	  if (EVAL_trace_on(out)) {
	    TRACE_println( &out->trace_out, base->location.last_line ,  "end", 3  );
	  }
      }
    }
    if (EVAL_trace_on(out)) {
      TRACE_println( &out->trace_out, base->location.last_line ,  "end # finish loop", 17  );
    }

   }
    break;

  case S_FOR:  {
#if 0  
    AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
    EVAL_do( out, (AST_BASE *) scl->loop_var );
    fprintf( out, " , ");
    EVAL_do( out, (AST_BASE *) scl->loop_expr );
    fprintf( out, " , ");
    EVAL_do( out, (AST_BASE *) scl->block );
    newline = 1;
#endif    
    }
    break;
  
  case S_FUN_CALL: {
    AST_FUNC_CALL *scl = (AST_FUNC_CALL *) base;
    int show;

    if (EVAL_trace_on(out)) {
      trace = EVAL_CTX_new_trace( out, &scl->base);
    }

    show = EVAL_do_function( out, scl );
    data = EVAL_THREAD_pop_stack( cthread );
    BINDING_DATA_free( data );   
    
    if (trace) {
       EVAL_CTX_free_trace( out,  show ? SHOW : DISCARD ); 	  
    }
  }
    break;

  case S_RETURN: {
    AST_RETURN *scl = (AST_RETURN *) base;
    BINDING_DATA *rval, *top;
	
	
    if (EVAL_trace_on(out)) {
      trace = EVAL_CTX_new_trace( out, &scl->base);
      DBUF_add( &trace->text,"return ", 7);
    }  

    if (scl->rvalue) {
      EVAL_do_expr( out,  scl->rvalue  );
      
      // move the return value (top) to slot for function return value.
      rval = EVAL_THREAD_get_stack_top( cthread ); 
      if (rval->b.value_flags_ref == S_VAR_REF_STACK2STACK ) {
	rval = BINDING_DATA_follow_ref( rval );
      }

      top = EVAL_THREAD_stack_frame_offset( cthread, 0); 
      BINDING_DATA_move( top,  rval ); 
      	

      cthread->binding_data_stack.elmcount --; // discard top of stack without destructor (have moved it to ret value).

      // return to parent frame.
      EVAL_THREAD_pop_frame( cthread );

      out->loop_exit = EXIT_FUNCTION;
    }

    if (trace) {
          EVAL_CTX_free_trace( out, SHOW ); 	  
    }
    }
    break;

  case S_NEXT:
    if (EVAL_trace_on(out)) {
      TRACE_println( &out->trace_out, base->location.last_line ,  "next", 4  );
    }
    out->loop_exit = LOOP_CONTINUE;
    break;
  
  case S_BREAK:
    if (EVAL_trace_on(out)) {
      TRACE_println( &out->trace_out, base->location.last_line ,  "break", 5  );
    }
    out->loop_exit = LOOP_BREAK;
    break;

  case S_FUN_DECL: {
     }
    break;

  default:
    assert(0);
  }
}

int EVAL_run(  EVAL_CTX *out, AST_BASE *base )
{
  EVAL_CONTEXT *ctx;
  AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) base;

  ctx = &out->context;

  if (setjmp(  ctx->jmpbuf) ) { 
     return -1;
  } else { 
     ctx->is_jmpbuf_set = 1; 
  } 
  EVAL_CONTEXT_start( ctx );
 
  ctx->main.instr = (AST_BASE *) fdecl->func_body;
   
  EVAL_do( out );

  return 0;
}

int EVAL_free( EVAL_CTX *out )
{
  EVAL_CONTEXT *ctx;

  ctx = &out->context;

  EVAL_CONTEXT_free( ctx );

  return 0;
}


