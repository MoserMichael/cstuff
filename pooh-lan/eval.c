#include <pooh-lan/parser.h>
#include <pooh-lan/ast.h>
#include <pooh-lan/pars.h>
#include <pooh-lan/rtlib/rtlib.h>
#include <corothread/cthread.h>
#include <eval.h>
#include <stdarg.h>
#include <math.h>

int can_assign( AST_VAR_TYPE lhs_type, AST_VAR_TYPE rhs_type, AST_VAR_TYPE *offending_type);

struct tagEVAL_CTX;


BINDING_DATA *EVAL_reference_scalar( EVAL_CTX *out, AST_EXPRESSION *expr, BINDING_DATA *new_value, EVAL_REF_KIND copy_type);
void EVAL_reference( EVAL_CTX *out, AST_EXPRESSION *expr, BINDING_DATA *nvalue, EVAL_REF_KIND  copy_type, VALFUNC_PRINT_MODE rval_method_hide_sig);

void EVAL_do_expr( struct tagEVAL_CTX *out, AST_EXPRESSION *expr, EVAL_REF_KIND get_value_ref, int get_function);
int  EVAL_do_function( struct tagEVAL_CTX *out , AST_FUNC_CALL *scl, int is_threa, EVAL_THREAD **newthread );
void EVAL_do(  struct tagEVAL_CTX *out );
int EVAL_thread_create(AST_BASE *fdecl, size_t frame_start, EVAL_THREAD *arg_thread, void *eval_ctx);
int EVAL_thread_resume_do( EVAL_THREAD *thread, BINDING_DATA *resume_msg, BINDING_DATA **yield_value );

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

