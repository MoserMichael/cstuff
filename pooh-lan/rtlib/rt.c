#include "rtlib.h"
#include <math.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <wordexp.h>

//#include <values.h>

EVAL_THREAD * EVAL_THREAD_yield( BINDING_DATA *value_to_yield, BINDING_DATA **resume_msg, int *force_exit );
EVAL_THREAD * EVAL_THREAD_thread_yield_value( BINDING_DATA *function_obj, BINDING_DATA **yield_value, int *is_coroutine_exit  );
EVAL_THREAD * EVAL_THREAD_resume(BINDING_DATA *function_obj, BINDING_DATA *resume_msg, BINDING_DATA **yield_value, int *is_coroutine_exit );
void EVAL_THREAD_stopthread( BINDING_DATA *function );
void EVAL_THREAD_threadexit();


/* --------------- function for reflection --------------------------- */

#if 0
static void x_functionparameters( XCALL_DATA *xcall )
{
   (void) xcall;
}

static void x_functionreturntype( XCALL_DATA *xcall )
{
   (void) xcall;
}

static void x_applyfunc( XCALL_DATA *xcall )
{
  (void) xcall;
}
#endif

static void x_isarray( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_set_int( XCALL_rvalue(xcall), arg->b.value_type & S_VAR_LIST );
}

static void x_isfunction( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_set_int( XCALL_rvalue(xcall), arg->b.value_type & S_VAR_CODE );
}

static void x_istable( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_set_int( XCALL_rvalue(xcall), arg->b.value_type & S_VAR_HASH );
}

static void x_isstring( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_set_int( XCALL_rvalue(xcall), arg->b.value_type & S_VAR_STRING );
}

static void x_isnumber( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_set_int( XCALL_rvalue(xcall), IS_NUMBER( arg ) );
}


/* --------------- function for more than one type ------------------- */

static void x_collectgarbage( XCALL_DATA *xcall )
{
  EVAL_CONTEXT *ctx = xcall->thread->context;
  EVAL_CONTEXT_gc( ctx );
}

static void x_exit( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALSTRING *sdata;
  POOH_INT status = 0;

  arg = XCALL_param(xcall, 0 ); 
  if (! IS_NULL(arg)) {
     sdata = BINDING_DATA_get_string( arg );
     fprintf( stderr, "%.*s", (int) sdata->length, sdata->string );
  }

  arg = XCALL_param(xcall, 1 ); 
  BINDING_DATA_get_int( arg, &status );
 
  exit( status );
}


static void x_size( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  size_t rt;
  
  arg = XCALL_param(xcall, 0 ); 

  arg = BINDING_DATA_follow_ref( arg );  
  
  if (arg->b.value_type & S_VAR_STRING) {
    rt = VALSTRING_size( &arg->b.value.string_value );
  } else 
  if (arg->b.value_type & S_VAR_HASH) {
    rt = VALHASH_size( &arg->b.value.hash_value );
  } else
  if (arg->b.value_type & S_VAR_LIST) {
    rt = VALARRAY_size( &arg->b.value.array_value);
  }

  BINDING_DATA_set_int( XCALL_rvalue(xcall), rt );
} 

static void x_defined( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  
  arg = XCALL_param(xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );  
  BINDING_DATA_set_int( XCALL_rvalue(xcall), ! IS_NULL( arg ) );
}

static void x_dump( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  
  arg = XCALL_param(xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );  
  BINDING_DATA_print( stderr, arg , 0 );
}

static void x_trace( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  POOH_INT trace;
  
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_get_int( arg, &trace );
  xcall->thread->context->trace_on = trace;
}

static void x_showstack( XCALL_DATA *xcall )
{
  EVAL_THREAD_print_stack_trace( stderr, xcall->thread );
}
 
static void x_osname( XCALL_DATA *xcall )
{
  const char *osname = 0;

#if defined(__CYGWIN__) || defined(_WIN32)
  osname = "Windows";
#endif

#ifdef __linux__
  osname = "Linux";
#endif


  BINDING_DATA_set_string( XCALL_rvalue(xcall), osname, strlen( osname ) );
}
 
static void x_typeof( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  const char *tname;
  
  arg = XCALL_param(xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );  
  
  switch( arg->b.value_type ) {
    case S_VAR_INT:
    case S_VAR_DOUBLE:
	tname = "Number";
	break;
    case S_VAR_STRING:
	tname = "String";
	break;
    case S_VAR_CODE:
	tname = "Function";
	break;
    case S_VAR_HASH:
	tname = "Table";
	break;
    case S_VAR_LIST:
	tname = "Array";
	break;
    case S_VAR_NULL:
	tname = "Null";
	break;
    default:
	assert(0);
	break;
  }
  BINDING_DATA_set_string( XCALL_rvalue(xcall), tname, strlen( tname ) );
}	


/* --------- thread functions ----------------------------------- */
static void x_make_thread( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *ret;

  arg = XCALL_param(xcall, 0 );
  arg = BINDING_DATA_follow_ref( arg );  

  assert( ( arg->b.value_type & S_VAR_CODE ) != 0 );
 
  // function value marked as thread 
  ret = BINDING_DATA_MEM_new( S_VAR_NULL );

  BINDING_DATA_copy( ret, arg, CP_VALUE);
  ret->b.value_type |= S_VAR_CODE_THREAD;  
 
  BINDING_DATA_copy( XCALL_rvalue( xcall ), ret, CP_REF );
}

static void x_isthreadmain( XCALL_DATA *xcall )
{
  int iscoro = EVAL_THREAD_is_threadmain( xcall->thread );
  BINDING_DATA_set_int( XCALL_rvalue(xcall), iscoro );
}

static void x_threadexit( XCALL_DATA *xcall )
{
  EVAL_THREAD_threadexit( xcall->thread );
}

static void  x_stopthread( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  
  arg = XCALL_param(xcall, 0 ); 

  EVAL_THREAD_stopthread( arg );
}

void makethreadrvalue(XCALL_DATA *xcall, int has_exit, BINDING_DATA *msg_val )
{
  BINDING_DATA *rval, rstatus;
  VALARRAY *arval;

  BINDING_DATA_init( &rstatus, S_VAR_INT );
  BINDING_DATA_set_int( &rstatus, has_exit ); 

  rval = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  arval = &rval->b.value.array_value;
  VALARRAY_set_capacity( arval, 2 );
  VALARRAY_set( arval, 0, &rstatus, CP_VALUE );
 
  if (has_exit) {
    if (!msg_val) {
      msg_val = get_CONST_NULL();
    }
    VALARRAY_set( arval, 1, msg_val , CP_REF );
  } else {
    assert( msg_val == 0 );
    VALARRAY_set( arval, 1, get_CONST_NULL(), CP_VALUE );
  }
 
#if 0  
  ref = VALARRAY_get( arval, 1);
  if (ref->b.value_flags_ref & S_VAR_REF_HEAP2STACK) {
    ref->b.value.heap2stack_ref.thread = thread;
  }
#endif  

  BINDING_DATA_copy( XCALL_rvalue( xcall ), rval, CP_REF );
}

static void x_threadyield( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  BINDING_DATA *msg_val;
  int force_exit;
  EVAL_THREAD *thr;

  arg = XCALL_param(xcall, 0 ); 

  thr = EVAL_THREAD_yield( arg, &msg_val, &force_exit );

  makethreadrvalue( xcall, force_exit, msg_val );
  EVAL_THREAD_set_current_thread( thr );
 
#if 0
  BINDING_DATA_init( &rstatus, S_VAR_INT );
  BINDING_DATA_set_int( &rstatus, force_exit ); 

  rval = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  arval = &rval->b.value.array_value;
  VALARRAY_set_capacity( arval, 2 );
  VALARRAY_set( arval, 0, &rstatus, CP_VALUE );
 
  if (!force_exit) {
    VALARRAY_set( arval, 1, msg_val , CP_REF );
  } else {
    assert( msg_val == 0 );
    VALARRAY_set( arval, 1, get_CONST_NULL(), CP_VALUE );
  }
  
  BINDING_DATA_copy( XCALL_rvalue( xcall ), rval, CP_REF );
#endif
}

void dothreadyield0_nomsg( BINDING_DATA *arg )
{
  int force_exit;
  EVAL_THREAD *thr;

  thr = EVAL_THREAD_yield( arg, 0, &force_exit );

  EVAL_THREAD_set_current_thread( thr );
  if (force_exit) {
    EVAL_THREAD_threadexit( );
  }
}


BINDING_DATA * dothreadyield0( BINDING_DATA *arg )
{
  BINDING_DATA *msg_val, *tmp;
  int force_exit;
  EVAL_THREAD *thr;

  thr = EVAL_THREAD_yield( arg, &msg_val, &force_exit );

  if (force_exit) {
     EVAL_THREAD_set_current_thread( thr );
     EVAL_THREAD_threadexit( );
  }

  if (!msg_val) {
    msg_val = get_CONST_NULL();
  } else {
    if (IS_STACK_VALUE(msg_val)) {
      tmp = BINDING_DATA_MEM_new( S_VAR_NULL );
      BINDING_DATA_copy( tmp, msg_val, CP_REF );
      
      if (tmp->b.value_flags_ref & S_VAR_REF_HEAP2STACK) {
	tmp->b.value.heap2stack_ref.thread = thr;
      }
      
      msg_val = tmp;
    }
  }

  EVAL_THREAD_set_current_thread( thr );
  return msg_val;
}

BINDING_DATA * dothreadyield( BINDING_DATA *arg, int *force_exit )
{
  BINDING_DATA *msg_val, *tmp;
  EVAL_THREAD *thr;

  thr = EVAL_THREAD_yield( arg, &msg_val, force_exit );

  if (!msg_val) {
    msg_val = get_CONST_NULL();
  } else {
    if (IS_STACK_VALUE(msg_val)) {
      tmp = BINDING_DATA_MEM_new( S_VAR_NULL );
      BINDING_DATA_copy( tmp, msg_val, CP_REF );
      
      if (tmp->b.value_flags_ref & S_VAR_REF_HEAP2STACK) {
	tmp->b.value.heap2stack_ref.thread = thr;
      }
      
      msg_val = tmp;
    }
  }

  EVAL_THREAD_set_current_thread( thr );
  return msg_val;
}
static void x_threadyield0( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *msg_val;
  arg = XCALL_param(xcall, 0 ); 

  msg_val = dothreadyield0( arg );  
  
  BINDING_DATA_copy( XCALL_rvalue( xcall ), msg_val, CP_REF );
}

static void x_resume( XCALL_DATA *xcall )
{
  BINDING_DATA *func,*arg;
  BINDING_DATA *msg_val;
  int has_exit;
  EVAL_THREAD *thr;

  func = XCALL_param(xcall, 0 ); 
  arg = XCALL_param(xcall, 1 ); 

  thr = EVAL_THREAD_resume( func, arg, &msg_val, &has_exit );
  
  makethreadrvalue( xcall, has_exit, msg_val );
  EVAL_THREAD_set_current_thread( thr );
 
}


static void x_threadyieldvalue( XCALL_DATA *xcall )
{
  BINDING_DATA *func, *msg_val;
  int has_exit;
  EVAL_THREAD *thr,*othread;

  func = XCALL_param(xcall, 0 ); 

  thr = EVAL_THREAD_thread_yield_value( func, &msg_val, &has_exit  );

  othread = EVAL_THREAD_get_current_thread();
  EVAL_THREAD_set_current_thread( thr );
  makethreadrvalue( xcall, has_exit, msg_val );
  EVAL_THREAD_set_current_thread( othread );
 
}



/* -------------- function for arrays --------------------------- */

