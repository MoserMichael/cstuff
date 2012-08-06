#include <pooh-lan/rtlib/rtlib.h>
#include <vtest/vtest.h>
#include <setjmp.h>

#define START_INVOKE_RTLIB( ctx, error_action ) \
   if (setjmp(  ((ctx)->jmpbuf) ) ) { \
     (error_action) ( ctx ); \
      return ; \
   } else { \
    (ctx)->is_jmpbuf_set = 1; \
   } \
   EVAL_CONTEXT_start( ctx ); \
 

size_t count_heap_objects( EVAL_CONTEXT * ctx )
{
  size_t rt = 0;
  DRING *cur;

  DRING_FOREACH( cur, &ctx->gc_heap )
  {
    rt += 1;
  }

  return rt;
}

void set_binding_array( ARRAY *arr, size_t nsize )
{
    size_t i;

    if (ARRAY_resize( arr, nsize )) {
	VASSERT(0);
    }

    for(i = 0; i< nsize; i++) {
     //BINDING_DATA_init( (BINDING_DATA *) ARRAY_at( array, i ), S_VAR_NULL );
     BINDING_DATA_init( ((BINDING_DATA *) arr->buffer ) + i, S_VAR_NULL ); 
    }
    arr->elmcount = nsize;

}

void init_test_thread( EVAL_THREAD *thread, size_t nsize )
{
  set_binding_array( & thread->binding_data_stack, nsize ); 
 
  // even if no function is runing - do as if the stack starts from the top.
  thread->current_function_frame_start = 0;
}

void test_error_action( EVAL_CONTEXT *ctx )
{
  (void) ctx;

  VASSERT( 0 );
}

extern AST_XFUNC_DECL xlib[];

long sums( long from , long to)
{
   return (from + to) * (to - from + 1) / 2; 
}

void BASIC_hash_test()
{
  EVAL_CONTEXT context;
  BINDING_DATA *hash,*key,*value, *values, *arg;
  VALHASH *phash;
  VALARRAY *arr;
  size_t frame_start;
  long sum;
  size_t i;

  VASSERT( EVAL_CONTEXT_init( &context, 0 ) == 0 );

  START_INVOKE_RTLIB( &context, test_error_action );

  // *** make a hash (force resize of hash)

  hash = BINDING_DATA_MEM_new( S_VAR_HASH );
  
  for( i=0; i < (2*HASH_INIT_SIZE); i++ ) {

    key = BINDING_DATA_MEM_new( S_VAR_INT );
    value = BINDING_DATA_MEM_new( S_VAR_INT );
    
    BINDING_DATA_set_int( key, i + 1 );
    BINDING_DATA_set_int( value, i + 1 );
    
    phash = BINDING_DATA_get_hash( hash );
    VALHASH_set( phash, key, value, CP_REF );
  }

  VASSERT( VALHASH_size( phash ) == (2 * HASH_INIT_SIZE) );
  VASSERT( phash->numbuckets == 64 );
  
  // *** extract keys of hash (call library function that does that.

  frame_start = EVAL_THREAD_prepare_xcall( &context.main, &xlib[ 0 ] );
  
  arg =	EVAL_THREAD_parameter( &context.main, frame_start, 0, S_VAR_NULL );
  BINDING_DATA_copy( arg, hash, CP_REF );
  
  EVAL_THREAD_call_xfunc( &context.main, frame_start, &xlib[ 0 ] ); // don't breathe - indexes in xlib can cagne.
  values = EVAL_THREAD_get_stack_top( &context.main );
 
  // ****  compute sum of keys; must add up.

  arr = BINDING_DATA_get_arr( values );

  VASSERT( arr != 0 );
  VASSERT( VALARRAY_size( arr ) == (2 * HASH_INIT_SIZE) );

  for(i = 0, sum = 0 ; i < VALARRAY_size( arr ); i++) {
    key = VALARRAY_get( arr, i );
    VASSERT( key->b.value_type == S_VAR_INT );

    sum += key->b.value.long_value;
  }
  VASSERT( sum == sums(1, 2 * HASH_INIT_SIZE ) );
  
  BINDING_DATA_free( hash );
  EVAL_CONTEXT_free( &context );
}