void show_func_prototype( EVAL_TRACE_ENTRY *trace, AST_BASE *fdecl );


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

  if (CTHREAD_libinit()) {
    return -1;
  }

  if (STACKS_init_on_demand( &out->stacks, -1, 15 )) {
    return -1;
  }
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
  POOH_INT rval,lval,ret;
  BINDING_DATA *retval;
  EVAL_THREAD *cthread;
 
  cthread = out->context.current_thread;
  lval = lhs->b.value.long_value;
  rval = rhs->b.value.long_value;

  
  switch( op ) {
    case TK_OP_NUM_POW:
      ret = pow(lval , rval );
      break;
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
      ret = lval / rval;
#if 0      
      EVAL_THREAD_discard_pop_stack( cthread );
      EVAL_THREAD_discard_pop_stack( cthread );

      retval = EVAL_THREAD_push_stack( cthread, S_VAR_DOUBLE );  
      retval->b.value.double_value = (double) lval / rval;
      return retval;
#endif      
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
    POOH_INT rvalt;
 	
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
    case TK_OP_NUM_POW:
      ret = pow(lval, rval);
      break;
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
    retval->b.value.long_value = (POOH_INT) ret;
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
 
  EVAL_do_expr( out, expr->val.expr.expr_left, PARAM_BY_VAL, 0 );
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


  EVAL_do_expr( out, expr->val.expr.expr_right, PARAM_BY_VAL, 0 );
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
  POOH_INT rn;
  DBUF tmpl, tmpr;
  
   if ((lhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
    BINDING_DATA_move( lhs, rhs );
    EVAL_THREAD_discard_pop_stack( cthread );
    return lhs;
  }
  
  if ((rhs->b.value_type & S_VAR_ALL_TYPES) == S_VAR_NULL) {
    EVAL_THREAD_discard_pop_stack( cthread );
    return lhs;
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
        
	VALSTRING_set( &tmpres.b.value.string_value, (const char *) tmpl.buf, tmpl.buf_used );
     }
     if ((rhs->b.value_type & S_VAR_ANY) == S_VAR_STRING) {
	VALSTRING_append( &tmpres.b.value.string_value,  &rhs->b.value.string_value );
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


BINDING_DATA *EVAL_reference_scalar( EVAL_CTX *out, AST_EXPRESSION *expr, BINDING_DATA *new_value, EVAL_REF_KIND copy_type)
{
    EVAL_THREAD *cthread;
    BINDING_DATA *data = 0,*arec;
    BINDING_ENTRY *bentry;
    VALACTIVATION *activation_rec;
    VALFUNCTION *val_func;
    int scope = REF_SCOPE_THIS; 

    cthread = out->context.current_thread;
#if 0
    scope = expr->val.ref.scope;
#endif    
    bentry = expr->val.ref.binding;
    if (bentry) {
      scope = bentry->scope;
    }

    switch( scope ) {
      case REF_SCOPE_LOCAL: 
        data = (BINDING_DATA *) EVAL_THREAD_stack_frame_offset( cthread, bentry->stack_offset );
	  assert( data != 0 );
        if (new_value) {
 	  BINDING_DATA_copy_ext( data, new_value, copy_type );
        }
        return data;
        break;
    
      case REF_SCOPE_GLOBAL: {
        ARRAY *glob;
   
        glob =  &out->context.bindings_global;	
        if (new_value) {
          data = (BINDING_DATA *) ARRAY_at( glob, bentry->stack_offset);
	  assert( data != 0 );
	  BINDING_DATA_copy_ext( data, new_value, copy_type );
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
        break;

      default:

        arec = EVAL_THREAD_stack_frame_offset( cthread, 1 );
        activation_rec = &arec->activation_record;
        assert( activation_rec );
        val_func = activation_rec->function_object;
 
        switch( scope ) {
    
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
           case REF_SCOPE_CLOSURE: {
             VALFUNCTION_CAPTURE *cap;

             assert( val_func->pcaptures );
             assert( bentry->stack_offset < val_func->ncaptures );
             cap = &val_func->pcaptures[ bentry->stack_offset ];

             //fprintf(stderr,"cap ref %p func-obj %p cap-offset %d \n", cap, val_func, bentry->stack_offset );
             if (cap->next.next == 0) {
               data = cap->value.ref;
             } else {
               size_t pos = cap->value.stack_ref;
               assert( pos < ARRAY_size( &cthread->binding_data_stack ) );
	       data = ((BINDING_DATA *) cthread->binding_data_stack.buffer) + pos;
             }
             assert( data );
             if (new_value) {
	       BINDING_DATA_copy_ext( data, new_value, copy_type );
             }
           }
             break;
           default:
             assert(0);
         }
    }
    return data;
}

void EVAL_reference( EVAL_CTX *out, AST_EXPRESSION *expr,  BINDING_DATA *nvalue, EVAL_REF_KIND copy_type,  VALFUNC_PRINT_MODE rval_method_hide_sig)
{
  EVAL_THREAD *cthread;
  BINDING_ENTRY *entry;
  BINDING_DATA *data;
  EVAL_TRACE_ENTRY *tracer = 0;
  AST_VECTOR *vect;
  int in_assignment = (int) copy_type >= (int) COPY_SINGLE_ASSIGN_BY_VAL;
  
  entry = expr->val.ref.binding;
  cthread = out->context.current_thread;
  
  if (EVAL_trace_on(out) ) {
      if (rval_method_hide_sig == VALFUNC_HIDE_METHOD_SIG && !in_assignment) { 
        VALFUNCTION_print_mode( VALFUNC_HIDE_METHOD_SIG );
      }
      
      tracer =  out->top_trace;

      switch(expr->val.ref.scope) {
        case REF_SCOPE_CLOSURE:
	    DBUF_add(  &tracer->text, "outer.", 6 ); 
       	    break;
	case REF_SCOPE_THIS:
       	    //DBUF_add(  &tracer->text, "this.", 5 ); 
            break;
	case REF_SCOPE_GLOBAL:
	    if (! EVAL_CONTEXT_is_main_scope( &out->context ))
		DBUF_add(  &tracer->text, "global.", 7 ); 
       	    break;
        case REF_SCOPE_LOCAL:
	    break;
      }
  }
 
#if 1 
  if (tracer) { 
    DBUF_add(  &tracer->text, expr->val.ref.lhs, strlen( expr->val.ref.lhs ) );
  }
#endif  

  entry = expr->val.ref.binding;
  vect = expr->val.ref.indexes;

  if (expr->val.ref.indexes == 0 ) { // scalars
      if (in_assignment == 0) { // scalars value lookup
       nvalue = EVAL_THREAD_push_stack( cthread, S_VAR_NULL ); // value lookup puts a new value on the stack
       data = EVAL_reference_scalar( out, expr, 0, copy_type);

       // copy to stack what is easy to copy, put in ref otherwise.
       BINDING_DATA_copy_ext( nvalue , data, copy_type ); 
       if (tracer) { 
 	 DBUF_add(  &tracer->text, ":", 1 );
	 BINDING_DATA_prints( &tracer->text, data, 0 );
	 VALFUNCTION_print_mode( VALFUNC_SHOW_METHOD_SIG );
       }	
      } else { // scalar assignment
	   if (!nvalue) {
	     nvalue = EVAL_THREAD_get_stack_top( cthread );
	   }	 
	   EVAL_reference_scalar( out, expr, nvalue, copy_type);
	 } 
      } else { // collections

	 AST_EXPRESSION *index_expr,*follow_index_expr;
	 BINDING_DATA *index_data, *next_data, *tmp;
	 size_t i;
	 double nval;
	 AST_VAR_TYPE collection_type;

	 // put reference to the collection - on the stack.
	 EVAL_THREAD_push_stack( cthread, S_VAR_NULL );
	 data = EVAL_reference_scalar( out, expr, 0, copy_type);
	 BINDING_DATA_copy( EVAL_THREAD_get_stack_top( cthread ), data, CP_REF ); 
	 
	 
	 for( i = 0; i < AST_VECTOR_size( vect ); ) {
	    index_expr = (AST_EXPRESSION *) AST_VECTOR_get( vect , i  );
	    follow_index_expr = (i + 1) < AST_VECTOR_size( vect ) ? (AST_EXPRESSION *) AST_VECTOR_get( vect, i + 1 ) : 0;

	    // evaluate the index expression.
	    EVAL_do_expr( out, index_expr, PARAM_BY_VAL, 0 );
	    index_data = EVAL_THREAD_pop_stack( cthread );

	    // perform the collection lookup - get pointer to collection( is on stack, right before index expression)
	    data = index_data - 1; 
	    data = BINDING_DATA_follow_ref( data );
	    collection_type = data->b.value_type;
    

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

	    // for array collection - check that the index is a number (must).
	    if (index_expr->exp_type == S_EXPR_ARRAY_INDEX) {
	      if (! is_numeric_type( index_data->b.value_type ) ) {
		EVAL_error( out, &expr->base, "Expects number as array index for %s instead got %s", expr->val.ref.lhs, get_type_name( data->b.value_type ) );
	      }
	    }

	
	    // if last index and function is used in assingnment - insert value into collection
	    if ( follow_index_expr == 0 && in_assignment) {
		if (index_expr->exp_type == S_EXPR_HASH_INDEX) {  // insert into hash 
		    tmp = VALHASH_set_entry( &data->b.value.hash_value, index_data );	
		} else {					      // insert into vector
		    BINDING_DATA_get_double( index_data, &nval );
		
		    if (nval < 1) {
			 EVAL_error( out , &index_expr->base, "Array index must be greater or equal to one. instead got %d", (long) nval );
		    }
		    tmp = VALARRAY_set_entry( &data->b.value.array_value, (long) nval - 1 );  
		}
		BINDING_DATA_copy_ext( tmp,  index_data - 2 , copy_type ); 
      
		if (tracer) { 
		    DBUF_add(  &tracer->text, ":", 1 );
		    tmp = BINDING_DATA_follow_ref( tmp );
		    BINDING_DATA_prints( &tracer->text, tmp, 0 );
		}	
     
		EVAL_THREAD_pop_stack( cthread );
		return;

	    } else { // not the last index - lookup collection value. 

		if (index_expr->exp_type == S_EXPR_HASH_INDEX) {
		  VALHASH_find( &data->b.value.hash_value , index_data, &next_data );
		  if (follow_index_expr != 0) {
		    if (next_data == 0) {  
		      next_data = VALHASH_set_entry( &data->b.value.hash_value, index_data );	
		      BINDING_DATA_init( next_data, follow_index_expr->exp_type == S_EXPR_HASH_INDEX ? S_VAR_HASH : S_VAR_LIST ); 
		    }  
		  }
		} else {
		  BINDING_DATA_get_double( index_data, &nval );

		  if (nval < 1 ) {
		    EVAL_error( out, &index_expr->base, "Sequence index starts with 1, instead got %ld", nval );
		  }
		  next_data = VALARRAY_get_n( &data->b.value.array_value, (long) nval - 1 );
		  if (follow_index_expr != 0) {
		    if (next_data == 0) {
		      next_data = VALARRAY_set_entry( &data->b.value.array_value, (long) nval - 1 );
		      BINDING_DATA_init( next_data, follow_index_expr->exp_type == S_EXPR_HASH_INDEX ? S_VAR_HASH : S_VAR_LIST ); 
		      next_data->b.value_flags_val = S_VAR_HEAP_VALUE;
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
		  next_data->b.value_flags_val = S_VAR_HEAP_VALUE;
		}
	    }

	    if (follow_index_expr == 0) 
		  break;

	    i += 1;
	 } // end for (indexes)

       
	 if (tracer) { 
	   DBUF_add(  &tracer->text, ":", 1 );
	   data = EVAL_THREAD_get_stack_top( cthread ); 
	   data = BINDING_DATA_follow_ref( data );
	   BINDING_DATA_prints( &tracer->text, data, 0 );
       VALFUNCTION_print_mode(  VALFUNC_SHOW_METHOD_SIG );
     }	
  
  } // end else (collection)
  
}

void EVAL_do_expr( EVAL_CTX *out, AST_EXPRESSION *expr, EVAL_REF_KIND get_value_ref, int get_function)
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

        EVAL_do_expr( out, expr->val.expr.expr_left, PARAM_BY_VAL, 0 );
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

	  EVAL_do_expr( out, expr->val.expr.expr_right, PARAM_BY_VAL, 0 );
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

	   EVAL_do_expr( out, expr->val.expr.expr_right, PARAM_BY_VAL, 0 );
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
     break;

    case S_EXPR_UNARY:

      if (tracer)  {
	 const char *op_name = get_op_name(expr->val.unary.op);  
         DBUF_add(  &tracer->text, " ", 1);
         DBUF_add(  &tracer->text, op_name, 1 );
         DBUF_add(  &tracer->text, " ", 1);
      }	
      switch( expr->val.unary.op )
      { 
      case TK_OP_NUM_SUBST:
        EVAL_do_expr( out, expr->val.unary.expr, PARAM_BY_VAL, 0 );
        ret = EVAL_THREAD_get_stack_top( cthread );
        ret = BINDING_DATA_follow_ref( ret );

        if ( ! is_numeric_type( ret->b.value_type )) {
	   EVAL_error( out, &expr->base , "Operator - expects number as value, instead got %s", get_type_name( ret->b.value_type ) );
 
        }
        if ( ret->b.value_type == S_VAR_INT) {
	  ret->b.value.long_value = - ret->b.value.long_value;
        } else {
          ret->b.value.double_value = - ret->b.value.double_value;
        }
	break;
      case TK_OP_NUM_ADD:
        EVAL_do_expr( out, expr->val.unary.expr, PARAM_BY_VAL, 0 );
	break;
      case TK_OP_LOGICAL_NEGATE:
        if (tracer)  {
          DBUF_add(  &tracer->text, " ! ", 3 );
        }	
        EVAL_do_expr( out, expr->val.unary.expr, PARAM_BY_VAL, 0 );
        ret = EVAL_THREAD_get_stack_top( cthread );
        ret = BINDING_DATA_follow_ref( ret );

        if ( ! is_numeric_type( ret->b.value_type )) {
	   EVAL_error( out, &expr->base , "Operator ! expects number as value, instead got %s", get_type_name( ret->b.value_type ) );
        }
        if ( ret->b.value_type == S_VAR_INT) {
	  ret->b.value.long_value = ret->b.value.long_value == 0;
        } else {
          ret->b.value.double_value = ret->b.value.double_value == 0;
        }
  	break;
      }
      break;

    case S_EXPR_REFERENCE: {
      EVAL_reference( out, expr, 0, get_value_ref, get_function ? VALFUNC_SHOW_METHOD_SIG : VALFUNC_HIDE_METHOD_SIG );
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
     EVAL_do_function( out , expr->val.fcall, 0, 0 );
     break;

    case S_EXPR_HASH_INDEX:
      if (tracer)  {
        DBUF_add(  &tracer->text, "{", 1 );
      }
      EVAL_do_expr( out, expr->val.index_expr, 0, 0 );
      if (tracer)  {
        DBUF_add(  &tracer->text, "}", 1 );
      }
      break;
  
    case S_EXPR_ARRAY_INDEX:
      if (tracer)  {
        DBUF_add(  &tracer->text, "[", 1 );
      }
      EVAL_do_expr( out, expr->val.index_expr, 0, 0 );
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
         EVAL_do_expr( out, expr, 0, 0 );
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
	 EVAL_do_expr( out, expr->val.expr.expr_left, 0, 0 );
	 
	 if (tracer) {
	    DBUF_add(  &tracer->text, " : ", 3 );
	 }

	 EVAL_do_expr( out,  expr->val.expr.expr_right, 0, 0 );
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
      VALFUNCTION *val_func, *valfunc;
      VALACTIVATION *activation_rec;
      BINDING_DATA *arec, *func;
      AST_FUNC_DECL *ffdecl;
  
      fdecl = expr->val.fdecl;
      assert( fdecl );

      if (fdecl->type == S_XFUN_DECL) {
rvall:      
        ret = EVAL_THREAD_push_stack( cthread, S_VAR_CODE );
        valfunc = &ret->b.value.func_value;
        valfunc->fdecl = fdecl;

	if (tracer)
	    show_func_prototype( tracer, fdecl );
        return;
      }
 
      ffdecl = (AST_FUNC_DECL *) fdecl;
      assert( fdecl->type == S_FUN_DECL );

      if (ffdecl->f_name != 0) {
         goto rvall;
      }

      if (ARRAY_size( &ffdecl->outer_refs ) == 0 && ffdecl->num_nested_func_decl == 0 ) { 
        goto rvall;
      }

      if (tracer)
        show_func_prototype( tracer, fdecl );

      // check if current frame is called as a lambda function with nested declarations. if it is then return exiting function object created for nested declaration
      arec = EVAL_THREAD_stack_frame_offset( cthread, 1 );
      activation_rec = &arec->activation_record;
      assert( activation_rec );
      val_func = activation_rec->function_object;
      
      if (val_func && val_func->pnested_closures) {  

        // if current evaluated function is a closure, then we want to return function object that is nested in current declaration.
        func = val_func->pnested_closures[ ffdecl->this_func_decl_index ];
 
        valfunc = &func->b.value.func_value;
 
      } else {
      
        // top level function object returned. 
        ret = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );

        func = BINDING_DATA_MEM_new( S_VAR_CODE );


        valfunc = &func->b.value.func_value;
        valfunc->fdecl = fdecl;
        
        VALFUNCTION_init_cap( valfunc, ARRAY_size( &ffdecl->outer_refs ), ffdecl->num_nested_func_decl );
       } 

      ret = EVAL_THREAD_push_stack( cthread, S_VAR_NULL );
      BINDING_DATA_copy( ret, func , CP_REF );   
     
      VALFUNCTION_init_outer_ref_tree( cthread, valfunc, ffdecl, 0 );

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

void EVAL_do_function_param( EVAL_CTX *out , EVAL_THREAD *cthread, AST_FUNC_CALL *scl, size_t frame_start, int is_xfunc_decl )
{
      size_t i;
      AST_FUNC_CALL_PARAM *param;  
      EVAL_TRACE_ENTRY *tracer = 0;
      BINDING_DATA *param_value,*param_value_r;
      AST_VAR_TYPE vtype;

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

        if ( is_xfunc_decl ) {
           AST_XFUNC_PARAM_DECL *xdecl = (AST_XFUNC_PARAM_DECL *) param->param_decl; 
	   vtype = xdecl->var_type;
        } else {
           AST_EXPRESSION *paramdecl = (AST_EXPRESSION *) param->param_decl;
	   vtype = paramdecl->value_type;
        }   
  

//	EVAL_reference( out, param->expr, 0, CP_VALUE );
	EVAL_do_expr( out,  param->expr, vtype & S_VAR_PARAM_BYREF ? PARAM_BY_REF : PARAM_BY_VAL, 0 );


        param_value_r = EVAL_THREAD_pop_stack( cthread );  

        param_value = BINDING_DATA_follow_ref( param_value_r );
        if ( is_xfunc_decl ) {

           AST_XFUNC_PARAM_DECL *xdecl = (AST_XFUNC_PARAM_DECL *) param->param_decl; 
           if ((xdecl->var_type & S_VAR_ANY) != S_VAR_ANY &&
               (xdecl->var_type & param_value->b.value_type) == 0 )
           {
              EVAL_error( out, &param->base, "Parameter ~%s expects %s type. instead got %s", param->label_name, get_type_name(xdecl->var_type), get_type_name2( param_value->b.value_type )   );
           }
        } else {
           AST_EXPRESSION *paramdecl = (AST_EXPRESSION *) param->param_decl;
           BINDING_ENTRY *bentry = paramdecl->val.ref.binding;
           if ((bentry->value_type  & S_VAR_ANY) != S_VAR_ANY &&
               (bentry->value_type & param_value->b.value_type) == 0 )
           {
              // error
              EVAL_error( out, &param->base, "Parameter ~%s expects %s type. instead got %s", param->label_name, get_type_name( bentry->value_type ), get_type_name2( param_value->b.value_type ) );
           }
        }

	BINDING_DATA_move( EVAL_THREAD_parameter_ref( cthread, frame_start, param->param_num ), param_value_r );
      }
}

void show_func_prototype( EVAL_TRACE_ENTRY *trace, AST_BASE *fdecl )
{
     DBUF_add( &trace->text,"sub ", 4);
     size_t i;

     if (fdecl->type ==  S_FUN_DECL) {
	AST_FUNC_DECL *ffdecl;  
	AST_EXPRESSION *param_decl;

	ffdecl = (AST_FUNC_DECL *) fdecl;
	if (ffdecl->f_name)
	   DBUF_add( &trace->text, ffdecl->f_name, strlen( ffdecl->f_name ) );
        DBUF_add( &trace->text,"(", 1 );	  
	for( i = 0; i< AST_VECTOR_size( ffdecl->func_params ); i++ ) {
	   if (i > 0)
		DBUF_add( &trace->text, " , ~", 4 );
	   else
		DBUF_add( &trace->text, "~", 1 );
	   param_decl = (AST_EXPRESSION *) AST_VECTOR_get( ffdecl->func_params, i );
      
	   DBUF_add( &trace->text, param_decl->val.ref.lhs, strlen( param_decl->val.ref.lhs ) );
	}		
        DBUF_add( &trace->text,")", 1 );	  
     } else {
	AST_XFUNC_DECL *fxdecl;
	AST_XFUNC_PARAM_DECL *xparam_decl;

	assert( fdecl->type == S_XFUN_DECL );
	fxdecl = (AST_XFUNC_DECL *) fdecl;

	if (fxdecl->f_name)
	   DBUF_add( &trace->text, fxdecl->f_name, strlen( fxdecl->f_name ) );
        DBUF_add( &trace->text,"(", 1 );	  
        for( i = 0; i < fxdecl->nparams; i++ ) {
	    if (i > 0)
		DBUF_add( &trace->text, " , ~", 4 );
	    else
		DBUF_add( &trace->text, "~", 1 );
  
	    xparam_decl = &fxdecl->params[ i ];
	    DBUF_add( &trace->text, xparam_decl->param_name, strlen( xparam_decl->param_name ) );
	 }
         DBUF_add( &trace->text,")", 1 );	  
    }
}

void EVAL_proceed_fun_call( size_t frame_start, AST_BASE *fdecl, VALFUNCTION *valfunc, void *oout  )
{
      AST_FUNC_DECL *ffdecl = (AST_FUNC_DECL *) fdecl;
      EVAL_THREAD *cthread;
      EVAL_CTX *out = (EVAL_CTX *) oout;   
      EVAL_TRACE_ENTRY *tracer; 
      int old_level = 0;
 
      
      cthread = out->context.current_thread;
 
      if (EVAL_trace_on(out) ) {
         tracer = EVAL_CTX_new_trace( out, fdecl);
   	 show_func_prototype( tracer, fdecl );  
         EVAL_CTX_free_trace( out, SHOW ); 	  
         old_level = TRACE_inc( &out->trace_out );
      }

      EVAL_THREAD_call_func( cthread, frame_start, ffdecl, valfunc );
      
      // do the function body.
      cthread->instr = (AST_BASE *) ffdecl->func_body;
      EVAL_do( out );

      out->loop_exit = 0;
      EVAL_THREAD_pop_frame( cthread );
 
      if (EVAL_trace_on(out) ) {
        TRACE_set( &out->trace_out, old_level );
        TRACE_println( &out->trace_out, fdecl->location.last_line , "end", 3 ); 	
      }
 
}      
 
int EVAL_do_function( EVAL_CTX *out , AST_FUNC_CALL *scl, int is_thread, EVAL_THREAD **newthread  )
{
    AST_EXPRESSION *f_name = scl->f_name;
    AST_BASE *fdecl;
    VALFUNCTION *valfunc = 0;
    BINDING_DATA *data;
    EVAL_THREAD *cthread;
    size_t frame_start;
    EVAL_TRACE_ENTRY *tracer = 0;
    int prev = 0;
    int has_return_value = 0;
    //int prev_val;

    cthread = out->context.current_thread;
 
    tracer = out->top_trace;
 
    if (f_name->exp_type != S_EXPR_LAMBDA_RESOLVED) {
      
      // evaluate expression that must return the function value.
      //prev_val =  out->context.trace_on; 
      //out->context.trace_on = 0; 
      EVAL_do_expr( out, f_name, PARAM_BY_REF, 1 );
      //out->context.trace_on = prev_val; 
      data = EVAL_THREAD_pop_stack( cthread );
      
      data = BINDING_DATA_follow_ref( data );
	
      if ((data->b.value_type & S_VAR_CODE) == 0) {
	EVAL_error( out, &f_name->base , "Expression must return function type, instead got %s", get_type_name( data->b.value_type ) );
      }

      if (!is_thread) {
        is_thread = data->b.value_type & S_VAR_CODE_THREAD;  
      }

      valfunc = &data->b.value.func_value;
      fdecl = valfunc->fdecl;

      // check that all parameters are supplied.
      if (CHECKER_check_func_call_params( out->ctx, 0, scl, fdecl ) ) {
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

      EVAL_do_function_param( out, cthread, scl, frame_start, 0 );

     } else {
      assert( fdecl->type == S_XFUN_DECL);
      
      frame_start = EVAL_THREAD_prepare_xcall( cthread, (AST_XFUNC_DECL *) fdecl );
      
      EVAL_do_function_param( out, cthread, scl, frame_start, 1 );
      
     }
     
     if (tracer) {
        DBUF_add(  &tracer->text, " )", 2 );
        EVAL_CTX_show( out, tracer, "..." );
        prev = TRACE_inc( &out->trace_out );
     }
    
     if (is_thread) {
        EVAL_THREAD *new_thread;

        if ( valfunc && valfunc->thread != 0 ) {
          EVAL_error( out , &scl->base,  "Can't start thread on this value - it is already running" );
        }
       
        new_thread = EVAL_THREAD_prepare_coroutine( cthread, frame_start, fdecl );
        
        if (newthread) {
          *newthread = new_thread;
        }

	if ( ! EVAL_thread_create( fdecl, frame_start, new_thread, out) ) {
	  if (valfunc)
          valfunc->thread = new_thread;
        } else {
       	  if (valfunc)
	  valfunc->thread = 0;
        }
	return 0;
      }

    if (fdecl->type == S_FUN_DECL) {
      AST_FUNC_DECL *ffdecl = (AST_FUNC_DECL *) fdecl;

      EVAL_THREAD_call_func( cthread, frame_start, ffdecl, valfunc );
      
      // do the function body.
      cthread->instr = (AST_BASE *) ffdecl->func_body;
      EVAL_do( out );

      out->loop_exit = 0;
      EVAL_THREAD_pop_frame( cthread );
 
      if (tracer) {
	has_return_value = ffdecl->return_type_value != 0; 
        TRACE_set( &out->trace_out, prev );
      }
 
    } else {
 
      EVAL_THREAD_call_xfunc( cthread, frame_start, (AST_XFUNC_DECL *) fdecl, valfunc );
 
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
  int old_level = 0;

  cthread = out->context.current_thread;
  base = cthread->instr;
  
  assert(base );

  
  switch( base->type ) {
  case S_EXPRESSION:
    EVAL_do_expr( out, (AST_EXPRESSION *) base, PARAM_BY_VAL, 0 );
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
    EVAL_TRACE_ENTRY *trace_lhs = 0;
    AST_EXPRESSION *lhs;
    AST_VAR_TYPE offending_type;
    BINDING_DATA *dref;

    if (EVAL_trace_on(out)) {
      trace = EVAL_CTX_new_trace( out, &scl->base);

      DBUF_add( &trace->text,"... ", 4);
      if (scl->type == CP_REF) {
        DBUF_add( &trace->text,":= ", 3) ;
      } else {
        DBUF_add( &trace->text,"= ", 2) ;
      }
    }

    EVAL_do_expr( out, scl->right_side, scl->type == CP_REF ? COPY_EVAL_BY_REF : COPY_EVAL_BY_VAL, 0 );
	
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

       EVAL_reference( out, scl->left_side, 0, scl->type == CP_REF ? COPY_SINGLE_ASSIGN_BY_REF : COPY_SINGLE_ASSIGN_BY_VAL, VALFUNC_SHOW_METHOD_SIG );
  

    } else { // multivalue assignment
       AST_VECTOR *values;
       BINDING_DATA *binding_rhs;

       data = BINDING_DATA_follow_ref( data );     
 
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
       
         if (EVAL_trace_on(out) && i > 0) {
           DBUF_add(  &trace_lhs->text, " , ", 3 ); 
         }

	 if ( lhs->exp_type  != S_EXPR_PLACEHOLDER) {
	   assert( lhs->exp_type == S_EXPR_REFERENCE );

	   // assign value to lhs.
	   EVAL_reference( out, lhs, binding_rhs , scl->type == CP_REF ? COPY_MULTI_ASSIGN_BY_REF : COPY_MULTI_ASSIGN_BY_VAL,  VALFUNC_SHOW_METHOD_SIG );

	   if (EVAL_trace_on(out)) {
             DBUF_add(  &trace_lhs->text, ":", 1 );
             BINDING_DATA_prints( &trace_lhs->text, binding_rhs, 0 ); 
	   }

	 } else {
	   if (EVAL_trace_on(out)) {
	     DBUF_add(  &trace_lhs->text, " _ ", 1 ); 
           }
	 }
       }
       
       if (EVAL_trace_on(out)) {
         DBUF_add(  &trace_lhs->text, "] ", 2 ); 
       }
    }

    if (trace) {
         const char *msg;

//       DBUF_add(  &trace_lhs->text, lhs->val.ref.lhs, strlen(lhs->val.ref.lhs) );
         if (scl->type == CP_REF) {
	   DBUF_add( &trace_lhs->text," :=", 3) ;
	 } else {
	    DBUF_add( &trace_lhs->text," =", 2) ;
	 }

	 msg = memchr( trace->text.buf, '=', trace->text.buf_used );
	 if (msg != 0) {
           DBUF_add(  &trace_lhs->text, msg + 1,  trace->text.buf_used - (msg - (char *) trace->text.buf) - 1);
         } else {
           DBUF_add(  &trace_lhs->text, trace->text.buf,  trace->text.buf_used );
	 }
         EVAL_CTX_free_trace( out, SHOW ); 	  
         EVAL_CTX_free_trace( out, DISCARD );
    }

// more efficient if enabled (assignment right hand side will not pop into heap), but loops with assignment need more stack
#if 0
    if (lhs->exp_type == S_EXPR_LIST_VALUES)  
#endif
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

        EVAL_do_expr( out, cond->condition, 0, 0 );
        
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
        break;
 
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
        EVAL_do_expr( out, scl->condition, 0, 0 );
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
        EVAL_do_expr( out, scl->condition, 0, 0 );
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
    AST_FOR_LOOP *scl = (AST_FOR_LOOP *) base;
    EVAL_THREAD *newthread;
    AST_EXPRESSION *exprvar = scl->loop_var;
    AST_EXPRESSION  *lhs, *lexpr = scl->loop_expr;
    BINDING_DATA *yield_value, *binding_rhs;
    EVAL_THREAD *cur_thread = out->context.current_thread;
    AST_VAR_TYPE offending_type;
    AST_VECTOR *values;
    int do_loop = 1;

    if (EVAL_trace_on(out)) {
      trace = EVAL_CTX_new_trace( out, &scl->base);
      DBUF_add( &trace->text,"for ... ", 8);
    }

    switch(lexpr->exp_type) {
      case S_EXPR_FUNCALL:
        EVAL_do_function( out , lexpr->val.fcall, 1, &newthread );
        break;
      default: {
        BINDING_DATA *top;
	size_t frame_start;
	AST_XFUNC_DECL *xfunc;

 	xfunc = get_each_in_array_xfunc(); 

	frame_start = EVAL_THREAD_prepare_xcall( cthread, xfunc );

	cthread->binding_data_stack.elmcount --;

	EVAL_do_expr( out, lexpr, PARAM_BY_REF, 0 );

	top = EVAL_THREAD_get_stack_top( cthread );
        top = BINDING_DATA_follow_ref( top );

	if (top->b.value_type != S_VAR_LIST) {
          EVAL_error( out , &lexpr->base,  "For statement needs function call or array, instead got %s",  get_type_name( top->b.value_type ) );
	}

        newthread = EVAL_THREAD_prepare_coroutine( cthread, frame_start, &xfunc->base );
        
	EVAL_thread_create( &xfunc->base, frame_start, newthread, out);
	}
	break;
    }

    yield_value = newthread->yield_rvalue;
    
    while( do_loop && newthread->eval_impl && CTHREAD_state( newthread->eval_impl ) != CTHREAD_STATE_EXIT)  {

	 if (!yield_value) {
	   yield_value = get_CONST_NULL();
	 }

         if (IS_STACK_VALUE(yield_value)) {
	   BINDING_DATA *tmp;
	   
	   tmp = BINDING_DATA_MEM_new( S_VAR_NULL );
	   EVAL_THREAD_set_current_thread( newthread );
	   BINDING_DATA_copy( tmp, yield_value, CP_REF );
	   
	   if (tmp->b.value_flags_ref & S_VAR_REF_HEAP2STACK) {
	     tmp->b.value.heap2stack_ref.thread = newthread;
	   }
	   yield_value = tmp;
	 }
	 EVAL_THREAD_set_current_thread( cur_thread );

         if (EVAL_trace_on(out)) {
           trace = EVAL_CTX_new_trace( out, &scl->base);
           DBUF_add( &trace->text,"for ", 4);
         }

	 data = BINDING_DATA_follow_ref( yield_value );     
	 if (exprvar->exp_type != S_EXPR_LIST_VALUES) { // not multivalue left hand side
	   assert(exprvar->exp_type ==  S_EXPR_REFERENCE );
           if (can_assign( AST_EXPRESSION_type( exprvar ), data->b.value_type, &offending_type)) {
	     EVAL_error( out , &lexpr->base, "Can't assign the %s value to %s", get_type_name( yield_value->b.value_type ), exprvar->val.ref.lhs);
           }
	   if (EVAL_trace_on(out)) {
             DBUF_add( &trace->text,exprvar->val.ref.lhs, strlen( exprvar->val.ref.lhs ) ); 
	     DBUF_add( &trace->text, " = ", 3 );
	     BINDING_DATA_prints( &trace->text, data, 0 ); 
	     DBUF_add_null( &trace->text );
	   }
	   EVAL_reference( out, exprvar, yield_value, COPY_SINGLE_ASSIGN_BY_VAL, VALFUNC_SHOW_METHOD_SIG );

           //if (can_assign( AST_EXPRESSION_type( lexpr ), data->b.value_type, &offending_type)) {
	   //  EVAL_error( out , &lexpr->base, "Can't assign the %s value to %s", get_type_name( data->b.value_type ), exprvar->val.ref.lhs);
           //}
	   //EVAL_reference( out, exprvar, 1, data, CP_REF );
	   //if (EVAL_trace_on(out)) {
	   //  DBUF_add( &trace->text, " = ", 3 );
	   //  BINDING_DATA_prints( &trace->text, data, 0 ); 
	   //}


	} else {
  
	   //data = BINDING_DATA_follow_ref( yield_value );     

	   if (data->b.value_type != S_VAR_LIST) {
		 EVAL_error( out , &exprvar->base,  "expects list to be returned, instead got %s", get_type_name( data->b.value_type ) );
	   }
	  
	   values = (AST_VECTOR *) exprvar->val.index_expr;
	   for( i = 0; i < AST_VECTOR_size( values ); i++) {
		 lhs = (AST_EXPRESSION *) AST_VECTOR_get( values, i );
		 binding_rhs = VALARRAY_get( &data->b.value.array_value, i );
       
	 	 if (EVAL_trace_on(out) && i > 0) {
		     DBUF_add(  &trace->text, " , ", 3 ); 
		 }

		 if ( lhs->exp_type  != S_EXPR_PLACEHOLDER) {
		   assert( lhs->exp_type == S_EXPR_REFERENCE );

		   // assign value to lhs.
		   EVAL_reference( out, lhs, binding_rhs ,  COPY_MULTI_ASSIGN_BY_VAL,  VALFUNC_SHOW_METHOD_SIG  );

		   if (EVAL_trace_on(out)) {
		     DBUF_add(  &trace->text, ":", 1 );
		     BINDING_DATA_prints( &trace->text, binding_rhs, 0 ); 
		   }

		 } else {
		   if (EVAL_trace_on(out)) {
		     DBUF_add(  &trace->text, " _ ", 1 ); 
		   }
		} 
	   }
           if (EVAL_trace_on(out)) {
              DBUF_add(  &trace->text, "] ", 2 ); 
           }
       }

       if (EVAL_trace_on(out)) {
         EVAL_CTX_free_trace( out, SHOW ); 	   
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
 	  out->loop_exit = LOOP_NO_BREAK;
	  break;
	 case EXIT_FUNCTION:
	  do_loop = 0;
	  return;
	 default:
	  ;
        }
       }


       if (EVAL_trace_on(out)) {
	  TRACE_println( &out->trace_out, scl->base.location.first_line ,  "end", 3 );
       	  TRACE_println( &out->trace_out, scl->base.location.first_line ,  "for ...", 7 );
       }
       
       EVAL_thread_resume_do( newthread, 0, &yield_value );

    }
    EVAL_THREAD_set_current_thread( cur_thread );

    if (EVAL_trace_on(out)) {
      TRACE_println( &out->trace_out, scl->base.location.first_line ,  "end # finish for loop", 21 );
    }

    if (newthread->eval_impl ) 
      CTHREAD_kill( newthread->eval_impl );
  
    EVAL_THREAD_free( newthread ); 

     
#if 0    
        break;
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

    show = EVAL_do_function( out, scl, 0, 0 );

    //EVAL_THREAD_pop_frame( cthread );

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
      EVAL_do_expr( out,  scl->rvalue, 0, 0  );
      
      // move the return value (top) to slot for function return value.
      rval = EVAL_THREAD_get_stack_top( cthread ); 
      if (rval->b.value_flags_ref & S_VAR_REF_STACK2STACK ) {
	rval = BINDING_DATA_follow_ref( rval );
      }

      top = EVAL_THREAD_stack_frame_offset( cthread, 0); 
      BINDING_DATA_move( top,  rval ); 
      	

      cthread->binding_data_stack.elmcount --; // discard top of stack without destructor (have moved it to ret value).

      // return to parent frame.
      //EVAL_THREAD_pop_frame( cthread );

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

  case S_NULL:
    break;

  default:
    assert(0);
  }
}

int EVAL_run(  EVAL_CTX *out, AST_BASE *base, char **argv, int argc )
{
  EVAL_CONTEXT *ctx;
  AST_FUNC_DECL *fdecl = (AST_FUNC_DECL *) base;

  ctx = &out->context;

  if (setjmp(  ctx->jmpbuf) ) { 
     return -1;
  } else { 
     ctx->is_jmpbuf_set = 1; 
  } 
  EVAL_CONTEXT_start( ctx, argv, argc );
 
  ctx->main.instr = (AST_BASE *) fdecl->func_body;
   
  EVAL_do( out );

  return 0;
}

// main procedure of a thread.
void EVAL_thread( VALUES *arg )
{
  EVAL_CTX *out = 0;
  EVAL_THREAD *thread = 0;
  AST_BASE *fdcl = 0;
  AST_FUNC_DECL *fdecl;
  EVAL_CONTEXT *rctx;

  VALUES_scan(arg, "%p%p%p", &out, &thread, &fdcl );

  rctx = &out->context;
  
  EVAL_THREAD_set_current_thread( thread );

  if (fdcl->type ==  S_FUN_DECL) {
    fdecl = (AST_FUNC_DECL *) fdcl;
    
     // do the function body.
    thread->instr = (AST_BASE *) fdecl->func_body;
    EVAL_do( out );

  } else {
    XCALL_DATA xdata;
    AST_XFUNC_DECL *xfunc = (AST_XFUNC_DECL *) fdcl;

    assert( fdcl->type == S_XFUN_DECL );
      
    //EVAL_THREAD_call_xfunc( thread, 0 , (AST_XFUNC_DECL *) fdcl );
    
    xdata.thread = thread;
    xdata.frame_offset = 0;
    xdata.num_arguments = xfunc->nparams;

    xfunc->xcall( &xdata );

    EVAL_THREAD_pop_frame ( thread );
  }
}
 


// PRECONDITION: a new thread has been created, arguments for top level call pushed onto stack.
// ACTION: thread created and is running
// POSTCONDITION: thread has yielded or exited.
int EVAL_thread_create(AST_BASE *fdecl, size_t frame_start, EVAL_THREAD *arg_thread, void *eval_ctx)
{
  EVAL_CTX *ctx = (EVAL_CTX *) eval_ctx;
  EVAL_CONTEXT *rtctx = &ctx->context; 
  EVAL_THREAD *cur_thread = rtctx->current_thread;
  CTHREAD *cthread;
  BINDING_DATA *data;
  size_t i;

  cthread = CTHREAD_init( &ctx->stacks, EVAL_thread );
 
  CTHREAD_start( cthread, 0, "%p%p%p", ctx, arg_thread, fdecl );

  EVAL_THREAD_set_current_thread( cur_thread );

  // pop arguments of calling thread.
  for( i = frame_start + 2 ; i < ARRAY_size(& cur_thread->binding_data_stack ); i++ ) {
     data = ((BINDING_DATA *) cur_thread->binding_data_stack.buffer) + i; 
     BINDING_DATA_free( data );
  }
  cur_thread->binding_data_stack.elmcount = frame_start;

  if (CTHREAD_state( cthread ) == CTHREAD_STATE_EXIT) {
     CTHREAD_free( cthread );
     arg_thread->eval_impl = 0;
     //EVAL_THREAD_free( arg_thread );
     return -1;
  }
  arg_thread->eval_impl = cthread;
  return  0;
}

int EVAL_thread_yield( BINDING_DATA *yield_value, BINDING_DATA **resume_msg, void *eval_ctx )
{
  EVAL_CTX *ctx = (EVAL_CTX *) eval_ctx;
  EVAL_CONTEXT *rtctx = &ctx->context; 
  int rt;

  rtctx->current_thread->yield_rvalue = yield_value;
 
  rt = CTHREAD_yield( 0, 0 );

  if (resume_msg) {
    *resume_msg = rtctx->current_thread->resume_value;
  }
  rtctx->current_thread->resume_value = 0;; 
 
  return rt;
}

int EVAL_thread_resume_do( EVAL_THREAD *thread, BINDING_DATA *resume_msg, BINDING_DATA **yield_value )
{
  CTHREAD *cthread;

  cthread = (CTHREAD *) thread->eval_impl;  

  thread->resume_value = resume_msg;

  CTHREAD_resume( cthread, 0, 0 );  

  *yield_value = thread->yield_rvalue;
  thread->yield_rvalue = 0;

  if (CTHREAD_state( cthread ) == CTHREAD_STATE_EXIT) {
     CTHREAD_free( cthread );
     thread->eval_impl = 0;
     //EVAL_THREAD_free( thread );
     //func->thread = 0;
     return 0;
  }
  return 1;
}

int EVAL_thread_resume( BINDING_DATA *function, BINDING_DATA *resume_msg, BINDING_DATA **yield_value, void *eval_ctx )
{
  VALFUNCTION *func = BINDING_DATA_get_fun( function );
  EVAL_THREAD *thread = func->thread;

  (void) eval_ctx;

  if (! EVAL_thread_resume_do( thread, resume_msg, yield_value ) ) {
    func->thread = 0;
    return 0;
  }
  return 1;
}


int EVAL_thread_exit( void *eval_ctx )
{
  (void) eval_ctx;

  CTHREAD_exit();
  return -1;
}


int EVAL_thread_kill( BINDING_DATA *function, void *eval_ctx )
{
  //EVAL_CTX *ctx = (EVAL_CTX *) eval_ctx;
  VALFUNCTION *func = BINDING_DATA_get_fun( function );
  
  (void) eval_ctx;


  EVAL_THREAD *thread = func->thread;
  CTHREAD *cthread;
  
  if (!thread) {
    return 0;
  }

  cthread = (CTHREAD *) thread->eval_impl;  
 
  if (cthread) 
    CTHREAD_kill( cthread );
  
  EVAL_THREAD_free( thread ); 


  return 0;
}

int EVAL_free( EVAL_CTX *out )
{
  EVAL_CONTEXT *ctx;

  ctx = &out->context;

  EVAL_CONTEXT_free( ctx );

  return 0;
}

int eval( PARSECONTEXT *ctx, EVAL_OPTIONS *opts  )
{
  EVAL_CTX ectx;

  if ( !EVAL_init( &ectx, ctx )) {

    ectx.context.show_source_line =  SHOW_SOURCE_LINE_impl;
    ectx.context.show_source_line_ctx = ctx;

    ectx.context.thread_yield_cb  = EVAL_thread_yield; 
    ectx.context.thread_resume_cb = EVAL_thread_resume; 
    ectx.context.thread_exit_cb   = EVAL_thread_exit;
    ectx.context.thread_kill_cb   = EVAL_thread_kill;
    ectx.context.invoke_function_cb  =  EVAL_proceed_fun_call;
    ectx.context.thread_ctx       = &ectx;

    ectx.context.trace_on =  opts->is_trace_on;

    if (EVAL_run( &ectx, ctx->my_ast_root, opts->argv, opts->argc )) {
      return -1;
    } 
  } else {
    fprintf( stderr, "Can't initialize interpreter\n");
    return -1;
  }
  EVAL_free( &ectx );
  return 0;
}