static void x_range( XCALL_DATA *xcall )
{
  BINDING_DATA *tmpn,tmpv,*arg,*rval;
  VALARRAY *arr;
  POOH_INT idx, from, to, step;
  int top = EVAL_THREAD_is_threadmain( xcall->thread );
 
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_get_int( arg,  &from );

  arg = XCALL_param(xcall, 1 ); 
  BINDING_DATA_get_int( arg,  &to );

  step = 1;
  arg = XCALL_param(xcall, 2 ); 
  if (! IS_NULL(arg)) {
    BINDING_DATA_get_int( arg, &step );
  }


  if (top) {
    for(idx = 0 ; idx <= (to - from) ; idx += step ) {
       tmpn = BINDING_DATA_MEM_new( S_VAR_INT );
       tmpn->b.value.long_value = from + idx;
       dothreadyield0_nomsg( tmpn ); 
    }
  } else {
    rval = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( rval, S_VAR_LIST );
    arr = &rval->b.value.array_value;

    for(idx = 0 ; idx <= (to - from) ; idx += step ) {
       BINDING_DATA_init( &tmpv, S_VAR_INT );
       tmpv.b.value.long_value = from + idx;
       VALARRAY_set( arr, idx , &tmpv, CP_VALUE );
    }
  }
}

static DATA_REF cb_func;

int compare_cb(const void *a, const void *b )
{
  BINDING_DATA *pa, *pb, *rval, *tmp;
  size_t frame_start;
  POOH_INT nval;
  EVAL_THREAD *cthread;
  VALFUNCTION *func;

  pa = * ((BINDING_DATA **) a);
  pb = * ((BINDING_DATA **) b);

  tmp = DATA_REF_get( &cb_func );
  func = BINDING_DATA_get_fun( tmp );
  
  cthread = EVAL_THREAD_get_current_thread();
/// invoke the callback functiond.
  frame_start = EVAL_THREAD_prepare_func_call( cthread, func );

  tmp = EVAL_THREAD_parameter_ref( cthread, frame_start, 0 );
  memcpy( tmp, pa, sizeof(BINDING_DATA) );
  tmp->b.value_flags_val = 0;

  tmp = EVAL_THREAD_parameter_ref( cthread, frame_start, 1 );
  memcpy( tmp, pb, sizeof(BINDING_DATA) );
  tmp->b.value_flags_val = 0;

  tmp = DATA_REF_get( &cb_func );
  func = BINDING_DATA_get_fun( tmp );
 
  rval = EVAL_THREAD_proceed_func_call( cthread, frame_start, func );

  BINDING_DATA_get_int( rval, &nval );
  
  EVAL_THREAD_discard_pop_stack( cthread ); 
  
  if (nval < 0)
    return -1;
  if (nval > 0)
    return 1;
  return  0 ;
}




static void x_sort( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALARRAY *arr;
  
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &cb_func, arg );
  
  qsort( arr->data, arr->size, sizeof(void *), compare_cb );
}

static void x_eachinarray( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp,*ret;
  VALARRAY *arr,*retarr;
  size_t i;
  int top = EVAL_THREAD_is_threadmain( xcall->thread );
 
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );

  if (top) {
    for( i = 0; i <  arr->size; i++ ) {
      tmp = VALARRAY_get( arr, i );
      dothreadyield0_nomsg( tmp ); 
    }  
  } else {

    ret = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( ret, S_VAR_LIST );
    retarr = &ret->b.value.array_value;
     
    VALARRAY_set_capacity( retarr, VALARRAY_size( arr ) );
 
    for( i = 0; i < arr->size; i++ ) {
      tmp = VALARRAY_get( arr, i );
      VALARRAY_set( retarr, i , tmp , CP_REF );
    }
  }
}

static void x_reverse( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp,*ret;
  VALARRAY *arr,*retarr;
  size_t i;
  int top = EVAL_THREAD_is_threadmain( xcall->thread );
 
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );

  if (top) {
    i = arr->size;
    if ( i > 0 ) {
      do {
	tmp = VALARRAY_get( arr, i - 1 );
        dothreadyield0_nomsg( tmp ); 
	i -= 1;
      } while( i != 0 );
    }  
  } else {

    ret = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( ret, S_VAR_LIST );
    retarr = &ret->b.value.array_value;
     
    VALARRAY_set_capacity( retarr, VALARRAY_size( arr ) );
 

    for( i = 0; i < arr->size; i++ ) {
      tmp = VALARRAY_get( arr, i );
      VALARRAY_set( retarr, arr->size - i - 1 , tmp , CP_REF );
    }
  }
}

static void x_append( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *elm, *tmp;
  VALARRAY *arr, *arrappend;
  size_t i,j,nsize;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );

  elm = XCALL_param( xcall, 1);
  arrappend = BINDING_DATA_get_arr( elm );

  VALARRAY_set_capacity( arr, VALARRAY_size( arr ) + VALARRAY_size( arrappend ) );  
   
  nsize = VALARRAY_size( arrappend ); 
  for( j = VALARRAY_size( arr ), i = 0; i < nsize;  i++ ) {
     tmp = VALARRAY_get( arrappend, i );
     VALARRAY_set( arr, j ++ , tmp, CP_VALUE );
  }
}

static void x_push( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *elm;
  VALARRAY *arr;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );

  elm = XCALL_param( xcall, 1);

  VALARRAY_set( arr, VALARRAY_size( arr ), elm, CP_REF );
}



static void x_pop( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *tmp;
  VALARRAY *arr;
  
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );


  if (arr->size == 0) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't 'pop' value off an empty stack. " );
  }

  tmp = VALARRAY_get( arr, arr->size - 1 );  
  arr->size -= 1;
  BINDING_DATA_copy( XCALL_rvalue( xcall ), tmp, CP_REF );
}

static void x_unshift( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALARRAY *arr;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );


  VALARRAY_set( arr, VALARRAY_size( arr ), XCALL_param(xcall, 1 ), CP_REF );
}

static void x_shift( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *tmp;
  VALARRAY *arr;
  
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );


  if (arr->size == 0) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't 'pop' value off an empty stack. " );
  }

  tmp = VALARRAY_get( arr, arr->size - 1 );  
  BINDING_DATA_copy( XCALL_rvalue( xcall ), tmp, CP_REF );
}

static void x_join( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *val;
  VALARRAY *arr;
  VALSTRING *sdelim,*rval,tval;
  size_t i;
  char stmp[ 120 ];

  arg = XCALL_param( xcall, 0 ); 
  sdelim = BINDING_DATA_get_string( arg );

  arg = XCALL_param( xcall, 1 ); 
  arr = BINDING_DATA_get_arr( arg );


  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( arg, S_VAR_STRING );
  rval = &arg->b.value.string_value;


  for( i = 0; i < VALARRAY_size( arr ); i++ ) {
    val = VALARRAY_get( arr, i ); 
 
    if (IS_REF( val )) {
      val = BINDING_DATA_follow_ref( val );
    }

    if ( i > 0) {
      VALSTRING_append( rval, sdelim );
    }

    if (val->b.value_type & S_VAR_STRING) {

      VALSTRING_append( rval, &val->b.value.string_value );

    } else {
       if (val->b.value_type & S_VAR_INT) {
#if POOH_INT_SIZE == 4       
         sprintf( stmp, "%ld", val->b.value.long_value );
#else
         sprintf( stmp, "%lld", val->b.value.long_value );
#endif	 
       } else if (val->b.value_type & S_VAR_DOUBLE) {
         sprintf( stmp, "%f", val->b.value.double_value );
       } else if ( val->b.value_type & S_VAR_HASH) {
         sprintf( stmp, "Hash(%p)", val );
       } else if (val->b.value_type & S_VAR_LIST) {
         sprintf( stmp, "List(%p)", val );
       } else if (val->b.value_type & S_VAR_CODE) {
         sprintf( stmp, "Function(%p)", val );
       } else {
         assert( 0 );
       }

       tval.string = stmp;
       tval.capacity = tval.length = strlen(stmp);

       VALSTRING_append( rval, &tval );
    }
  }
}


/* --------------- higher order functions  -------------------------- */

static void x_mapcopy( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp,*ret, *rval, **arrdata;
  VALARRAY *arr,*aret;
  DATA_REF rfunc,rret;
  VALFUNCTION *func;
  size_t i, frame_start, arrsize;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &rfunc, arg );
  //func = BINDING_DATA_get_fun( arg );
   
  ret = XCALL_rvalue( xcall ); 
  aret = &ret->b.value.array_value;
  BINDING_DATA_init( ret, S_VAR_LIST );
  DATA_REF_set( &rret, ret );
 
  arrdata = (BINDING_DATA **) arr->data;
  arrsize = arr->size;
  for(  i = 0; i < arrsize; i++ ) {
 
    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    
    frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, func );
    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );

    BINDING_DATA_copy( tmp, arrdata[ i ] , CP_REF );

    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    rval = EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, func );

    ret = DATA_REF_get( &rret );
    aret = &ret->b.value.array_value;
    VALARRAY_set( aret, i , rval, CP_REF );
    
    EVAL_THREAD_discard_pop_stack( xcall->thread ); 
  }
}

static void x_filtercopy( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp,*ret, *rval, **arrdata;
  VALARRAY *arr,*aret;
  DATA_REF rfunc,rret;
  VALFUNCTION *func;
  size_t i, j, frame_start, arrsize;
  POOH_INT nval;
  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &rfunc, arg );
   
  ret = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( ret, S_VAR_LIST );
  DATA_REF_set( &rret, ret );

  arrdata = (BINDING_DATA **) arr->data;
  arrsize = arr->size;
  for( j = i = 0; i < arrsize; i++ ) {
 
    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
  
    frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, func );
    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );

    BINDING_DATA_copy( tmp, arrdata[ i ], CP_REF );

    rval = EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, func );

    BINDING_DATA_get_int( rval, &nval );
 
    if (nval) {
	ret = DATA_REF_get( &rret );
	aret = &ret->b.value.array_value;
 	
	VALARRAY_set( aret, j++, arrdata[ i ], CP_REF );
    }
    
    EVAL_THREAD_discard_pop_stack( xcall->thread ); 
  }
}

static void x_mapreplace( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp, *rval, **arrdata;
  VALARRAY *arr;
  DATA_REF rfunc;
  VALFUNCTION *func;
  size_t i, frame_start, arrsize;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &rfunc, arg );
  //func = BINDING_DATA_get_fun( arg );
   
 
  arrdata = (BINDING_DATA **) arr->data;
  arrsize = arr->size;
  for(  i = 0; i < arrsize; i++ ) {
 
    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    
    frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, func );
    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );

    BINDING_DATA_copy( tmp, arrdata[ i ] , CP_REF );

    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    rval = EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, func );

    BINDING_DATA_copy( arrdata[ i ], rval , CP_REF );


#if 0
    ret = DATA_REF_get( &rret );
    aret = &ret->b.value.array_value;
    VALARRAY_set( aret, i , rval, CP_REF );
#endif

    EVAL_THREAD_discard_pop_stack( xcall->thread ); 
  }
}


static void x_foldfirst2last( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp, *rval, *initval, *tmpres,**arrdata;
  VALARRAY *arr;
  DATA_REF rfunc,rinitval,rtmpres;
  VALFUNCTION *func;
  size_t i, frame_start, arrsize;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &rfunc, arg );
  
  initval = XCALL_param(xcall, 2 ); 
  DATA_REF_set( &rinitval, initval );

  tmpres = EVAL_THREAD_push_stack( xcall->thread, S_VAR_NULL );
  DATA_REF_set( &rtmpres, tmpres );
  
  arrdata = (BINDING_DATA **) arr->data;
  arrsize = arr->size;
  for( i = 0; i < arrsize; i ++ ) {

    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
 
    frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, func );
    
    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 1 );
    BINDING_DATA_copy( tmp,  i == 0 ? DATA_REF_get( &rinitval ) : DATA_REF_get( &rtmpres ) , CP_REF );

    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );
    BINDING_DATA_copy( tmp, arrdata[ i ], CP_REF );
 
    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    rval = EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, func );

    BINDING_DATA_move( DATA_REF_get( &rtmpres ), rval );
  }

  EVAL_THREAD_discard_pop_stack( xcall->thread ); 
 
  BINDING_DATA_move(XCALL_rvalue( xcall ) , DATA_REF_get( &rtmpres ) );

}
 