void BASIC_gc_test()
{
  EVAL_CONTEXT context;
  EVAL_THREAD  thread;
  BINDING_DATA *pa1, *pa2, tmp_a;
  size_t i;
  size_t before_gc, after_gc;
 
  VASSERT( EVAL_CONTEXT_init( &context, 0 ) == 0 );

  VASSERT( EVAL_THREAD_init( &thread, &context ) == 0 );

  START_INVOKE_RTLIB( &context, test_error_action );

  //allocate some referenced objects.
  init_test_thread( & context.main, 10 );


  //set up a reference cycle.
  pa1 = BINDING_DATA_MEM_new( S_VAR_LIST );
  pa2 = BINDING_DATA_MEM_new( S_VAR_HASH );
  VALARRAY_set( &pa1->b.value.array_value, 0, pa2, CP_REF );
  BINDING_DATA_init( &tmp_a, S_VAR_INT );
  tmp_a.b.value.long_value = 10;
  VALHASH_set( &pa2->b.value.hash_value, &tmp_a, pa1, 0 );

  // stack slot point to cycle start.
  BINDING_DATA_copy( EVAL_THREAD_stack_frame_offset( &context.main, 0 ) , pa1, CP_REF );

  // do non garbage array
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main, 1 );
  BINDING_DATA_init( pa1, S_VAR_LIST );

  for( i = 0; i < 42; i++ ) {
     pa2 = BINDING_DATA_MEM_new( S_VAR_INT );
     VALARRAY_set( &pa1->b.value.array_value, i, pa2, CP_REF );
  }

  // assign unreferenced garbage - will be cleaned by collection.
  for( i = 0; i< 42; i++ ) {
     BINDING_DATA_MEM_new( S_VAR_INT );
  }

  VASSERT( DRING_check( &context.gc_heap ) );
  before_gc = count_heap_objects( &context );

  //perform garbage collection cycle
  EVAL_CONTEXT_gc( &context );

  after_gc = count_heap_objects( &context );
 
fprintf(stderr,"\nbefore %d after %d\n", before_gc, after_gc);

  //see how many object are left on the heap.
  VASSERT( after_gc  == (before_gc - 42 ));

  EVAL_CONTEXT_free( &context );
}

void PRINT_VALUE_test()
{
  EVAL_CONTEXT context;
  EVAL_THREAD  thread;
  BINDING_DATA *pa1, *pa2, *pa3, tmp_a, tmp_b, tmp_c;
  size_t i, j, next_stack = 0;
  char stmp[ 30 ];
  VASSERT( EVAL_CONTEXT_init( &context, 0 ) == 0 );

  VASSERT( EVAL_THREAD_init( &thread, &context ) == 0 );

  START_INVOKE_RTLIB( &context, test_error_action );

  //allocate some referenced objects.
  init_test_thread( & context.main, 15 );
 
/*---*/

  // short integer array sequence (not collapsed)
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );

  for( i = 0; i < 3; i++ ) {
     pa2 = BINDING_DATA_MEM_new( S_VAR_INT );
     pa2->b.value.long_value = i;
     VALARRAY_set( &pa1->b.value.array_value, i, pa2, CP_REF );
  }
  fprintf(stderr, "\n\nArray - reference to heap entries\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  // short integer array sequence (not collapsed)
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );

  for( i = 0; i < 3; i++ ) {
     pa2 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
     BINDING_DATA_init( pa2, S_VAR_INT );
     pa2->b.value.long_value = i;
     VALARRAY_set( &pa1->b.value.array_value, i, pa2, CP_REF );
  }
  fprintf(stderr, "\n\nArray - reference to stack entries\n");
  BINDING_DATA_print( stderr, pa1, 0 );


