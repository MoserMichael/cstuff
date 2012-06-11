#include "rtlib.h"
#include <math.h>


/* --------------- function for more than one type ------------------- */
static void x_size( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  size_t rt;
  
  arg = XCALL_param(xcall, 0 ); 
  
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
  BINDING_DATA_set_int( XCALL_rvalue(xcall), ! IS_NULL( arg ) );
}

static void x_dump( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_print( stderr, arg , 0 );
}

static void x_trace( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  int trace;
  
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_get_int( arg, (long *) &trace );
  xcall->thread->trace_on = trace;
}

static void x_typeof( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  const char *tname;
  
  arg = XCALL_param(xcall, 0 ); 
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
	tname = "Hash";
	break;
    case S_VAR_LIST:
	tname = "List";
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
  BINDING_DATA *arg;
//  VALFUNCTION *func;

  arg = XCALL_param(xcall, 0 ); 
#if 0
 func = &arg->b.value.func_value;

 if (ARRAY_size( & func->captures ) != 0) {
      // either disallow captures for threads, or copy all captures when a thread is created ?
  }
#endif

  // when the function value is created, a new thread will be created.
  arg->b.value_type |= S_VAR_CODE_THREAD;  
  //VALFUNCTION_mark_as_thread( &arg->b.value.func_value );
}

static void x_yield( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  BINDING_DATA *msg_val; 

  arg = XCALL_param(xcall, 0 ); 

  EVAL_THREAD_yield_value( xcall->thread, arg, &msg_val );

  memcpy( XCALL_rvalue(xcall), msg_val, sizeof(BINDING_DATA) );
}

static void x_resume( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  BINDING_DATA *msg_val; 

  arg = XCALL_param(xcall, 0 ); 

  EVAL_THREAD_activate( xcall->thread, arg, &msg_val );

  memcpy( XCALL_rvalue(xcall), msg_val, sizeof(BINDING_DATA) );
}

static void x_isactive( XCALL_DATA *xcall )
{
  (void) xcall;
}

static void  x_stopthread( XCALL_DATA *xcall )
{
  (void) xcall;
}

/* -------------- function for arrays --------------------------- */

static void x_range( XCALL_DATA *xcall )
{
  BINDING_DATA tmpn,*arg,*rval;
  VALARRAY *arr;
  long idx, from, to, step;
  int top = EVAL_THREAD_is_top_level_frame( xcall->thread );
   
  arg = XCALL_param(xcall, 0 ); 
  BINDING_DATA_get_int( arg, (long *) &from );

  arg = XCALL_param(xcall, 1 ); 
  BINDING_DATA_get_int( arg, (long *) &to );

  step = 1;
  arg = XCALL_param(xcall, 2 ); 
  if (! IS_NULL(arg)) {
    BINDING_DATA_get_int( arg, (long *) &step );
  }

  
  BINDING_DATA_init( &tmpn, S_VAR_INT );
 
  if (top) {
    for(idx = 0 ; idx < (to - from) ; idx += step ) {
       tmpn.b.value.long_value = from + idx;
       EVAL_THREAD_yield_value( xcall->thread, &tmpn, 0 );
    }
  } else {
    rval = XCALL_rvalue( xcall ); 
    BINDING_DATA_init( rval, S_VAR_LIST );
    arr = &rval->b.value.array_value;

    for(idx = 0 ; idx < (to - from) ; idx += step ) {
       tmpn.b.value.long_value = from + idx;
       VALARRAY_set( arr, idx , &tmpn, CP_VALUE );
    }
  }
}


static void x_push( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALARRAY *arr;

  arg = XCALL_param(xcall, 0 ); 
  arr = &arg->b.value.array_value;

  VALARRAY_set( arr, VALARRAY_size( arr ), XCALL_param(xcall, 1 ), CP_REF );
}