static void x_foldlast2first( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*tmp, *rval, *initval, *tmpres,**arrdata;
  VALARRAY *arr;
  DATA_REF rfunc,rinitval,rtmpres;
  VALFUNCTION *func;
  size_t i, frame_start;

  arg = XCALL_param(xcall, 0 ); 
  arr = BINDING_DATA_get_arr( arg );
  
  arg = XCALL_param(xcall, 1 ); 
  DATA_REF_set( &rfunc, arg );
  
  initval = XCALL_param(xcall, 2 ); 
  DATA_REF_set( &rinitval, initval );

  tmpres = EVAL_THREAD_push_stack( xcall->thread, S_VAR_NULL );
  DATA_REF_set( &rtmpres, tmpres );
  
  arrdata = (BINDING_DATA **) arr->data;
  for( i = arr->size; i > 0; i -- ) {

    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
 
    frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, func );
    
    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 1 );
    BINDING_DATA_copy( tmp,  i == arr->size ? DATA_REF_get( &rinitval ) : DATA_REF_get( &rtmpres ) , CP_REF );

    tmp = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );
    BINDING_DATA_copy( tmp, arrdata[ i - 1 ], CP_REF );
 
    arg = DATA_REF_get( &rfunc );
    func = BINDING_DATA_get_fun( arg );
    
    rval = EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, func );

    BINDING_DATA_move( DATA_REF_get( &rtmpres ), rval );
  }

  EVAL_THREAD_discard_pop_stack( xcall->thread ); 
 
  BINDING_DATA_move(XCALL_rvalue( xcall ) , DATA_REF_get( &rtmpres ) );

}
 
/* --------------- function for hashes ------------------------------ */
static void x_exists ( XCALL_DATA *xcall )
{
  VALHASH *hash;
  BINDING_DATA *arg, *key, *rval;
  int rt;

  arg = XCALL_param( xcall, 0 ); 
  hash = BINDING_DATA_get_hash( arg );

  key = XCALL_param( xcall, 0 ); 
  key = BINDING_DATA_follow_ref( key );
  
  rt = VALHASH_find( hash, key, &rval )  == 0;

  BINDING_DATA_set_int( XCALL_rvalue(xcall), rt );
}

typedef enum {
  HGET_KEYS,
  HGET_VALUES,
  HGET_EACH,
} HGET_ACTION;


static void keyvals( XCALL_DATA *xcall, HGET_ACTION act)
{
  BINDING_DATA *arg,*elm;
  VALHASH *hash;
  VALARRAY *rarr, *aelm;
  VALHASHPOS pos;
  BINDING_DATA *key,*value, *rval, *cpykey;
  size_t idx = 0;
  int iscoro = EVAL_THREAD_is_threadmain( xcall->thread );

  arg = XCALL_param( xcall, 0 ); 
  hash = BINDING_DATA_get_hash( arg );

  if (iscoro) {
    VALHASHPOS_init( &pos );
    while( VALHASH_iterate( hash, &key, &value, &pos ) ) {
      switch( act ) {
         case HGET_KEYS:

	 cpykey = BINDING_DATA_MEM_new( S_VAR_NULL );
	 BINDING_DATA_copy( cpykey, key, CP_VALUE );
	 dothreadyield0_nomsg( cpykey ); 
         break;
       case HGET_VALUES:
       	 dothreadyield0_nomsg( value ); 
         break;
       case HGET_EACH:
         elm = BINDING_DATA_MEM_new( S_VAR_LIST );
         aelm = &elm->b.value.array_value;
         VALARRAY_set_capacity( aelm, 2 );
         VALARRAY_set( aelm, 0, key , CP_VALUE );
         VALARRAY_set( aelm, 1, value , CP_REF );
       	 dothreadyield0_nomsg( elm ); 
         break;
      }
    }
  } else {
    rval = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( rval, S_VAR_LIST );
    rarr = &rval->b.value.array_value;

    VALHASHPOS_init( &pos );
    while( VALHASH_iterate( hash, &key, &value, &pos ) ) {
     switch( act ) {
       case HGET_KEYS:
         VALARRAY_set( rarr, idx++, key, CP_VALUE ); // hash keys in array may not me modified by reference, so copy them by value.
         break;
       case HGET_VALUES:
         VALARRAY_set( rarr, idx++, value, CP_REF );
         break;
       case HGET_EACH:
         elm = BINDING_DATA_MEM_new( S_VAR_LIST );
         aelm = &elm->b.value.array_value;
         VALARRAY_set_capacity( aelm, 2 );
         VALARRAY_set( aelm, 0, key , CP_VALUE );
         VALARRAY_set( aelm, 1, value , CP_REF );
         VALARRAY_set( rarr, idx++, elm, CP_VALUE );
         break;
      }
    }
  }
}





static void x_keys( XCALL_DATA *xcall )
{
  keyvals( xcall, HGET_KEYS );
}
   
static void x_values( XCALL_DATA *xcall )
{
  keyvals( xcall, HGET_VALUES );
}

static void x_each( XCALL_DATA *xcall )
{
  keyvals( xcall, HGET_EACH );
}

static void x_erase( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALHASH *hash;
    
  arg = XCALL_param(xcall, 0 ); 
  hash = &arg->b.value.hash_value;

  arg = XCALL_param(xcall, 1 ); 
  
  VALHASH_delete_key( hash, arg );

}
 
/* -------------------- function for strings ----------------------- */

static void ton( XCALL_DATA *xcall, int radix )
{
  BINDING_DATA *arg;
  VALSTRING *sval;
  POOH_INT ret;
  
  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );
  
  VALSTRING_make_null_terminated( sval );

  ret = strtol( sval->string,  0, radix );
  BINDING_DATA_set_int( XCALL_rvalue( xcall ) , ret ); 

}

static void x_toint( XCALL_DATA *xcall )
{
   ton( xcall, 10 );

}

static void x_tohex( XCALL_DATA *xcall )
{
  ton( xcall, 16 );
}

static void x_tooct( XCALL_DATA *xcall )
{
  ton( xcall, 8  );
}


static void x_find( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALSTRING *hay, *needle;
  int rt;

  arg = XCALL_param( xcall, 0 ); 
  hay = BINDING_DATA_get_string( arg );

  arg = XCALL_param( xcall, 1 ); 
  needle = BINDING_DATA_get_string( arg );

  rt = VALSTRING_find( hay, needle );
  if (rt < 0) {
    rt = 0;
  } else {
    rt += 1;
  }
  BINDING_DATA_set_int( XCALL_rvalue( xcall ), rt );
}

typedef enum {
  STR_LEFT,
  STR_RIGHT,
  STR_MID,
} STR_ACTION;

static void mid_imp( XCALL_DATA *xcall, STR_ACTION act)
{
  BINDING_DATA *arg;
  VALSTRING *sval,*rval;
  POOH_INT n1,n2;

  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );

  if (act != STR_MID) {
    arg = XCALL_param( xcall, 1 );
    BINDING_DATA_get_int( arg, &n1 );
  }

  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( arg, S_VAR_STRING );
  rval = &arg->b.value.string_value;

  switch(act) {
    case STR_LEFT:
      if (n1 == 0) {
	return; // the empty string is returned
      }
      if (n1 >= (POOH_INT) sval->length) {
	n1 = sval->length - 1;
      }
      VALSTRING_substr( rval, sval, 0, n1 );
      break;
    case STR_MID: {
      n1 = 1;
      arg = XCALL_param( xcall, 1 );
      if (!IS_NULL(arg)) {
        BINDING_DATA_get_int( arg, &n1 );
        if (n1 > (POOH_INT) sval->length) {
          return; // the empty string is returned.
        }
      } 
 
      n2 = sval->length; 
      arg = XCALL_param( xcall, 2 );
      if (!IS_NULL(arg)) {
        BINDING_DATA_get_int( arg, &n2 );
      } 
      
      if (n1 < 1) {
        n2 += (n1 - 1);
	n1 = 1;
      }
      if (n2 <= 0) {
	 return; // the empty string is returned.
      }
      
      --n1;
      VALSTRING_substr( rval, sval, n1, n2);
      }
      break;
    case STR_RIGHT:
      if (n1 == 0) {
	return; // the empty string is returned
      }
      if (n1 > (POOH_INT) sval->length) {
	n1 = sval->length;
      }
      VALSTRING_substr( rval, sval, sval->length - n1, n1 );
      break;
  }

}

static void x_mid( XCALL_DATA *xcall )
{
  mid_imp( xcall, STR_MID );
}

static void x_left( XCALL_DATA *xcall )
{
  mid_imp( xcall, STR_LEFT );
}

static void x_right( XCALL_DATA *xcall )
{
  mid_imp( xcall, STR_RIGHT );
}

static void x_split( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *ret, *tmps;
  VALSTRING *str,*delim;
  VALARRAY *aret;
  char *sstr, *sdelim, *tok;;
  int force_exit;
  int i = 0;
  int top = EVAL_THREAD_is_threadmain( xcall->thread );
  
  if (!top) {
    ret =  BINDING_DATA_MEM_new( S_VAR_LIST ); 
    aret = &ret->b.value.array_value;
    BINDING_DATA_copy( XCALL_rvalue( xcall ), ret, CP_REF );
  }
  
  arg = XCALL_param( xcall, 0 ); 
  str = BINDING_DATA_get_string( arg );
  VALSTRING_make_null_terminated( str );
  sstr = strdup( str->string );

  arg = XCALL_param( xcall, 1 ); 
  delim = BINDING_DATA_get_string( arg );
  if (delim) {
    VALSTRING_make_null_terminated( delim );
    sdelim = delim->string;
  } else {
    sdelim = " \t";
  }
 
  tok = strtok( sstr, sdelim );
  if (tok) {
    do {
      if (top) {
         tmps = BINDING_DATA_MEM_new( S_VAR_STRING );
         VALSTRING_set( &tmps->b.value.string_value, tok , strlen( tok ) );
         dothreadyield( tmps, &force_exit );
         if (force_exit) {
           break;
         }
      } else {
         tmps = VALARRAY_set_entry( aret, i ++  );
         BINDING_DATA_init( tmps, S_VAR_STRING );
         VALSTRING_set( &tmps->b.value.string_value, tok, strlen( tok ) );
     }
    } while( (tok = strtok( 0, sdelim ) ) != 0 );
  }
  free( sstr );
}
 

static void x_print_imp( BINDING_DATA *data )
{
  VALSTRING *sdata;
  VALARRAY *arr;
  size_t i;

  data = BINDING_DATA_follow_ref( data );

  switch( data->b.value_type ) {
     case S_VAR_INT:
#if POOH_INT_SIZE == 4       
       fprintf( stdout, "%ld", data->b.value.long_value );
#else       
       fprintf( stdout, "%lld", data->b.value.long_value );
#endif       
       break;
     case S_VAR_DOUBLE:
       fprintf( stdout, "%f", data->b.value.double_value );
       break;
     case S_VAR_STRING:
       sdata = &data->b.value.string_value;
       fprintf( stdout, "%.*s", (int) sdata->length, sdata->string );
       break;
     case S_VAR_LIST:
       arr = &data->b.value.array_value;
       for( i = 0; i < VALARRAY_size( arr ) ; i++ ) {
         if ( i > 0 ) {
	    fprintf( stdout, " " );
	 }
	 x_print_imp( VALARRAY_get( arr, i ) );
       }
       break;
  }
}