/*---*/
 
  // integer aray sequence (collapsed)
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );

  pa2 = BINDING_DATA_MEM_new( S_VAR_INT );
  pa2->b.value.long_value = -10;
  VALARRAY_set( &pa1->b.value.array_value, 3, pa2, CP_VALUE );
  
  for( i = 1; i < 42; i++ ) {
     pa2 = BINDING_DATA_MEM_new( S_VAR_INT );
     pa2->b.value.long_value = i;
     VALARRAY_set( &pa1->b.value.array_value, i, pa2, CP_VALUE );
  }
  BINDING_DATA_init( &tmp_c, S_VAR_INT );
  tmp_c.b.value.long_value = 100;
  VALARRAY_set( &pa1->b.value.array_value, i++, &tmp_c, CP_VALUE );
 
  BINDING_DATA_init( &tmp_c, S_VAR_INT );
  tmp_c.b.value.long_value = 200;
  VALARRAY_set( &pa1->b.value.array_value, i++, &tmp_c, CP_VALUE );

  fprintf(stderr,"\n\nArray - with integer sequence\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  // make a hash with arrays in int
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_HASH );

  for(i = 0; i< 24; i++)  {
    BINDING_DATA_init( &tmp_a, S_VAR_STRING );

    sprintf( stmp,"key_%d", i );
    VALSTRING_set( &tmp_a.b.value.string_value, stmp, strlen(stmp) );

    BINDING_DATA_init( &tmp_b, S_VAR_LIST );
    for( j = 0; j < 3 ; j++ ) {
       BINDING_DATA_init( &tmp_c, S_VAR_INT );
       tmp_c.b.value.long_value = i + j;
       VALARRAY_set( &tmp_b.b.value.array_value, j, &tmp_c, CP_VALUE);
    }
    VALHASH_set( &pa1->b.value.hash_value, &tmp_a, &tmp_b, CP_VALUE);
  }
  
  fprintf(stderr,"\n\nHash - map string to array\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  //make circular structure
  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++  );
  BINDING_DATA_init( pa1, S_VAR_LIST );
  
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j;
    VALARRAY_set( &pa1->b.value.array_value, j, &tmp_c, CP_VALUE);
  }
 
  pa2 = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j+1;
    VALARRAY_set( &pa2->b.value.array_value, j, &tmp_c, CP_VALUE);
  }

  // set back reference from second array to first one.
  VALARRAY_set( &pa2->b.value.array_value, j, pa1, CP_REF);

  // copy second array to first one.
  VALARRAY_set( &pa1->b.value.array_value, j, pa2, CP_VALUE);

  fprintf(stderr,"\n\nNested arrays, refers back to itself (circular reference)\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );
 
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j;
    VALARRAY_set( &pa1->b.value.array_value, j, &tmp_c, CP_VALUE);
  }
 
  pa2 = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j+1;
    VALARRAY_set( &pa2->b.value.array_value, j, &tmp_c, CP_VALUE);
  }

  // copy second array to first one.
  VALARRAY_set( &pa1->b.value.array_value, j+1, pa2, CP_VALUE );

  pa2 = VALARRAY_get( &pa1->b.value.array_value, j+1);
  pa3 = VALARRAY_get( &pa2->b.value.array_value, 2); 
  // set forward reference (from first to middle of the second array)
  VALARRAY_set( &pa1->b.value.array_value, j,  pa3, CP_REF );


  fprintf(stderr,"\n\nNested arrays, refers back to itself (forward reference)\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  pa1 = EVAL_THREAD_stack_frame_offset( &context.main,  next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );
 
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j;
    VALARRAY_set( &pa1->b.value.array_value, j, &tmp_c, CP_VALUE);
  }
 
  pa2 = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j+1;
    VALARRAY_set( &pa2->b.value.array_value, j, &tmp_c, CP_VALUE);
  }

  // copy second array to first one.
  //VALARRAY_set( &pa1->b.value.array_value, j+1, pa2, CP_VALUE );

  //pa2 = VALARRAY_get( &pa1->b.value.array_value, j+1);
  pa3 = VALARRAY_get( &pa2->b.value.array_value, 2); 
  // set forward reference (from first to middle of the second array)
  VALARRAY_set( &pa1->b.value.array_value, j,  pa3, CP_REF );


  fprintf(stderr,"\n\nReference into another array\n");
  BINDING_DATA_print( stderr, pa1, 0 );