static void x_pop( XCALL_DATA *xcall )
{
  BINDING_DATA *arg, *tmp;
  VALARRAY *arr;
  
  arg = XCALL_param(xcall, 0 ); 
  arr = &arg->b.value.array_value;

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
  char stmp[ 64 ];

  arg = XCALL_param( xcall, 0 ); 
  sdelim = &arg->b.value.string_value;

  arg = XCALL_param( xcall, 1 ); 
  arr = &arg->b.value.array_value;

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
         sprintf( stmp, "%ld", val->b.value.long_value );
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

static void x_reverse( XCALL_DATA *xcall )
{
  BINDING_DATA *arg,tmp;
  VALARRAY *arr;
  size_t i, n;

  arg = XCALL_param(xcall, 0 ); 
  arr = &arg->b.value.array_value;

  n = arr->size >> 1;

  for( i = 0; i < n; i++ ) {
    memcpy( &tmp, &arr->data[ i ], sizeof( BINDING_DATA ) );
    memcpy( &arr->data[ i ], &arr->data[ arr->size - i ], sizeof( BINDING_DATA ) );
    arr->data[ i ].b.container = arg;

    memcpy( &arr->data[ arr->size - i ] , &tmp , sizeof( BINDING_DATA ) );
    arr->data[ arr->size - i ].b.container = arg;
  }
}

/* --------------- higher order functions  -------------------------- */

static void x_map( XCALL_DATA *xcall )
{
  BINDING_DATA *arg;
  VALARRAY *arr;
  VALFUNCTION *func;
  
  arg = XCALL_param(xcall, 0 ); 
  arr = &arg->b.value.array_value;

  arg = XCALL_param(xcall, 1 );
  func = &arg->b.value.func_value;

  // check that function argument prototype can be used.
}

static void x_copymap( XCALL_DATA *xcall )
{
  (void) xcall;
}

static void x_filter( XCALL_DATA *xcall )
{
  (void) xcall;
}

static void x_foldl( XCALL_DATA *xcall )
{
  (void) xcall;
}

static void x_foldr( XCALL_DATA *xcall )
{
  (void) xcall;
}

 
/* --------------- function for hashes ------------------------------ */
static void x_exists ( XCALL_DATA *xcall )
{

  VALHASH *hash;
  BINDING_DATA *arg, *key, *rval;
  int rt;

  arg = XCALL_param( xcall, 0 ); 
  hash = &arg->b.value.hash_value;

  key = XCALL_param( xcall, 0 ); 

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
  BINDING_DATA *key,*value, *rval;
  size_t idx = 0;
  int top =  EVAL_THREAD_is_top_level_frame( xcall->thread );
   
  
  arg = XCALL_param( xcall, 0 ); 
  hash = &arg->b.value.hash_value;
  
  if (top) {
    VALHASHPOS_init( &pos );
    while( VALHASH_iterate( hash, &key, &value, &pos ) ) {
      switch( act ) {
         case HGET_KEYS:
         EVAL_THREAD_yield_value( xcall->thread, key, 0 );
         break;
       case HGET_VALUES:
         EVAL_THREAD_yield_value( xcall->thread, value, 0 );
         break;
       case HGET_EACH:
         elm = BINDING_DATA_MEM_new( S_VAR_LIST );
         aelm = &elm->b.value.array_value;
         VALARRAY_set_capacity( aelm, 2 );
         VALARRAY_set( aelm, 0, key , CP_REF );
         VALARRAY_set( aelm, 1, value , CP_REF );
         EVAL_THREAD_yield_value( xcall->thread, elm, 0 );
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
         VALARRAY_set( rarr, idx++, key, CP_REF );
         break;
       case HGET_VALUES:
         VALARRAY_set( rarr, idx++, value, CP_REF );
         break;
       case HGET_EACH:
         elm = BINDING_DATA_MEM_new( S_VAR_LIST );
         aelm = &elm->b.value.array_value;
         VALARRAY_set_capacity( aelm, 2 );
         VALARRAY_set( aelm, 0, key , CP_REF );
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

typedef enum {
  STR_LEFT,
  STR_RIGHT,
  STR_MID,
} STR_ACTION;

static void mid_imp( XCALL_DATA *xcall, STR_ACTION act)
{
  BINDING_DATA *arg;
  VALSTRING *sval,*rval;
  size_t n1,n2;

  arg = XCALL_param( xcall, 0 ); 
  sval =  &arg->b.value.string_value;

  arg = XCALL_param( xcall, 0 );
  BINDING_DATA_get_int( arg, (long *) &n1 );

  if (act == STR_MID) {
    arg = XCALL_param( xcall, 0 );
    BINDING_DATA_get_int( arg, (long *) &n2 );
    
    if (n2 >= sval->length) {
      n2 = sval->length - 1;
    }
  }

  if (n1 >= sval->length) {
    n1 = sval->length - 1;
  }

  arg = XCALL_rvalue( xcall ); 
  BINDING_DATA_init( arg, S_VAR_LIST );
  rval = &arg->b.value.string_value;

  switch(act) {
    case STR_LEFT:
      VALSTRING_substr( rval, sval, 0, n1 );
      break;
    case STR_MID:
      VALSTRING_substr( rval, sval, n1, n2);
      break;
    case STR_RIGHT:
      VALSTRING_substr( rval, sval, n2, sval->length );
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
static void x_print_imp( BINDING_DATA *data )
{
  VALSTRING *sdata;
  VALARRAY *arr;
  size_t i;

 
  switch( data->b.value_type ) {
     case S_VAR_INT:
       fprintf( stdout, "%ld", data->b.value.long_value );
       break;
     case S_VAR_DOUBLE:
       fprintf( stdout, "%f", data->b.value.double_value );
       break;
     case S_VAR_STRING:
       sdata = &data->b.value.string_value;
       fprintf( stdout, "%.*s", sdata->length, sdata->string );
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
}

static void x_println( XCALL_DATA *xcall )
{
  x_print_imp( XCALL_param( xcall, 0 ) );
  fprintf( stdout, "\n" );
}




/* -------------- numeric functions ---------------------- */
static void x_srand( XCALL_DATA *xcall )
{
   long nmaxn;

   BINDING_DATA_get_int( XCALL_param( xcall, 0 ), &nmaxn );
   srand( (unsigned long) nmaxn );
}
 
static void x_rand( XCALL_DATA *xcall )
{
  BINDING_DATA *maxn = XCALL_param( xcall, 0 );
  long nmaxn;
  double res;

  if (maxn) {
     BINDING_DATA_get_int( maxn, &nmaxn );
  } 
 
  res = rand() / RAND_MAX;
  if (maxn) {
     res *= nmaxn;
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
  long larg;

  arg = XCALL_param( xcall, 0 );
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

/* -------------- the library ------------------------- */


AST_XFUNC_DECL xlib[] = {

//to add: exit( ~msg ) shift ( ~array ) , unshift( ~array, ~start)  functionparameters ( [ ~function ] ) applyfunc( ~func ~params  )  emptystring() - returns empty string.
 
/* Miscellaneous functions ;  more than one argument type */
  DEFINE_XFUNC1( "size",    x_size,	    S_VAR_INT,	"arg", S_VAR_STRING | S_VAR_LIST | S_VAR_HASH ),
  DEFINE_XFUNC1( "defined", x_defined,	    S_VAR_INT,	"arg", S_VAR_ANY ),  
  DEFINE_XFUNC1( "dump",    x_dump,	    0,		"arg", S_VAR_ANY ),  
  DEFINE_XFUNC1( "trace",   x_trace,	    0,		 "onoff", S_VAR_INT ),  
  DEFINE_XFUNC1( "type",    x_typeof,	    S_VAR_STRING, "arg", S_VAR_ANY ),  
   
/* threads */
  DEFINE_XFUNC1( "makethread",  x_make_thread,	S_VAR_CODE, "thread", S_VAR_CODE ),  
  DEFINE_XFUNC1( "yield",	x_yield,	S_VAR_ANY, "message", S_VAR_ANY | S_VAR_PARAM_OPTIONAL ),  
  DEFINE_XFUNC2( "resume",	x_resume,	S_VAR_ANY, "thread", S_VAR_CODE, "message", S_VAR_ANY | S_VAR_PARAM_OPTIONAL ),  
  DEFINE_XFUNC1( "isactive",	x_isactive,	S_VAR_INT, "thread", S_VAR_CODE ),  
  DEFINE_XFUNC1( "stopthread",	x_stopthread,	S_VAR_INT, "thread", S_VAR_CODE ),  
         
/* arrays */  
  DEFINE_XFUNC3( "range",   x_range,	S_VAR_LIST,	"from", S_VAR_INT | S_VAR_DOUBLE, "to", S_VAR_INT | S_VAR_DOUBLE, "step", S_VAR_INT | S_VAR_DOUBLE | S_VAR_PARAM_OPTIONAL ),
  DEFINE_XFUNC2( "push",    x_push,	0,		"array",  S_VAR_LIST,  "top",  S_VAR_ANY ),
  DEFINE_XFUNC1( "pop",	    x_pop,	S_VAR_ANY,	"array", S_VAR_LIST ),
  DEFINE_XFUNC2( "join",    x_join,	S_VAR_STRING,	"separator", S_VAR_STRING,  "array", S_VAR_LIST ),
  DEFINE_XFUNC1( "reverse", x_reverse,	0,		"array" , S_VAR_LIST | S_VAR_PARAM_OPTIONAL ),

/* higher order functions */
  DEFINE_XFUNC2( "map",	    x_map,	0,		"array", S_VAR_LIST | S_VAR_PARAM_OPTIONAL, "func",  S_VAR_CODE ),
  DEFINE_XFUNC2( "copymap", x_copymap,	S_VAR_LIST,	"array", S_VAR_LIST, "func", S_VAR_CODE ),
  DEFINE_XFUNC2( "filter",  x_filter,	S_VAR_LIST,	"array", S_VAR_LIST, "func",  S_VAR_CODE ),
  DEFINE_XFUNC2( "foldleft", x_foldl,	S_VAR_ANY,	"array", S_VAR_LIST, "func", S_VAR_CODE ),
  DEFINE_XFUNC2( "foldright",x_foldr,	S_VAR_ANY,	"array", S_VAR_LIST, "func", S_VAR_CODE ),

/* hashes */
  DEFINE_XFUNC1( "keys",   x_keys,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC1( "values", x_values,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC1( "each",   x_each,	S_VAR_LIST, "table", S_VAR_HASH ),
  DEFINE_XFUNC2( "exists", x_exists,	S_VAR_INT,  "table", S_VAR_HASH, "key", S_VAR_ANY ),
  DEFINE_XFUNC2( "exist",  x_exists,	S_VAR_INT,  "table", S_VAR_HASH, "key", S_VAR_ANY ),
  DEFINE_XFUNC2( "erase",  x_erase,	0,	    "table", S_VAR_HASH, "key", S_VAR_ANY ),
     
/* strings */
  DEFINE_XFUNC3( "mid",	   x_mid, S_VAR_STRING, "string", S_VAR_STRING, "offset", S_VAR_INT, "length", S_VAR_INT ),
  DEFINE_XFUNC2( "left",   x_left, S_VAR_STRING, "string", S_VAR_STRING, "length", S_VAR_INT ),
  DEFINE_XFUNC2( "right",  x_right, S_VAR_STRING, "string", S_VAR_STRING, "length", S_VAR_INT ),

  DEFINE_XFUNC1( "print",  x_print, S_VAR_STRING, "msg", S_VAR_STRING | S_VAR_INT | S_VAR_DOUBLE | S_VAR_LIST ),
  DEFINE_XFUNC1( "println",  x_println, S_VAR_STRING, "msg", S_VAR_STRING | S_VAR_INT | S_VAR_DOUBLE | S_VAR_LIST ),
    

/* numeric functions */
  DEFINE_XFUNC1( "srand",   x_srand, 0,  "seed", S_VAR_INT | S_VAR_DOUBLE ),
  DEFINE_XFUNC1( "rand",    x_rand, 0,  "max", S_VAR_INT | S_VAR_DOUBLE | S_VAR_PARAM_OPTIONAL  ),
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


/* eof */
  DEFINE_XFUNC1( 0, 0, 0, "", 0 )
};