static void x_print( XCALL_DATA *xcall )
{
  x_print_imp( XCALL_param( xcall, 0 ) );
  
  if (xcall->thread->context->trace_on) {
    fflush( stdout );
    fprintf( stderr, "\n" ); // to allign the next trace line.
  }
}

static void x_println( XCALL_DATA *xcall )
{
  x_print_imp( XCALL_param( xcall, 0 ) );
  fprintf( stdout, "\n" );

  if (xcall->thread->context->trace_on) {
    fflush( stdout );
  }
}


static void x_emptystring( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  
  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( arg, S_VAR_STRING );
}

static void x_newline( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALSTRING *str;
  
  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( arg, S_VAR_STRING );
  str = &arg->b.value.string_value;
  VALSTRING_set( str, "\n", 1 );

}



/* -------------- numeric functions ---------------------- */
static void x_srand( XCALL_DATA *xcall )
{
   POOH_INT nmaxn;

   BINDING_DATA_get_int( XCALL_param( xcall, 0 ), &nmaxn );
   srand( (unsigned long) nmaxn );
}
 
static void x_rand( XCALL_DATA *xcall )
{
  BINDING_DATA *maxn;    
  POOH_INT nmaxn;
  double res;

  maxn = XCALL_param( xcall, 0 );
  if (maxn) {
     BINDING_DATA_get_int( maxn, &nmaxn );
     res = rand() / RAND_MAX;
     res *= nmaxn;
  } else {
     res = rand();
  }
 

  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}


static void x_sqrt( XCALL_DATA *xcall )
{ 
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  
  if (arg < 0) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't compute square root of negative number %f ", arg );
  }
  res = sqrt( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
 
}