/*---*/

  pa1 = EVAL_THREAD_stack_frame_offset( &context.main, next_stack++ );
  BINDING_DATA_init( pa1, S_VAR_LIST );
 
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j;
    VALARRAY_set( &pa1->b.value.array_value, j, &tmp_c, CP_VALUE );
  }
  pa2 = BINDING_DATA_MEM_new( S_VAR_LIST );
  
  for( j = 0; j < 3 ; j++ ) {
    BINDING_DATA_init( &tmp_c, S_VAR_INT );
    tmp_c.b.value.long_value = j+1;
    VALARRAY_set( &pa2->b.value.array_value, j, &tmp_c, CP_VALUE );
  }

  // two references from first to second array.
  VALARRAY_set( &pa1->b.value.array_value, j, pa2, CP_REF );
  VALARRAY_set( &pa1->b.value.array_value, j+1, pa2, CP_REF );

  fprintf(stderr,"\n\nNested arrays, refers back to itself (forward reference)\n");
  BINDING_DATA_print( stderr, pa1, 0 );


  fprintf(stderr,"\n");
  EVAL_CONTEXT_free( &context );
}

void x_add ( XCALL_DATA *xcall )
{
   double val1,val2;


   VASSERT( XCALL_nparams( xcall )  == 2 );
   VASSERT( IS_NUMBER( XCALL_param(xcall, 0 ) ) );
   VASSERT( IS_NUMBER( XCALL_param(xcall, 1 ) ) );


   VASSERT( ! BINDING_DATA_get_double( XCALL_param(xcall, 0 ), &val1 ) );
   VASSERT( ! BINDING_DATA_get_double( XCALL_param(xcall, 1 ), &val2 ) );

   BINDING_DATA_set_double( XCALL_rvalue(xcall), val1 + val2 );
   
}

void x_fact( XCALL_DATA *xcall );

AST_XFUNC_DECL x_fact_decl = DEFINE_XFUNC1( "x_fact", x_fact, S_VAR_INT, "num",  S_VAR_INT|S_VAR_DOUBLE);

void x_fact( XCALL_DATA *xcall )
{
  long narg, rnval;
  BINDING_DATA *arg,*rval;
  size_t frame_start;

  VASSERT(  XCALL_nparams( xcall ) == 1 );
  VASSERT( ! BINDING_DATA_get_int( XCALL_param(xcall, 0 ), &narg ) );
  
  if (narg == 0) {
    EVAL_THREAD_print_stack_trace( stderr, xcall->thread );
    BINDING_DATA_set_int( XCALL_rvalue(xcall), 1);
    return;
  }

  frame_start = EVAL_THREAD_prepare_xcall( xcall->thread, &x_fact_decl );
 
  arg =	EVAL_THREAD_parameter( xcall->thread, frame_start, 0, S_VAR_INT );
  BINDING_DATA_set_int( arg, narg - 1);
  
  EVAL_THREAD_call_xfunc( xcall->thread, frame_start, &x_fact_decl );
 
  rval = EVAL_THREAD_get_stack_top( xcall->thread );
  VASSERT( ! BINDING_DATA_get_int(rval , &rnval ) );
  BINDING_DATA_set_int( XCALL_rvalue(xcall), rnval * narg );


}

void XCALL_test()
{
  EVAL_CONTEXT context;
  EVAL_THREAD  *thread;
  BINDING_DATA *arg,*rval;
  size_t frame_start;
  double rdval;
  long rnval;
  AST_XFUNC_DECL x_add_decl = DEFINE_XFUNC2( "x_add", x_add, S_VAR_DOUBLE, "a", S_VAR_INT|S_VAR_DOUBLE , "b", S_VAR_INT|S_VAR_DOUBLE );
  

  VASSERT( EVAL_CONTEXT_init( &context, 0 ) == 0 );
  thread = &context.main;

  START_INVOKE_RTLIB( &context, test_error_action );

  // call x_add
  frame_start = EVAL_THREAD_prepare_xcall( thread, &x_add_decl );
  
  arg =	EVAL_THREAD_parameter( thread, frame_start, 0, S_VAR_INT );
  BINDING_DATA_set_int( arg, 2 );
  
  arg = EVAL_THREAD_parameter( thread, frame_start, 1, S_VAR_DOUBLE );
  BINDING_DATA_set_double( arg, 2);
  
  EVAL_THREAD_call_xfunc( thread, frame_start, &x_add_decl );
  
  rval = EVAL_THREAD_get_stack_top( thread );
  VASSERT( ! BINDING_DATA_get_double( rval, &rdval ) );
  VASSERT( rdval == 4 );


  // call x_fact
  frame_start = EVAL_THREAD_prepare_xcall( thread, &x_fact_decl );
  
  arg =	EVAL_THREAD_parameter( thread, frame_start, 0, S_VAR_INT );
  BINDING_DATA_set_int( arg, 10 );
  
  EVAL_THREAD_call_xfunc( thread, frame_start, &x_fact_decl );
   
  rval = EVAL_THREAD_get_stack_top( thread );
  VASSERT( ! BINDING_DATA_get_int( rval, &rnval ) );
  VASSERT( rnval == 3628800 );
   
  EVAL_CONTEXT_free( &context );
}

#if 0
void x_fib( XCALL_DATA *xcall );


AST_XFUNC_DECL x_fib_decl = DEFINE_XFUNC2( "x_fact", x_fib, S_VAR_INT, "from", S_VAR_INT|S_VAR_DOUBLE, "to", S_VAR_INT|S_VAR_DOUBLE);


void x_fib( XCALL_DATA *xcall )
{
   long fromval, toval, n1, n2, i, next;
   BINDING_DATA tmp;

   BINDING_DATA_get_int( XCALL_param( xcall, 0 ) , &fromval );
   BINDING_DATA_get_int( XCALL_param( xcall, 0 ) , &toval );
  
   
   for( i = 1, n1 = 1, n2 = 1 ; i < toval; i++ )
   {
     next = n1 + n2;
     n2 = n1;
     n1 = next;

     if ( i >= fromval) {
        BINDING_DATA_set_int( &tmp, next );
	EVAL_THREAD_yield_value( xcall->thread, &tmp, 0 );
     }	    
   }
}


void THREAD_test()
{
  EVAL_CONTEXT context;
  EVAL_THREAD  *thread, *th;
  BINDING_DATA  tcall,*rval;
  VALFUNCTION *fobj;
  long rnval;

  VASSERT( EVAL_CONTEXT_init( &context ) == 0 );
  thread = &context.main;

  START_INVOKE_RTLIB( &context, test_error_action );

  // prepare function object that represents the thread.
  BINDING_DATA_init( &tcall, S_VAR_CODE );

  tcall.b.value_type |= S_VAR_CODE_THREAD;  // mark as thread.
  fobj = &tcall->b.value.func_value;
  VALFUNCTION_mark_as_thread( fobj );

  // prepare call.
  fobj->fdecl = &x_fib_decl;
  VALFUNCTION_prepare_call( fobj );

  // put argument for call.
  arg = EVAL_THREAD_stack_alloc( thread, S_VAR_INT );
  BINDING_DATA_set_int( arg, 5 );
  arg = EVAL_THREAD_stack_alloc( thread, S_VAR_INT );
  BINDING_DATA_set_int( arg, 10 );
    
  // dispatch call.
  VALFUNCTION_call( fobj );
 
  // return value of thread call.
  rval = EVAL_THREAD_get_stack_top( thread );
 
  // while coroutine thread is running...
  while( (th = VALFUNCTION_thread( fobj )) != 0 ) {

     //resume coroutine thread
     EVAL_THREAD_activate( th, 0, &rval );

     // check return value.
     VASSERT( th != 0 );
     VASSERT( ! BINDING_DATA_get_int( rval, &rnval ) )
  }
 
  EVAL_CONTEXT_free( &context );
}
#endif