static void x_sin( XCALL_DATA *xcall )
{ 
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  res = sin( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_cos( XCALL_DATA *xcall )
{ 
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  res = cos( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_asin( XCALL_DATA *xcall )
{ 
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  if (arg < 1 || arg > 1) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't compute arc sine of value greater than one or smaller than -1. value %f ", arg );
  }
  res = asin( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_acos( XCALL_DATA *xcall )
{ 
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  if (arg < 1 || arg > 1) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't compute arc cosine of value greater than one or smaller than -1. value %f ", arg );
  }
  res = acos( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_tan( XCALL_DATA *xcall )
{
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  res = tan( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_atan( XCALL_DATA *xcall )
{
  double arg, res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  res = atan( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}


static void x_pow( XCALL_DATA *xcall )
{
  double arg,arg2,res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  BINDING_DATA_get_double( XCALL_param( xcall, 1 ), &arg2 );
  res = pow( arg, arg2 );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_loge( XCALL_DATA *xcall )
{
  double arg,res;

  BINDING_DATA_get_double( XCALL_param( xcall, 0 ), &arg );
  if (arg < 1) {
    EVAL_CONTEXT_runtime_error( xcall->thread->context , "Can't compute logarithm of value smaller than 0. value %f ", arg );
  }
  res = log( arg );
  BINDING_DATA_set_double( XCALL_rvalue( xcall ), res );
}

static void x_abs( XCALL_DATA *xcall )
{ 
  BINDING_DATA *arg;
  double darg;
  POOH_INT larg;

  arg = XCALL_param( xcall, 0 );
  arg = BINDING_DATA_follow_ref( arg );
  
  if (arg->b.value_type & S_VAR_INT) {
    larg = arg->b.value.long_value;  
    if (larg < 0) {
      larg  = - larg;
    }
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), larg );
  } else {
    assert( arg->b.value_type & S_VAR_DOUBLE );
    darg = arg->b.value.double_value;
    if (darg < 0) {
      darg = - darg;
    }
    BINDING_DATA_set_double( XCALL_rvalue( xcall ), darg );
  }
}

static void x_maxfloat( XCALL_DATA *xcall )
{ 
   BINDING_DATA_set_double( XCALL_rvalue( xcall ), DBL_MAX );
}


static void x_minfloat( XCALL_DATA *xcall )
{ 
   BINDING_DATA_set_double( XCALL_rvalue( xcall ), DBL_MIN );
}

static void x_maxint( XCALL_DATA *xcall )
{ 
#if POOH_INT_SIZE == 4       
   BINDING_DATA_set_int( XCALL_rvalue( xcall ), INT_MAX );
#else
   BINDING_DATA_set_int( XCALL_rvalue( xcall ), LLONG_MAX );
#endif
}

static void x_minint( XCALL_DATA *xcall )
{ 
#if POOH_INT_SIZE == 4       
   BINDING_DATA_set_int( XCALL_rvalue( xcall ),  INT_MIN );
#else
   BINDING_DATA_set_int( XCALL_rvalue( xcall ),  LLONG_MIN );
#endif
}

/* -------------- processes and groups ---------------------- */


static char **prev_make_env;
extern char **environ;


static void make_env( XCALL_DATA *xcall )  
{
  EVAL_CONTEXT *ctx = xcall->thread->context;
  BINDING_DATA *env = ctx->env;
  VALHASH *hash;   size_t i, nsize;
  VALHASHPOS pos;
  BINDING_DATA *key, *value;
  DBUF dbuf;
  char tmp[ 120 ];

  fflush( stdout );

  hash = BINDING_DATA_get_hash( env );
  if (!hash) {
    return;
  }

  if (environ == prev_make_env) {
    for( i = 0; environ[ i ] != 0; ++ i ) {
      free( environ[ i ] );
    }
    free( environ );
  }
  nsize = VALHASH_size( hash );  
  environ = malloc( sizeof( char *) * (nsize + 1) );
  
  i = 0;

  DBUF_init( &dbuf, 0 );
  VALHASHPOS_init( &pos );
  while( VALHASH_iterate( hash, &key, &value, &pos ) ) {

    assert(key->b.value_type & S_VAR_STRING);
    VALSTRING *str = &key->b.value.string_value;
    DBUF_add( &dbuf, str->string, str->length );
    DBUF_add( &dbuf, "=", 1 );

    value = BINDING_DATA_follow_ref( value );
    if (value->b.value_type & S_VAR_STRING) {
       VALSTRING *str = &value->b.value.string_value;
       DBUF_add( &dbuf, str->string, str->length );
    } else if (value->b.value_type & S_VAR_INT) {
#if POOH_INT_SIZE == 4       
       sprintf( tmp, "%ld", value->b.value.long_value );  
#else       
       sprintf( tmp, "%lld", value->b.value.long_value );  
#endif       
       DBUF_add( &dbuf, tmp, strlen( tmp ) );
    } else if (value->b.value_type & S_VAR_DOUBLE) {
       sprintf( tmp, "%e", value->b.value.double_value );  
       DBUF_add( &dbuf, tmp, strlen( tmp ) );
    } else {
       strcpy( tmp, "" );
       DBUF_add( &dbuf, tmp, strlen( tmp ) );
    }
    DBUF_add_null( &dbuf );
    environ[ i ++ ] = strdup( (char *) dbuf.buf ); 

    DBUF_reset( &dbuf );
  }
  environ[ i ] = 0;  
}

//--------------------------------------------------------------
#define MODE_READ	    1
#define MODE_WRITE	    2
#define MODE_WRITE_APPEND   4
#define MODE_PIPE	    8

#define STDIN_FD	    0
#define STDOUT_FD	    1

typedef struct tagPROC_OPEN {
  int read_fd;
  int write_fd;
  pid_t pid;
  int status;
} PROC_OPEN;

static int proc_open(  PROC_OPEN *proc, const char *cmd, int mode )
{
   int read_pipe[2], write_pipe[2];
   pid_t rpid;
   int fd;

   read_pipe[0] = read_pipe[1] = write_pipe[0] = write_pipe[1] = -1;
   proc->read_fd = proc->write_fd = -1;
   proc->pid = -1;
   proc->status = -1;

   if (mode & MODE_READ) {
     if (pipe2(read_pipe,O_CLOEXEC)) {
       goto err;
     }
     proc->read_fd = read_pipe[0];
   }

   if (mode & MODE_WRITE) {
     if (pipe2(write_pipe,O_CLOEXEC)) {
       goto err;
     }
     proc->write_fd = write_pipe[1];
   }	

   rpid = fork();

   if (rpid == -1) {
     goto err;
   } 

   if ( rpid == 0 ) {
     if (mode & MODE_READ) {
	fd = read_pipe[1];
	if (fd != STDOUT_FD) {
	  dup2( fd, STDOUT_FD );
	} else {
	  fcntl( fd, F_SETFD, 0 );
	}
     }	
     if (mode & MODE_WRITE) {
        fd = write_pipe[0];
	if (fd != STDIN_FD) {
	  dup2( fd, STDIN_FD );
	} else {
	  fcntl( fd, F_SETFD, 0 );
	}
     }
     execl( "/bin/sh", "sh", "-c", cmd, (char *) 0 ); 
     exit( 127 );
   }

   proc->pid = rpid;
   return 0;

err:  
   if (read_pipe[0] != -1) 
     close(read_pipe[0]);
   if (read_pipe[1] != -1) 
     close(read_pipe[1]);
   if (write_pipe[0] != -1) 
     close(write_pipe[0]);
   if (write_pipe[1] != -1) 
     close(write_pipe[1]);
    return -1;
}

static int proc_wait( PROC_OPEN *proc )
{
   int status, ret;

   if (proc->read_fd != -1) {
       close( proc->read_fd );
       proc->read_fd = -1;
   }

   if (proc->write_fd != -1) {
       close( proc->write_fd );
       proc->write_fd = -1;
   }

   do {
       ret = waitpid( proc->pid, &status, 0 );
   } while( ret == -1 && errno == EINTR );

   if (ret == -1) {
     return -1;
   }
   proc->status = status;
   return 0;
}

static void x_run( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *rval, *runout, tmpv;
  VALSTRING *sval, *srunout;
  VALARRAY *arr;
  int status;
  FILE *fp;
  char buf[ 513 ];
  size_t nread;

  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );

  VALSTRING_make_null_terminated( sval );
  
  runout = BINDING_DATA_MEM_new( S_VAR_STRING );
  srunout = &runout->b.value.string_value;

  make_env( xcall );  

  fp = popen( sval->string, "r" );
  if (fp) {
    while( (nread = fread( buf, 1, sizeof(buf) - 1, fp ) ) != (size_t) -1 ) {
	if (nread > 0) {
	   buf[ nread ] = '\0';
	   VALSTRING_appends( srunout, buf );
	} 

        if (feof( fp )  || ferror( fp )) {
	   break;
	}

	if (nread == 0 ) {
	   usleep( 10000 ); 
	} else  if (nread == (size_t) -1 ) {
	   break;
	}
    }	    
    status = pclose(fp);
  }

  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( rval, S_VAR_LIST );
  arr = &rval->b.value.array_value;

  BINDING_DATA_init( &tmpv, S_VAR_INT );
  tmpv.b.value.long_value = status;
  VALARRAY_set( arr, 0, &tmpv, CP_VALUE );
 
  VALARRAY_set( arr, 1, runout, CP_REF );

}

static void x_kill( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  pid_t pid;
  POOH_INT tmp,signal,rt;

  arg = XCALL_param( xcall, 0 ); 
  BINDING_DATA_get_int( arg, &tmp );
  pid = (pid_t) tmp;

  arg = XCALL_param( xcall, 0 ); 
  BINDING_DATA_get_int( arg, &signal );

  rt = kill(  pid, signal );
  BINDING_DATA_set_int( XCALL_rvalue( xcall ), rt );
}

static void x_system( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALSTRING *sval;
  int rs;

  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );

  VALSTRING_make_null_terminated( sval );

  make_env( xcall );  

  rs = system( sval->string );

  BINDING_DATA_set_int( XCALL_rvalue( xcall ), rs );  
}

#if 0
static void x_trap( XCALL_DATA *xcall )
{
  (void) xcall;
}
#endif

/* time related functions */

static void maketime( XCALL_DATA *xcall, int is_local )
{
  struct tm tmm;
  POOH_INT now; 
  BINDING_DATA *rval,tmp, *arg;
  VALARRAY *arr;

  arg = XCALL_param( xcall, 0 ); 

  assert( sizeof( POOH_INT ) >= sizeof( time_t ) );
  
  if (IS_NULL( arg )) {
    time( (time_t *) &now ); 
  } else {
    BINDING_DATA_get_int( arg, &now );
  }
  
  if (is_local) {
    localtime_r( (time_t *) &now, &tmm );
  } else {
    gmtime_r( (time_t *) &now, &tmm );
  }

  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( rval, S_VAR_LIST );
  arr = &rval->b.value.array_value;

  BINDING_DATA_init( &tmp, S_VAR_INT );
  tmp.b.value.long_value = tmm.tm_sec;
  VALARRAY_set( arr, 0, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_min;
  VALARRAY_set( arr, 1, &tmp, CP_VALUE );
  
  tmp.b.value.long_value = tmm.tm_hour;
  VALARRAY_set( arr, 2, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_mday;
  VALARRAY_set( arr, 3, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_mon;
  VALARRAY_set( arr, 4, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_year + 1900;
  VALARRAY_set( arr, 5, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_wday;
  VALARRAY_set( arr, 6, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_yday;
  VALARRAY_set( arr, 7, &tmp, CP_VALUE );

  tmp.b.value.long_value = tmm.tm_isdst;
  VALARRAY_set( arr, 8, &tmp, CP_VALUE );
}

static void x_localtime( XCALL_DATA *xcall )
{
  maketime( xcall,1 ); 
} 

static void x_gmtime( XCALL_DATA *xcall )
{
  maketime( xcall, 0 ); 
} 

static void x_time( XCALL_DATA *xcall )
{
  time_t now; 

  time( &now ); 
  BINDING_DATA_set_int( XCALL_rvalue( xcall ), now );  
}

static void x_sleep( XCALL_DATA *xcall )
{
  BINDING_DATA *units;
  POOH_INT val;
  POOH_INT val_units = 0;
    
  BINDING_DATA_get_int( XCALL_param( xcall, 0 ), &val );
  
  units = XCALL_param( xcall, 1 );
  if (units) {
    BINDING_DATA_get_int( units, &val_units );
  }

  switch( val_units ) {
    case 1:  // millisecond sleep.
	usleep( val * 1000 );	
	break;
    case 2:  // microsecond sleep.
	usleep( val );	
	break;
    default:
	sleep( val );
	break;
  }
}

/* object support */

static void set_int_value( VALHASH *hash, const char *name, POOH_INT nvalue )
{
  BINDING_DATA key, *value;

  BINDING_DATA_set_tmp_string( &key, name, -1 );
  value = VALHASH_set_entry( hash, &key );
  BINDING_DATA_init( value, S_VAR_INT );
  value->b.value_flags_val = S_VAR_HEAP_VALUE;
  value->b.value.long_value = nvalue;
}

static void add_member_func( BINDING_DATA *obj, const char *func_name, AST_BASE *func )
{
  VALHASH *hash;
  BINDING_DATA key, *value;
  VALFUNCTION *code_value; 

  hash = &obj->b.value.hash_value;
  
  BINDING_DATA_set_tmp_string( &key, func_name, -1 );
	
  value = VALHASH_set_entry( hash, &key );
  BINDING_DATA_init( value, S_VAR_CODE );
  value->b.value_flags_val = S_VAR_HEAP_VALUE;
  
  code_value = &value->b.value.func_value;
  code_value->this_environment = obj;

  assert( func->type == S_FUN_DECL || func->type == S_XFUN_DECL );
  code_value->fdecl = func;
}


static void * get_imp( BINDING_DATA *obj )
{
  VALHASH *hash;
  BINDING_DATA key,*value, *nvalue;
  void *rvalue = 0;
  POOH_INT val = 0;

  hash = &obj->b.value.hash_value;
 
  BINDING_DATA_set_tmp_string( &key, (char *) &val, sizeof( val )  );

  if ( ! VALHASH_find( hash, &key, &value ) ) {
     nvalue = BINDING_DATA_follow_ref( value );
     assert( nvalue && nvalue->b.value_type & S_VAR_INT );
     rvalue = (void *) (size_t) nvalue->b.value.long_value;
  }

  return rvalue;
}

static void add_imp( BINDING_DATA *obj, void *imp )
{
  VALHASH *hash;
  BINDING_DATA key,*value;
  POOH_INT nkey = 0;
  hash = &obj->b.value.hash_value;
  
  BINDING_DATA_set_tmp_string( &key, (char *) &nkey, sizeof( nkey )  );
  
  value = VALHASH_set_entry( hash, &key );
  BINDING_DATA_init( value, S_VAR_INT );
  value->b.value_flags_val = S_VAR_HEAP_VALUE;
  value->b.value.long_value = (POOH_INT) (size_t) imp;

}

/* --- */

static void binary_to_array( VALARRAY *arr, unsigned char * buf, size_t buf_len )
{
    size_t i;
    BINDING_DATA *entry;

    VALARRAY_set_capacity( arr, buf_len );

    for(i = 0; i < buf_len; i ++ ) {
	entry = VALARRAY_get( arr, i );

        if (entry && entry->b.value_type & S_VAR_INT) {
	    entry->b.value.long_value = (POOH_INT) (unsigned long) buf[ i ];
        } else {
	    entry = BINDING_DATA_MEM_new( S_VAR_INT );
	    entry->b.value.long_value = (POOH_INT) (unsigned long) buf[ i ];
	    VALARRAY_set( arr, i, entry, CP_VALUE );
	}
    }
}

static unsigned char * array_to_binary( VALARRAY *arr, size_t towrite )
{
    size_t i;
    unsigned char *ret;
    BINDING_DATA *elm;
  
    ret = HEAP_alloc( towrite );

    for(i = 0; i < towrite; i++ ) {
	elm = VALARRAY_get( arr, i );
	if (elm && elm->b.value_type & S_VAR_INT) {
	  ret[ i ] = (unsigned char) elm->b.value.long_value;
	} else {
	  ret[ i ] = 0;
	}
    }
    return ret;
}

/* ------ */

typedef enum {
  TRANSP_FP,
  TRANSP_FD,
} TRANSPORT_TYPE;

typedef union tagTRANSPORT {
  FILE *fp;
  int   fd;
} TRANSPORT;

static POOH_INT do_read_imp(  TRANSPORT_TYPE transport_type, TRANSPORT trans, void *buf, size_t buf_size )
{
  POOH_INT nret;

  do {
    switch( transport_type ) {
      case TRANSP_FD:
        nret = read( trans.fd, buf, buf_size );
        break;
      case TRANSP_FP:
	if (!feof( trans.fp ) && !ferror( trans.fp )) 
	  nret = fread( buf, 1, buf_size, trans.fp);
	else
	  nret = -1;
	break;
    }
  } while( nret == (POOH_INT) -1 && errno == EINTR);
 
  return nret;
}

static size_t do_write_imp( TRANSPORT_TYPE transport_type, TRANSPORT trans,  void *buf, size_t buf_size )
{
  size_t nret;

  do {
    switch( transport_type ) {
      case TRANSP_FD:
        nret = write( trans.fd, buf, buf_size );
        break;
      case TRANSP_FP:
	if (!feof( trans.fp ) && !ferror( trans.fp )) 
	  nret = fwrite( buf, 1, buf_size, trans.fp);
	else
	  nret = -1;
	break;
    }
  } while( nret == (size_t) -1 && errno == EINTR);
 
  return nret;
}

static void read_imp( TRANSPORT_TYPE transport, TRANSPORT trans, XCALL_DATA *xcall )
{
    BINDING_DATA *toread, *tmp;
    size_t nlen;
    POOH_INT nret = -1;
 
    toread = XCALL_param( xcall, 0 );  
    toread = BINDING_DATA_follow_ref( toread ); 
 
    tmp =  XCALL_param( xcall, 1 ); 
    tmp = BINDING_DATA_follow_ref( tmp ); 
    nlen = tmp->b.value.long_value;


    if (toread->b.value_type & S_VAR_STRING ) {
	  VALSTRING *str = &toread->b.value.string_value;

	  VALSTRING_set_capacity( str, nlen );
   
          nret = do_read_imp( transport, trans, str->string, nlen );

	  str->length = nret;

    } else {
	  VALARRAY *arr; 	
	  unsigned char *bin;
	  assert( toread->b.value_type & S_VAR_LIST );
  
	  arr = &toread->b.value.array_value;
	  bin = HEAP_alloc( nlen );

          nret = do_read_imp( transport, trans, bin, nlen );

	  if (nret != 0) {
	    binary_to_array( arr, bin, nret );
	  }

	  free(bin);
    }
    
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), nret );  
}

static void write_imp( TRANSPORT_TYPE transport, TRANSPORT trans, XCALL_DATA *xcall )
{
    BINDING_DATA *obj, *towrite, *tmp;
    size_t nlen;
    int has_len = 0;
    size_t nret;

    obj = XCALL_this( xcall ); 
 
    towrite =  XCALL_param( xcall, 0 ); 
    towrite = BINDING_DATA_follow_ref( towrite ); 
 
    tmp =  XCALL_param( xcall, 1 ); 
    tmp = BINDING_DATA_follow_ref( tmp ); 
    if (!tmp) {
      nlen = tmp->b.value.long_value;
      has_len = 1;
    }

    if (towrite->b.value_type & S_VAR_STRING ) {
	VALSTRING *str = &towrite->b.value.string_value;

	if (!has_len) {
	    nlen = str->length;
	} else {
	    if (nlen > str->length) {
		nlen = str->length;
	    }
	}
     
        nret = do_write_imp( transport, trans, str->string, nlen );

	
    } else {
	VALARRAY *arr;       
	void *bin;

	assert( towrite->b.value_type & S_VAR_LIST );
	arr = &towrite->b.value.array_value;
 
	if (!has_len) {
	    nlen = arr->size; ;
	} else {
	    if (nlen > arr->size) {
		nlen = arr->size;
	    }
	}
	bin =  array_to_binary( arr, nlen );

        nret = do_write_imp( transport, trans, bin, nlen );
	
	free( bin );
    }
    
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), nret );  
}



/* files */


typedef struct tagFILE_IMP {
 
  FILE *fp;

} FILE_IMP;


static void x_file_read( XCALL_DATA *xcall )
{
    FILE_IMP *imp;
    BINDING_DATA *obj;
    TRANSPORT trans;
    
    obj =  XCALL_this( xcall ); 
    imp = (FILE_IMP *) get_imp( obj );
    if (!imp || !imp->fp) {
      BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
      return;
    }

    trans.fp = imp->fp;
    read_imp( TRANSP_FP, trans, xcall );
}

static void x_file_write( XCALL_DATA *xcall )
{
    FILE_IMP *imp;
    BINDING_DATA *obj;
    TRANSPORT trans;
  
    obj = XCALL_this( xcall ); 
    imp = (FILE_IMP *) get_imp( obj );
    if (!imp)
      return;
 
    trans.fp = imp->fp;
    write_imp( TRANSP_FP, trans, xcall );
}    


static void x_file_seek( XCALL_DATA *xcall )
{
    FILE_IMP *imp;
    BINDING_DATA *obj, *seek_type, *off;
    size_t offset, type;
    size_t nret = -1;

    obj =  XCALL_this( xcall ); 
    imp = (FILE_IMP *) get_imp( obj );
    if (!imp || !imp->fp) {
      BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
      return;
    }

  
    seek_type =  XCALL_param( xcall, 0 ); 
    type = seek_type->b.value.long_value;
 
    off =  XCALL_param( xcall, 1 ); 
    offset = off->b.value.long_value;

    nret = fseek( imp->fp, offset, type );
   
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), nret );  
}


static void x_file_close( XCALL_DATA *xcall )
{
    FILE_IMP *imp;
    BINDING_DATA *obj;

    obj =  XCALL_this( xcall ); 
    imp = (FILE_IMP *) get_imp( obj );
    if (!imp || !imp->fp) {
      BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
      return;
    }

    fclose( imp->fp );
    imp->fp = 0;
    free(imp);

    add_imp(  obj, 0 );

}

static void x_file_flush( XCALL_DATA *xcall )
{
    FILE_IMP *imp;
    BINDING_DATA *obj;

    obj =  XCALL_this( xcall ); 
    imp = (FILE_IMP *) get_imp( obj );
    if (!imp || !imp->fp) {
      BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
      return;
    }
 
    fflush( imp->fp );
}

static void x_file_parse( XCALL_DATA *xcall );

AST_XFUNC_DECL file_io_xlib[] = {

    DEFINE_XFUNC0( "flush", x_file_flush,	S_VAR_INT ),
    DEFINE_XFUNC2( "seek",  x_file_seek,	S_VAR_INT,	"mode",   S_VAR_INT,	"offset", S_VAR_INT ),
    DEFINE_XFUNC2( "read",  x_file_read,	S_VAR_INT,	"toread", S_VAR_STRING|S_VAR_LIST|S_VAR_PARAM_BYREF ,  "numread", S_VAR_INT ),
    DEFINE_XFUNC2( "write", x_file_write,	S_VAR_INT,	"towrite", S_VAR_STRING|S_VAR_LIST, "numwrite", S_VAR_INT | S_VAR_PARAM_OPTIONAL ),
    DEFINE_XFUNC1( "parse", x_file_parse,	S_VAR_HASH,	"grammar", S_VAR_GRAMMAR ),

    
    DEFINE_XFUNC0( "close", x_file_close,	S_VAR_INT ),
};

static void x_file_open( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*obj,*rval;
  VALSTRING *sval;
  char *c;
  int mode = 0;
  FILE *fp;
  char smode[10];
  FILE_IMP *imp;
  int has_open_existing = 0;

  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );
  VALSTRING_make_null_terminated( sval );
  
  imp =  (FILE_IMP *) HEAP_alloc( sizeof(FILE_IMP) );
  
  for( c = sval->string; *c != '\0'; ++c ) { 
    if ( *c == '+') {
       if (has_open_existing) {
         break;
       }
       has_open_existing = 1;
    } else 
    if ( *c == '<') {
      mode |= MODE_READ;
    } else if ( *c == '>') {
      if (mode & MODE_WRITE) {
       mode &= ~MODE_WRITE;
       mode |=  MODE_WRITE_APPEND;
      } else {
        mode |=  MODE_WRITE;
      }
    } else {
      break;
    }
  }

  if (mode == 0) {
    mode = MODE_READ;
  }

  strcpy(smode,"");
    
  if (mode & MODE_READ){
    strcat(smode,"r");
  }
  if (mode & MODE_WRITE_APPEND) {
    strcat(smode,"a");
  } else if (mode & MODE_WRITE) {
    strcat(smode,"w");
  }

  if (has_open_existing) {
    strcat(smode,"+");
  }

  obj = BINDING_DATA_MEM_new( S_VAR_HASH );
  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_copy( rval, obj, CP_REF );
  
  fp = fopen( c, smode );
  if (fp == 0) {
    return;
  }
     
 
  add_member_func( obj, "flush",    &file_io_xlib[0].base );
  add_member_func( obj, "seek",     &file_io_xlib[1].base );
  add_member_func( obj, "read",     &file_io_xlib[2].base );
  add_member_func( obj, "write",    &file_io_xlib[3].base );
  add_member_func( obj, "close",    &file_io_xlib[4].base );
  add_member_func( obj, "finalize", &file_io_xlib[4].base );
  imp->fp = fp;

  add_imp( obj, imp );
}

static void x_pipe_read( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;
  TRANSPORT trans;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp) {
   BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
   return;
  }
  
  trans.fd = imp->read_fd;
  read_imp( TRANSP_FD, trans, xcall );
}

static void x_pipe_write( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;
  TRANSPORT trans;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp) {
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
    return;
  }
  
  trans.fd = imp->write_fd;
  write_imp( TRANSP_FD, trans, xcall );
}

static void x_pipe_read_close( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp)
    return;
 
  if (imp->read_fd != -1) {
    close( imp->read_fd );
    imp->read_fd = -1;
  }
}

static void x_pipe_write_close( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp)
    return;
 
  if (imp->write_fd != -1) {
    close( imp->write_fd );
    imp->write_fd = -1;
  }
}

static void x_pipe_wait( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp)
    return;
   
  proc_wait( imp );
  BINDING_DATA_set_int( XCALL_rvalue( xcall ), imp->status );  

}
 
static void x_pipe_finalize( XCALL_DATA *xcall )
{
  PROC_OPEN *imp;
  BINDING_DATA *obj;

  obj =  XCALL_this( xcall ); 
  imp = (PROC_OPEN *) get_imp( obj );
  if (!imp)
    return;
 
  if (imp->write_fd != -1) {
    close( imp->write_fd );
    imp->write_fd = -1;
  }
  if (imp->read_fd != -1) {
    close( imp->read_fd );
    imp->read_fd = -1;
  }
  kill( imp->pid, SIGKILL );
  proc_wait(imp);
  free( imp );
}
 
AST_XFUNC_DECL io_pipe_xlib[] = {

  DEFINE_XFUNC2( "read",      x_pipe_read,	S_VAR_INT,	"toread", S_VAR_STRING|S_VAR_LIST|S_VAR_PARAM_BYREF ,  "numread", S_VAR_INT ),
  DEFINE_XFUNC0( "closeread", x_pipe_read_close,S_VAR_INT ),
  DEFINE_XFUNC2( "write",     x_pipe_write,	S_VAR_INT,	"towrite", S_VAR_STRING|S_VAR_LIST, "numwrite", S_VAR_INT | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC0( "closewrite",x_pipe_write_close,S_VAR_INT ),
  DEFINE_XFUNC0( "wait",      x_pipe_wait,	S_VAR_INT ),
  DEFINE_XFUNC0( "finalize",  x_pipe_finalize,	0 ),

};

static void x_pipe_open( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,*obj,*rval;
  VALSTRING *sval;
  char *c;
  int mode = 0;
  PROC_OPEN *imp;

  arg = XCALL_param( xcall, 0 ); 
  sval = BINDING_DATA_get_string( arg );
  VALSTRING_make_null_terminated( sval );
  
  imp =  (PROC_OPEN *) HEAP_alloc( sizeof(PROC_OPEN) );
  
  for( c = sval->string; *c != '\0'; ++c ) { 
    if ( *c == '<') {
      mode |= MODE_READ;
    } else if ( *c == '>') {
      mode |=  MODE_WRITE;
    } else {
      break;
    }
  }

  if (mode == 0) {
    mode = MODE_READ|MODE_WRITE;
  }

  obj = BINDING_DATA_MEM_new( S_VAR_HASH );
  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_copy( rval, obj, CP_REF );
  
  make_env( xcall ); 
  if  (proc_open( imp, c, mode )) {
      return;
  }
  
 
  set_int_value( &obj->b.value.hash_value , "pid", imp->pid );

  add_member_func( obj, "read",		&io_pipe_xlib[0].base );
  add_member_func( obj, "closeread",    &io_pipe_xlib[1].base );
  add_member_func( obj, "write",	&io_pipe_xlib[2].base );
  add_member_func( obj, "closewrite",   &io_pipe_xlib[3].base );
  add_member_func( obj, "wait",		&io_pipe_xlib[4].base );
  add_member_func( obj, "finalize",	&io_pipe_xlib[5].base );

  add_imp( obj, imp );
}




static void x_read_lines( XCALL_DATA *xcall )
{
  BINDING_DATA  *ret, *arg, *readbuf, *linestr, key, *func, *readres;
  VALHASH *hash;
  VALARRAY *aret;
  VALSTRING *sep, *sreadbuf, *slinestr;
  size_t nreadbuf, nlinestr, idx = 0;  
  VALFUNCTION  *readfun;
  POOH_INT rt;
  size_t frame_start;
  char *ssep, *prev, *next, *eof;
  int incomplete_line = 0;
  int iscoro = EVAL_THREAD_is_threadmain( xcall->thread );
  
  if (!iscoro) {
    ret = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( ret, S_VAR_LIST );
    aret = &ret->b.value.array_value;
  }

  // get file object argument.
  arg = XCALL_param( xcall, 0 ); 
  hash = BINDING_DATA_get_hash( arg );

  // get line separator argument.
  arg = XCALL_param( xcall, 1 ); 
  if (!IS_NULL(arg)) {
    sep = BINDING_DATA_get_string( arg );
    VALSTRING_make_null_terminated( sep);
    ssep = sep->string;
  } else {
    ssep = "\n";
  }

  // get read member of object
  BINDING_DATA_set_tmp_string( &key, "read", -1 );
  if ( VALHASH_find( hash, &key, &func) ) {
    return;
  }
  readfun = BINDING_DATA_get_fun( func );
  if (!readfun) {
    return;
  }
  
  // create temporary variables (read buffer, line buffer)
  readbuf = EVAL_THREAD_push_stack( xcall->thread , S_VAR_STRING );
  nreadbuf = ARRAY_size( &xcall->thread->binding_data_stack ) - 1;
  VALSTRING_set_capacity( &readbuf->b.value.string_value, 1025 );
 
  linestr = EVAL_THREAD_push_stack( xcall->thread , S_VAR_STRING );
  nlinestr = ARRAY_size( &xcall->thread->binding_data_stack ) - 1;

  
  for(;;) {

     // read some data from the file into read buffer
     frame_start = EVAL_THREAD_prepare_func_call( xcall->thread, readfun );

     arg = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 0 );
     readbuf = EVAL_THREAD_stack_offset( xcall->thread, nreadbuf );
     BINDING_DATA_copy(arg, readbuf, CP_REF );
      
     arg = EVAL_THREAD_parameter_ref( xcall->thread, frame_start, 1 );
     BINDING_DATA_set_int(arg,  1024 );

     EVAL_THREAD_proceed_func_call( xcall->thread, frame_start, readfun );
     readres = EVAL_THREAD_pop_stack( xcall->thread );
     
     BINDING_DATA_get_int( readres, &rt);
     if (rt == -1) {
       break;
     }
     
     
     // parse lines.
     readbuf = EVAL_THREAD_stack_offset( xcall->thread, nreadbuf );
     VALSTRING_make_null_terminated( &readbuf->b.value.string_value );
     sreadbuf = &readbuf->b.value.string_value; 
     linestr = EVAL_THREAD_stack_offset( xcall->thread, nlinestr );
     slinestr = &linestr->b.value.string_value; 
      
     eof = sreadbuf->string + sreadbuf->length;
     for( prev = sreadbuf->string, next = strstr( prev, ssep ); 
	  next != 0 && next < eof; 
	  ) {
       
       if (incomplete_line) {
         VALSTRING tmp;
	 VALSTRING_set_tmp_str( &tmp, prev, next - prev );
         VALSTRING_append( slinestr, &tmp );
	 incomplete_line = 0;
       } else {
	 VALSTRING_set( slinestr, prev, next - prev );
       }

       if (iscoro) 
         dothreadyield0_nomsg( linestr ); 
       else {
         ret = XCALL_rvalue( xcall ); 
         aret = &ret->b.value.array_value;
         VALARRAY_set( aret, idx++, linestr, CP_VALUE ); // hash keys in array may not me modified by reference, so copy them by value.
       }	     

       prev = next + strlen(ssep);
       if (prev >= eof) {
         break;
       }
       next = strstr( prev, ssep );
     }
    
     if ( prev < eof) {
       if (prev == sreadbuf->string) {
         VALSTRING tmp;
	 VALSTRING_set_tmp_str( &tmp, sreadbuf->string, sreadbuf->length );
         VALSTRING_append( slinestr, &tmp );
       }
       else {
	 VALSTRING_set( slinestr, prev, eof - prev );
       }
       incomplete_line = 1;
     }
 
  }

  if (incomplete_line) {
    linestr = EVAL_THREAD_stack_offset( xcall->thread, nlinestr );

    if (iscoro) 
      dothreadyield0_nomsg( linestr ); 
    else {
      ret = XCALL_rvalue( xcall ); 
      aret = &ret->b.value.array_value;
      VALARRAY_set( aret, idx++, linestr, CP_VALUE ); // hash keys in array may not me modified by reference, so copy them by value.
    }

  }
 
  EVAL_THREAD_discard_pop_stack( xcall->thread );
  EVAL_THREAD_discard_pop_stack( xcall->thread );
}

/* other files */

static void checkaccess( XCALL_DATA *xcall, int mode )
{
  BINDING_DATA *arg;
  VALSTRING *sarg;
  int nres;

  arg = XCALL_param( xcall, 0 ); 
  sarg = BINDING_DATA_get_string( arg );

  nres = access( sarg->string, mode );
  
  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_set_int(arg, !nres );
}

static void x_canread( XCALL_DATA *xcall )
{
  checkaccess( xcall, R_OK );
}

static void x_canwrite( XCALL_DATA *xcall )
{
  checkaccess( xcall, W_OK );
}

static void x_canrun( XCALL_DATA *xcall )
{
  checkaccess( xcall, X_OK );
}


static void x_fsize( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALSTRING *str;
  struct stat buf;
  
  arg = XCALL_param( xcall, 0 ); 
  str = BINDING_DATA_get_string( arg );
  VALSTRING_make_null_terminated( str );

  arg = XCALL_rvalue( xcall ); 
  if ( stat( str->string, &buf ) ) {
   BINDING_DATA_set_int(arg,  -1 );
   return;
  }
  BINDING_DATA_set_int(arg,  buf.st_size );
}

static void x_stat( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *rval;
  VALSTRING *str;
  struct stat buf;
  VALHASH *hash;
  
  arg = XCALL_param( xcall, 0 ); 
  str = BINDING_DATA_get_string( arg );
  VALSTRING_make_null_terminated( str );

  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( rval, S_VAR_HASH );
 
  if ( stat( str->string, &buf ) ) {
    return;
  }
  
  hash = &rval->b.value.hash_value;

  set_int_value( hash, "dev", buf.st_dev );
  set_int_value( hash, "ino", buf.st_ino );
  set_int_value( hash, "mode", buf.st_mode );
  set_int_value( hash, "nlink", buf.st_nlink );
  set_int_value( hash, "uid", buf.st_uid );
  set_int_value( hash, "gid", buf.st_gid );
  set_int_value( hash, "rdev", buf.st_rdev );
  set_int_value( hash, "size", buf.st_size );
  set_int_value( hash, "blksize", buf.st_blksize );
  set_int_value( hash, "blocks", buf.st_blocks );
  set_int_value( hash, "atime", buf.st_atime );
  set_int_value( hash, "mtime", buf.st_mtime );
  set_int_value( hash, "ctime", buf.st_ctime );
 
}

char * join_strings( VALARRAY *arr )
{
  BINDING_DATA *data;
  VALSTRING *vals;
  size_t len = 0;
  char *ret;
  size_t i;

  for( i = 0; i < VALARRAY_size( arr ); i ++ ) {
    if ( i > 0)
      len += 1;

    data = VALARRAY_get( arr , i );
    data = BINDING_DATA_follow_ref( data );

    if ((data->b.value_type & S_VAR_STRING) == 0) {
      return 0;	
    }
    vals = &data->b.value.string_value;
    len += vals->length;
  }

  ret = malloc( len + 1 );
  if (!ret) {
    return 0;
  }
  strcpy( ret, "" );
  
  for( i = 0; i < VALARRAY_size( arr ); i ++ ) {
    if ( i > 0)
      strcat( ret, " " );

    data = VALARRAY_get( arr , i );
    data = BINDING_DATA_follow_ref( data );

    vals = &data->b.value.string_value;
    VALSTRING_make_null_terminated( vals ); 
    strcat( ret, vals->string );
  }
  return ret;
}

static void x_listdir( XCALL_DATA *xcall )
{
  wordexp_t p;
  char **w;
  BINDING_DATA *arg, *ret, *tmps;
  VALSTRING *str;
  VALARRAY *aret;
  size_t i;
  int force_exit;
  int top = EVAL_THREAD_is_threadmain( xcall->thread );
  
  if (!top) {
    ret =  BINDING_DATA_MEM_new( S_VAR_LIST ); 
    aret = &ret->b.value.array_value;
    BINDING_DATA_copy( XCALL_rvalue( xcall ), ret, CP_REF );
  }
  
  arg = XCALL_param( xcall, 0 ); 

  arg = BINDING_DATA_follow_ref( arg );
  if (arg->b.value_type & S_VAR_STRING) {
    str = BINDING_DATA_get_string( arg );
    VALSTRING_make_null_terminated( str ); 
    wordexp(str->string, &p, 0);
  } else {
    char *vals;

    assert( (arg->b.value_type & S_VAR_LIST) != 0 );
    vals = join_strings( &arg->b.value.array_value );    
    if (!vals) {
     return;
    }
    wordexp( vals, &p, 0);
    free(vals);
  }

 
  w = p.we_wordv;
  for (i = 0; i < p.we_wordc; i++) {
    if (top) {
       tmps = BINDING_DATA_MEM_new( S_VAR_STRING );
       VALSTRING_set( &tmps->b.value.string_value, w[i], strlen( w[i] ) );
       dothreadyield( tmps, &force_exit );
       if (force_exit) {
         break;
       }
    } else {
       tmps = VALARRAY_set_entry( aret, i );
       BINDING_DATA_init( tmps, S_VAR_STRING );
       VALSTRING_set( &tmps->b.value.string_value, w[i], strlen( w[i] ) );
    }
  }
  wordfree( &p );
}

/* parsing */

// fill the circular lookahead buffer with data; do some from FILE source.
int PARSER_DATA_SRC_file (struct tagCIRCBUF *buf, size_t *read, void *ctx )
{
   FILE *fl = (FILE *) ctx;
   size_t rt, sz, nread = 0;
   int wrap_buffer;


   wrap_buffer = buf->read_pos < buf->write_pos;
   sz = buf->maxcount - buf->write_pos; 
   rt = fread( buf->data + buf->write_pos, sz, 1, fl );
   if (rt > 0) {
      nread += rt;
      buf->write_pos += rt;
      if (wrap_buffer && rt == sz && buf->read_pos > 0) {
          rt = fread( buf->data, sz, 1, fl );
	  if (rt > 0) {
	    nread += rt;
	    buf->write_pos = rt;
	  }
      }
   }

   *read = nread;
   return nread != 0;
}

static void make_parse_rvalue(  XCALL_DATA *xcall, int rt, PP_BASE_INFO *pinfo )
{
  BINDING_DATA *rval, tmp;
  VALARRAY *arr;

  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( rval, S_VAR_LIST );
  arr = &rval->b.value.array_value;

  BINDING_DATA_init( &tmp, S_VAR_INT );
  tmp.b.value.long_value = rt;
  VALARRAY_set( arr, 0, &tmp, CP_VALUE );

  VALARRAY_set( arr, 1, pinfo->data, CP_REF );


}

int GRAMMAR_run_implx( AST_BASE *grammar, PEG_PARSER_DATA_SRC data_cb, void *data_cb_ctx, PP_BASE_INFO *pinfo  );

static void x_file_parse( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *obj;
  AST_BASE *grammar;
  FILE_IMP *imp;
  PP_BASE_INFO pinfo;
  int rt;

  arg = XCALL_param( xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );
  grammar = arg->b.value.grammar_value;

  obj =  XCALL_this( xcall ); 
  imp = (FILE_IMP *) get_imp( obj );
  if (!imp || !imp->fp) {
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
    return;
  }

  rt = GRAMMAR_run_implx(  grammar, PARSER_DATA_SRC_file, imp->fp, &pinfo );
  make_parse_rvalue( xcall, rt, &pinfo );

}


static void x_parse( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  AST_BASE *grammar;
  VALSTRING *str;
  FILE *fp;
  PP_BASE_INFO pinfo;
  int rt;

  arg = XCALL_param( xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );
  grammar = arg->b.value.grammar_value;

  arg = XCALL_param( xcall, 0 ); 
  arg = BINDING_DATA_follow_ref( arg );

  str = &arg->b.value.string_value; 

  fp = fmemopen( str->string, str->length, "r" );
  if (!fp) {
    BINDING_DATA_set_int( XCALL_rvalue( xcall ), -1 );  
    return;
  }

  rt =  GRAMMAR_run_implx(  grammar, PARSER_DATA_SRC_file, fp, &pinfo );
  make_parse_rvalue( xcall, rt, &pinfo );

}


static void x_terminfo( XCALL_DATA *xcall )
{
  BINDING_DATA *data;
  EVAL_THREAD *th = xcall->thread;
  PEG_PARSER *parse;
  POOH_INT index;
  PP_BASE_INFO *pinfo;

  if (!th->parse_impl) {
     return;
  }
  parse = (PEG_PARSER *) th->parse_impl;

  data = XCALL_param( xcall, 0 ); 
  BINDING_DATA_get_int( data, &index );

  if (parse->current_rhs_clause_len < index || index == 0)  {
    return;
  }
 
  pinfo = parse->current_rhs_clause + index - 1;

#if 0
  BINDING_DATA *obj, *rval;

  obj = BINDING_DATA_MEM_new( S_VAR_HASH );
  rval = XCALL_rvalue( xcall ); 
  BINDING_DATA_copy( rval, obj, CP_REF );
#endif
}



/* -------------- the library ------------------------- */
AST_XFUNC_DECL xlib[] = {

/* arrays */  
  DEFINE_XFUNC1( "eachinarray", x_eachinarray,	S_VAR_LIST,	"array", S_VAR_LIST | S_VAR_PARAM_BYREF ),
  DEFINE_XFUNC3( "range",   x_range,	S_VAR_LIST,	"from", S_VAR_INT | S_VAR_DOUBLE, "to", S_VAR_INT | S_VAR_DOUBLE, "step", S_VAR_INT | S_VAR_DOUBLE | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC2( "push",    x_push,	0,		"array", S_VAR_LIST | S_VAR_PARAM_BYREF,  "top",  S_VAR_ANY ),
  DEFINE_XFUNC1( "pop",	    x_pop,	S_VAR_ANY,	"array", S_VAR_LIST | S_VAR_PARAM_BYREF ),
  DEFINE_XFUNC2( "unshift",x_unshift,	0,		"array", S_VAR_LIST | S_VAR_PARAM_BYREF ,  "first",  S_VAR_ANY ),
  DEFINE_XFUNC1( "shift",   x_shift,	S_VAR_ANY,	"array", S_VAR_LIST | S_VAR_PARAM_BYREF ),
  DEFINE_XFUNC2( "append",  x_append,	0,		"array", S_VAR_LIST | S_VAR_PARAM_BYREF, "add", S_VAR_LIST ),
  DEFINE_XFUNC1( "reverse", x_reverse,	S_VAR_LIST,	"array", S_VAR_LIST | S_VAR_PARAM_BYREF ),
  DEFINE_XFUNC2( "join",    x_join,	S_VAR_STRING,	"separator", S_VAR_STRING,  "array", S_VAR_LIST ),
  DEFINE_XFUNC2( "sort",    x_sort,	S_VAR_LIST,	"array", S_VAR_LIST,  "func", S_VAR_CODE ),


/* hashes */
  DEFINE_XFUNC1( "keys",   x_keys,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC1( "values", x_values,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC1( "each",   x_each,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC2( "exists", x_exists,	S_VAR_INT,  "table", S_VAR_HASH, "key", S_VAR_ANY ),
  DEFINE_XFUNC2( "exist",  x_exists,	S_VAR_INT,  "table", S_VAR_HASH, "key", S_VAR_ANY ),
  DEFINE_XFUNC2( "erase",  x_erase,	0,	    "table", S_VAR_HASH | S_VAR_PARAM_BYREF, "key", S_VAR_ANY ),

/* strings */
  DEFINE_XFUNC2( "find",   x_find,      S_VAR_INT,  "hay", S_VAR_STRING, "needle", S_VAR_STRING ),
  DEFINE_XFUNC3( "mid",	   x_mid,       S_VAR_STRING, "string", S_VAR_STRING, "offset", S_VAR_INT | S_VAR_PARAM_OPTIONAL, "length", S_VAR_INT | S_VAR_PARAM_OPTIONAL),
  DEFINE_XFUNC2( "left",   x_left,      S_VAR_STRING, "string", S_VAR_STRING, "length", S_VAR_INT ),
  DEFINE_XFUNC2( "right",  x_right,     S_VAR_STRING, "string", S_VAR_STRING, "length", S_VAR_INT ),
  DEFINE_XFUNC2( "split",  x_split,     S_VAR_LIST,   "string", S_VAR_STRING, "separator", S_VAR_STRING | S_VAR_PARAM_OPTIONAL ),
   
  DEFINE_XFUNC1( "print",  x_print,     0,            "msg", S_VAR_STRING | S_VAR_INT | S_VAR_DOUBLE | S_VAR_LIST ),
  DEFINE_XFUNC1( "println",x_println,   0,            "msg", S_VAR_STRING | S_VAR_INT | S_VAR_DOUBLE | S_VAR_LIST ),
  DEFINE_XFUNC1( "int",	   x_toint,     S_VAR_INT, "string", S_VAR_STRING ),
  DEFINE_XFUNC1( "hex",	   x_tohex,     S_VAR_INT, "string", S_VAR_STRING ),
  DEFINE_XFUNC1( "oct",	   x_tooct,     S_VAR_INT, "string", S_VAR_STRING ),
  DEFINE_XFUNC0( "emptystring", x_emptystring, S_VAR_STRING ),
  DEFINE_XFUNC0( "newline",  x_newline, S_VAR_STRING ),
      

/* numeric functions */
  DEFINE_XFUNC1( "srand",   x_srand, 0,  "seed", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "rand",    x_rand, S_VAR_DOUBLE,  "max", S_VAR_INT | S_VAR_DOUBLE | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC1( "abs",     x_abs, S_VAR_INT | S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "sqr",     x_sqrt, S_VAR_DOUBLE,  "num",  S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC2( "pow",     x_pow, S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE, "power", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "loge",    x_loge, S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "sin",     x_sin, S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "asin",    x_asin, S_VAR_DOUBLE, "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "cos",     x_cos, S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "acos",    x_acos, S_VAR_DOUBLE, "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "tan",     x_tan, S_VAR_DOUBLE,  "num", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "atan",    x_atan, S_VAR_DOUBLE, "num", S_VAR_INT | S_VAR_DOUBLE ),

  DEFINE_XFUNC0( "maxfloat",  x_maxfloat, S_VAR_DOUBLE ),
  DEFINE_XFUNC0( "minfloat",  x_minfloat, S_VAR_DOUBLE ),
  DEFINE_XFUNC0( "maxint",    x_maxint, S_VAR_INT ),
  DEFINE_XFUNC0( "minint",    x_minint, S_VAR_INT ),


/* Miscellaneous functions */
  DEFINE_XFUNC1( "size",    x_size,	    S_VAR_INT,	"arg", S_VAR_STRING | S_VAR_LIST | S_VAR_HASH ),
  DEFINE_XFUNC1( "defined", x_defined,	    S_VAR_INT,	"arg", S_VAR_ANY ),  
  DEFINE_XFUNC1( "dump",    x_dump,	    0,		"arg", S_VAR_ANY ),  
  DEFINE_XFUNC1( "trace",   x_trace,	    0,		 "onoff", S_VAR_INT ),
  DEFINE_XFUNC0( "showstack", x_showstack,  0 ),
  DEFINE_XFUNC0( "osname",  x_osname,	    S_VAR_STRING ),
  DEFINE_XFUNC1( "type",    x_typeof,	    S_VAR_STRING, "arg", S_VAR_ANY ),  
  DEFINE_XFUNC2( "exit",    x_exit,         0,          "msg", S_VAR_STRING | S_VAR_PARAM_OPTIONAL, "status", S_VAR_INT ), 
  DEFINE_XFUNC0( "collectgarbage",    x_collectgarbage,  0 ), 

/* Reflection */
#if 0
  DEFINE_XFUNC1( "functionparameters",    x_functionparameters,   S_VAR_LIST, "function", S_VAR_CODE | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC1( "functionreturntype",    x_functionreturntype,   S_VAR_LIST, "function", S_VAR_CODE | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC2( "applyfunc",    x_applyfunc,   S_VAR_ANY, "function", S_VAR_CODE, "params", S_VAR_LIST  ),
#endif

  DEFINE_XFUNC1( "isarray",      x_isarray,   S_VAR_INT, "value", S_VAR_ANY  ),
  DEFINE_XFUNC1( "isfunction",   x_isfunction,   S_VAR_INT, "value", S_VAR_ANY  ),
  DEFINE_XFUNC1( "istable",      x_istable,   S_VAR_INT, "value", S_VAR_ANY  ),
  DEFINE_XFUNC1( "isstring",     x_isstring,   S_VAR_INT, "value", S_VAR_ANY  ),
  DEFINE_XFUNC1( "isnumber",     x_isnumber,   S_VAR_INT, "value", S_VAR_ANY  ),

/* co-routine threads */
  DEFINE_XFUNC1( "makethread",	 x_make_thread,	 S_VAR_CODE, "func", S_VAR_CODE ),  
  DEFINE_XFUNC1( "isthreadmain", x_isthreadmain, S_VAR_INT,  "thread", S_VAR_CODE ),  
  DEFINE_XFUNC0( "threadexit",	 x_threadexit, 0	 ),  
  DEFINE_XFUNC1( "stopthread",	 x_stopthread,	0,	    "thread", S_VAR_CODE ),  
  DEFINE_XFUNC1( "threadyield",	 x_threadyield,	S_VAR_LIST,  "yieldval", S_VAR_ANY | S_VAR_PARAM_OPTIONAL ),  
  DEFINE_XFUNC1( "threadyield0", x_threadyield0,	S_VAR_ANY,   "yieldval", S_VAR_ANY | S_VAR_PARAM_OPTIONAL ),  
  DEFINE_XFUNC2( "resumethread", x_resume,	S_VAR_ANY,  "thread", S_VAR_CODE, "message", S_VAR_ANY | S_VAR_PARAM_OPTIONAL ),  
  DEFINE_XFUNC1( "threadyieldvalue",x_threadyieldvalue, S_VAR_LIST, "thread", S_VAR_CODE ),  

/* higher order functions */
  DEFINE_XFUNC2( "mapcopy",  x_mapcopy,	S_VAR_LIST,	"array", S_VAR_LIST, "func", S_VAR_CODE ),
  DEFINE_XFUNC2( "filtercopy", x_filtercopy,	S_VAR_LIST,	"array", S_VAR_LIST, "func",  S_VAR_CODE ),
 
  DEFINE_XFUNC2( "mapreplace",  x_mapreplace,	0,	"array", S_VAR_LIST, "func", S_VAR_CODE ),


  DEFINE_XFUNC3( "foldfirst2last", x_foldfirst2last,	S_VAR_ANY,	"array", S_VAR_LIST, "func", S_VAR_CODE, "initval", S_VAR_ANY ),
  DEFINE_XFUNC3( "foldlast2first",x_foldlast2first,	S_VAR_ANY,	"array", S_VAR_LIST, "func", S_VAR_CODE, "initval", S_VAR_ANY ),
 
/* function for processes and process groups */
  DEFINE_XFUNC1( "run",      x_run,	S_VAR_LIST,	"command",  S_VAR_STRING ),
  DEFINE_XFUNC1( "system",   x_system,	S_VAR_INT,    	"command",  S_VAR_STRING ),
  DEFINE_XFUNC2( "kill",     x_kill,	0,		"pid", S_VAR_INT, "signal", S_VAR_INT  ),
 
/* time related functions */
  DEFINE_XFUNC1( "localtime", x_localtime, S_VAR_LIST, "epoch", S_VAR_INT | S_VAR_PARAM_OPTIONAL  ),
  DEFINE_XFUNC1( "gmtime",    x_gmtime,    S_VAR_LIST, "epoch", S_VAR_INT | S_VAR_PARAM_OPTIONAL  ),
  DEFINE_XFUNC0( "currenttime",      x_time,      S_VAR_INT ),
  DEFINE_XFUNC2( "sleep",     x_sleep,   0,  "delay", S_VAR_INT, "units", S_VAR_INT | S_VAR_PARAM_OPTIONAL ),
     
/* function for error handling */
#if 0
  DEFINE_XFUNC2( "trap",      x_trap,	 0,  "errortype",  S_VAR_STRING | S_VAR_PARAM_OPTIONAL, "func", S_VAR_CODE ),
#endif  

/* functions for input and output, files and directories */
  DEFINE_XFUNC1( "open",      x_file_open, S_VAR_HASH,  "filename",  S_VAR_STRING  ),
  DEFINE_XFUNC1( "openpipe",  x_pipe_open, S_VAR_HASH,  "cmdline",   S_VAR_STRING  ),
  DEFINE_XFUNC2( "readlines", x_read_lines,S_VAR_LIST, "file", S_VAR_HASH, "separator", S_VAR_STRING | S_VAR_PARAM_OPTIONAL),
  DEFINE_XFUNC1( "stat",      x_stat,      S_VAR_HASH,  "filename",  S_VAR_STRING  ),
  DEFINE_XFUNC1( "filesize",  x_fsize,     S_VAR_INT,   "filename",  S_VAR_STRING  ),
  DEFINE_XFUNC1( "listdir",   x_listdir,   S_VAR_LIST,  "pattern",   S_VAR_STRING | S_VAR_LIST  ),

  DEFINE_XFUNC1( "canread",   x_canread,   S_VAR_INT,   "filename",  S_VAR_STRING  ),
  DEFINE_XFUNC1( "canwrite",  x_canwrite,  S_VAR_INT,   "filename",  S_VAR_STRING  ),
  DEFINE_XFUNC1( "canrun",    x_canrun,    S_VAR_INT,   "filename",  S_VAR_STRING  ),
  
/* functions for parsing */
  DEFINE_XFUNC2( "parse",     x_parse, S_VAR_HASH,	"rules",     S_VAR_GRAMMAR, "input", S_VAR_STRING | S_VAR_HASH  ), 
  DEFINE_XFUNC1( "terminfo",  x_terminfo,  S_VAR_HASH,	"num",       S_VAR_INT ), 


/* eof */
  DEFINE_XFUNC1( 0, 0, 0, "", 0 )
};

AST_XFUNC_DECL *get_each_in_array_xfunc() {
    return &xlib[ 0 ];
}

