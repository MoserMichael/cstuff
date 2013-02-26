#ifndef __EVAL_H__
#define __EVAL_H__

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>

#include <cutils/array.h>
#include <cutils/sring.h>
#include <cutils/dring.h>
#include <cutils/dddlist.h>


#include <pooh-lan/ast.h>

/* ==================================================================================== */
#define RUNTIME_ERROR_MSG_LEN 1024

/* ==================================================================================== */
struct tagVALARRAY;
struct tagVALHASH; 
struct tagVALHASHPOS;
struct tagVALSTRING; 
struct tagVALFUNCTION_CAPTURE;
struct tagVALACTIVATION; 
union  tagBINDING_DATA; 
struct tagBINDING_DATA_MEM; 
struct tagEVAL_THREAD;
struct tagEVAL_CONTEXT;

/* ==================================================================================== */

// allocate memory block, if allocation fails then run garbage collection, if it fails aftrward then issue runtime error.
void *HEAP_alloc( size_t size );

// reallocate block; also do gc...
void HEAP_realloc( void **ptr, size_t size );

/* ==================================================================================== */

union tagBINDING_DATA *get_CONST_NULL();


/* ==================================================================================== */

// dynamic array value as part of runtime
typedef struct tagVALARRAY {
  union tagBINDING_DATA **data;
  size_t size;	    // number of entries (maximum index)
  size_t capacity; // size can grow up to capacity - without reallocating the array. 

} VALARRAY;

// initialise and empty array
M_INLINE void VALARRAY_init( VALARRAY *arr )
{
  arr->data = 0;
  arr->size = 0;
  arr->capacity = 0;
}

// reserve capacity - reserve room for bindings.
void VALARRAY_set_capacity( VALARRAY *arr, size_t capacity );

// copy array from into array to . capacity of from is not changes.
void VALARRAY_copy( VALARRAY *to, VALARRAY *from, CP_KIND deep_copy_values );

// free array and all the bindings that it contai s.
void VALARRAY_free( VALARRAY *arr );

// copy binding into array slot; here index is from 0
void VALARRAY_set( VALARRAY *arr, size_t idx, union tagBINDING_DATA *mem, CP_KIND copy_by_value);

union  tagBINDING_DATA  *VALARRAY_set_entry( VALARRAY *arr, size_t idx );

// get array slot; returns static null binding if index is out of bound; here index is from 0 
union tagBINDING_DATA *VALARRAY_get( VALARRAY *arr, size_t idx );

// get array slot; returns 0 if index is out of bound; here index is from 0 
union tagBINDING_DATA *VALARRAY_get_n( VALARRAY *arr, size_t idx );

// compare two arrays for equality; return -1 if 'less' ; 1 if 'greater' ; 0 if 'equal'
int VALARRAY_equal( VALARRAY *cmpa, VALARRAY *cmpb );



// return hash of array object ( used if array is a key value )
size_t VALARRAY_hash( VALARRAY *arr );

// returns number of elements in array
M_INLINE size_t VALARRAY_size( VALARRAY *arr )
{
  return arr->size;
}

// print an array
void VALARRAY_print( FILE *out, VALARRAY *data, int level );


/* ==================================================================================== */

#define HASH_INIT_SIZE 20


// hash value as part of runtime
typedef struct tagVALHASH {
  SRING *buckets; // array of buckets.
  size_t numbuckets; // number of buckets
  size_t size; // number of elements in hash
  union tagBINDING_DATA **ordered_keys; // if two hash tables are compared for equality (hash entry is part of a key ... )
} VALHASH;

// current position of iteration over hash value
typedef struct tagVALHASHPOS {
  SRING *bucket;
  SRING *bucket_pos;
} VALHASHPOS;

// initialise current position of iteration (must be calld before VALHASH_iterate)
M_INLINE void VALHASHPOS_init(VALHASHPOS *pos)
{
  pos->bucket = 0;
  pos->bucket_pos = 0;
}

// initialise an empty hash
M_INLINE void  VALHASH_init( VALHASH *hash )
{
  hash->buckets = 0;
  hash->numbuckets = 0;
  hash->size = 0;
  hash->ordered_keys = 0;
}

// reserve capacity - reserve room for bindings.
void VALHASH_set_capacity( VALHASH *hash, size_t capacity );

// copy hash from into hash to 
void VALHASH_copy( VALHASH *to, VALHASH *from,  CP_KIND deep_copy_values );

// free hash and all contained bindings (keys and values)
void VALHASH_free( VALHASH *hash );

// insert new key value pair. lookup of value by key is supposed to be fast. returns 1 if existing key updated; 2 if new key inserted.
int  VALHASH_set( VALHASH *hash, union tagBINDING_DATA *key, union tagBINDING_DATA *value , CP_KIND copy_by_value);

union tagBINDING_DATA * VALHASH_set_entry( VALHASH *hash, union tagBINDING_DATA *key );

// delete a key value pair identified by key. return 0 if entry has been deleted; -1 if no such entry.
int VALHASH_delete_key( VALHASH *hash, union tagBINDING_DATA *key );

// lookup of value by key; returns 0 if entry has been found; -1 if no such entry.
int VALHASH_find( VALHASH *hash, union tagBINDING_DATA *key, union tagBINDING_DATA **rval );

// iterate over hash; the position of iteration (pos) is passed as argument; key and value of current position is returned and position incremented; returns 0 on eof iteration
int VALHASH_iterate( VALHASH *hash, union tagBINDING_DATA **key, union tagBINDING_DATA **value, VALHASHPOS *pos );

// return hash of hash object ( if hash is a key value )
size_t VALHASH_hash( VALHASH *hash );

// returns number of elements in hash
M_INLINE size_t VALHASH_size( VALHASH *hash ) 
{
  return hash->size;
}

int VALHASH_equal( VALHASH *cmpa, VALHASH *cmpb );

// print a hash
void VALHASH_print( FILE *out, VALHASH *data, int level );

/* ==================================================================================== */
typedef struct tagVALSTRING {
  char   *string;
  size_t  length;
  size_t  capacity;
} VALSTRING;

M_INLINE void VALSTRING_init( VALSTRING *string )
{
  string->string = 0;
  string->length = 0;
  string->capacity = 0;
}

M_INLINE void VALSTRING_set_tmp_str( VALSTRING *string, char *str, size_t length )
{
  string->string = str;
  string->length = length;
  string->capacity = length + 1;
}

void VALSTRING_set_capacity( VALSTRING *string, size_t capacity );
void VALSTRING_set( VALSTRING *string, const char  *ptr, size_t length);
void VALSTRING_free( VALSTRING *string );
int  VALSTRING_copy( VALSTRING *to, VALSTRING *from );
int  VALSTRING_append( VALSTRING *to, VALSTRING *from );
int  VALSTRING_appends( VALSTRING *to, const char *from );
int  VALSTRING_find( VALSTRING *hay, VALSTRING *needle );
int  VALSTRING_substr( VALSTRING *to, VALSTRING *from, size_t offset, size_t length);
int  VALSTRING_cmp( VALSTRING *argA, VALSTRING *argB);
size_t VALSTRING_hash( VALSTRING *string );
void VALSTRING_print( FILE *out, VALSTRING *data );
void VALSTRING_make_null_terminated( VALSTRING *data );

M_INLINE size_t VALSTRING_size( VALSTRING *arg )
{
  return arg->length;
}

/* ==================================================================================== */

typedef struct tagVALFUNCTION_CAPTURE
{
  union {
    union tagBINDING_DATA *ref;    // if next.next ==0 then this is reference to heap location
    size_t stack_ref;		    // if next.next !=0 then this is reference to stack location
  } value;
  struct tagEVAL_THREAD *thread;   // points to thread that owns the stack where referenced item is on (for stack2stack)
//SRING next; 
  DDDLIST next;

} VALFUNCTION_CAPTURE;

M_INLINE int CAPTURE_IS_ON_HEAP( VALFUNCTION_CAPTURE *cap )
{
  return cap->next.next == 0;
}

typedef struct tagVALFUNCTION {
  
  union {
    union tagBINDING_DATA *this_environment;  // if function is member function of object - ptr to object data
    size_t this_offset;			      // if object is on stack - offset of stack object.
  }
    this_env;
  struct tagEVAL_THREAD *this_thread;	    // not null if ref to this is on the stack.
 
  struct tagAST_BASE *fdecl;		    // the function declaration
  struct tagEVAL_THREAD *thread;	    // attached co-routine.
  
  VALFUNCTION_CAPTURE *pcaptures;	    // non local variable references.
  union tagBINDING_DATA **pnested_closures; // nested closures.
  uint16_t ncaptures;			    // number of non local variable references
  uint16_t nclosures;			    // number of nested closures

} VALFUNCTION;

// init function object without captures.
M_INLINE void VALFUNCTION_init( VALFUNCTION *func )
{
  func->fdecl = 0;
  func->this_env.this_environment = 0;
  func->this_thread = 0;

#if 0
  ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), 0 );
  ARRAY_init( &func->nested_closures, sizeof( void * /*BINDING_DATA * */), 0 );
#endif
  func->pcaptures = 0;
  func->pnested_closures = 0;
  func->ncaptures = 0;
  func->nclosures = 0;
}

// init function object - must be known up front what number of captures are present in funcion.
void VALFUNCTION_init_cap( VALFUNCTION *fnc, size_t num_captures, size_t num_nested_closures );
void VALFUNCTION_init_outer_ref_tree( struct tagEVAL_THREAD *cthread, VALFUNCTION *fnc, AST_FUNC_DECL *fdecl, int nesting );
void VALFUNCTION_free( VALFUNCTION *fnc );

void VALFUNCTION_copy( VALFUNCTION *to, VALFUNCTION *from );
union tagBINDING_DATA *VALFUNCTION_this( VALFUNCTION *func );


// Make a capture ; if binding is on stack then forward reference is created.
void VALFUNCTION_make_capture( VALFUNCTION *func, union tagBINDING_DATA *data, int data_entry );

int  VALFUNCTION_equal( VALFUNCTION *cmpa, VALFUNCTION *cmpb);
void VALFUNCTION_print( FILE *out, VALFUNCTION *func );

typedef enum {
  VALFUNC_SHOW_METHOD_SIG,
  VALFUNC_HIDE_METHOD_SIG 
} 
  VALFUNC_PRINT_MODE;

void VALFUNCTION_print_mode( VALFUNC_PRINT_MODE show_method_sig );

#if 0
// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
void VALFUNCTION_prepare_call(VALFUNCTION *fcall )

// call via function object (if function object is marked as S_VAR_CODE_THREAD then new thread is dispatched)
void VALFUNCTION_call( VALFUNCTION *func );

// check if thread is active.
 struct tagEVAL_THREAD *VALFUNCTION_thread( VALFUNCTION *func )
#endif

#if 0
/* ==================================================================================== */
typedef struct tagERRORHANDLER {
  BINDING_DATA *trigger_condition;
  BINDING_DATA *trap_handler; 
} ERRORHANDLER;

int ERRORHANDLER_check_invoke( ERRORHANDLER *handler, const VALSTRING *error_code );
#endif

/* ==================================================================================== */

// the second slot in a function frame looks like this; chained entries form the call stack.

typedef struct tagVALACTIVATION {
  struct tagAST_BASE *fdecl; // how this function looks like in AST.
  size_t function_frame_start; // index of return value for this function call
  struct tagAST_BASE *ret_instr; // return address; after returning to function resume with this instruction.
  struct tagVALFUNCTION *function_object; // pointer to function object (if available). 
  size_t parent_function; 
} VALACTIVATION;

/* ==================================================================================== */

typedef enum {
  PARAM_BY_VAL, // pass param by value numbers are copied as is, by value, otherwise: copy on write for data value.
  PARAM_BY_REF, // pass param by reference

  COPY_EVAL_BY_VAL,  // assignment: evaluating right hand side  copy by value
  COPY_EVAL_BY_REF,  // assignment: evaluating right hand side, copy by reference.
  
  COPY_SINGLE_ASSIGN_BY_VAL,
  COPY_SINGLE_ASSIGN_BY_REF,
  COPY_MULTI_ASSIGN_BY_VAL,
  COPY_MULTI_ASSIGN_BY_REF,

}
  EVAL_REF_KIND;

/* ----------- */

#define S_VAR_REF_HEAP			0x01	// reference to heap value
#define S_VAR_REF_STACK2STACK		0x02	// reference on stack to stack value (offset to stack location relative to start of stack)
#define S_VAR_REF_HEAP2STACK		0x04	// reference on heap to stack value (treat like special case of capture)
#define S_VAR_REF_GLOB                  0x08    // reference to glob al
#define S_VAR_CAPTURE_REF		0x10    // refers to a captured value. (index into function objects' array of captured values)
#define S_VAR_OBJECT                	0x20    // collection that contains function object, the function object 
						// has a this pointer to containing collection.

#if 0
#define S_VAR_SELF_REF			0x40    // self referencial ; has elements that refer back to collection
#endif
#define REF_COPY_ON_WRITE		0x80    // copy on write - used for function parameters that are passed by value.
/* ----------- */

#define S_MASK_ENTRY			( S_VAR_HEAP_VALUE | S_VAR_GLOB_VALUE )

/* ----------- */

#define S_VAR_HEAP_VALUE		0x01	// heap entry.
#define S_VAR_GLOB_VALUE		0x02    // global entry.
#define S_VAR_HEAP_GC_MARK		0x10	// heap location has been marked.
#define S_VAR_HASH_HAS_SORTED_VALUES	0x20	// hash has sorted values
#define S_VAR_PRINT_CHECK_LOOPS		0x40

/* ----------- */

typedef struct tagBINDING_DATA_VALUE {
       short value_type;  
       unsigned char value_flags_ref;
       unsigned char value_flags_val;

       union {

	    union tagBINDING_DATA *value_ref; // reference to binding on heap.

	    //size_t	stack2stack_ref; // if reference between two stack locations, then reference is via offset from start of stack.
	    struct {
		size_t stack2stack_ref;		
		struct tagEVAL_THREAD *thread;
	    }
		stackref;	// if reference between two stack locations, then reference is via offset from start of stack.
	
	    VALFUNCTION_CAPTURE  heap2stack_ref; // if reference from heap allocated node to stack - then treat like capture.

	    POOH_INT	long_value;

	    double	double_value;

	    VALSTRING   string_value;
    
	    VALARRAY    array_value;

	    VALHASH     hash_value;

	    VALFUNCTION *func_value;

	} value;
    
        //void *backlink; //container;
	DDDLIST backlink;

} BINDING_DATA_VALUE;

typedef union tagBINDING_DATA {
    BINDING_DATA_VALUE b;	
    
    VALACTIVATION activation_record;

} BINDING_DATA;

// initialise stack value biding of given type.
void BINDING_DATA_init( BINDING_DATA *binding, AST_VAR_TYPE value_type);

// show value of a data binding; formatting details are in global context.
void BINDING_DATA_print( FILE *out, BINDING_DATA *data , int level );

// co show value of a data binding; formatting details are in global context.
void BINDING_DATA_prints( DBUF *out, BINDING_DATA *data , int level );

//copy binding value from to binding value to  - either by value or by reference (copy_by_value)
void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from , CP_KIND copy_by_value  );

void BINDING_DATA_copy_ext( BINDING_DATA *to, BINDING_DATA *from , EVAL_REF_KIND ty );

// move value around
void BINDING_DATA_move( BINDING_DATA *to, BINDING_DATA *from);

// return value from a function ret_slot - return value slot ; rvalue - value to return.
void BINDING_DATA_return_value( BINDING_DATA *ret_slot, BINDING_DATA *rvalue );

// freeing a binding.
void BINDING_DATA_free( BINDING_DATA *binding );

// internal comparison; imposes an ordering relation on data entries. 
int  BINDING_DATA_cmp( BINDING_DATA *cma, BINDING_DATA *cmpb );

// return hash value of binding
size_t BINDING_DATA_hash( BINDING_DATA *hash );

// follow the references! return value that it points to.
BINDING_DATA *BINDING_DATA_follow_ref( BINDING_DATA *binding );

// returns TRUE if binding is stack value.
M_INLINE int IS_STACK_VALUE( BINDING_DATA *binding )    
{
  return ( binding->b.value_flags_val & S_MASK_ENTRY )  == 0;
}

M_INLINE int IS_HEAP_VALUE( BINDING_DATA *binding )    
{
  return binding->b.value_flags_val & S_VAR_HEAP_VALUE;
}

M_INLINE int IS_GLOBAL_VALUE( BINDING_DATA *binding )
{
  return binding->b.value_flags_val & S_VAR_GLOB_VALUE ;
}

// returns TRUE if binding is a reference
M_INLINE int IS_REF( BINDING_DATA *binding )
{
    return (binding->b.value_flags_ref & ( S_VAR_REF_HEAP | S_VAR_REF_STACK2STACK | S_VAR_REF_HEAP2STACK | S_VAR_REF_GLOB | S_VAR_CAPTURE_REF )) != 0; 
}

// returns TRUE if binding is null value.
M_INLINE int IS_NULL( BINDING_DATA *binding )    
{
  return binding->b.value_type == S_VAR_NULL;
}

// returns TRUE if value is a number value.
M_INLINE int IS_NUMBER( BINDING_DATA *binding )    
{
  return binding->b.value_type & ( S_VAR_INT | S_VAR_DOUBLE );
}

M_INLINE int BINDING_DATA_get_int( BINDING_DATA *data, POOH_INT *val )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }

  if ((data->b.value_type & S_VAR_NULL) != 0) {
    *val = 0;
    return 0;
  }

  
  if ((data->b.value_type & (S_VAR_DOUBLE | S_VAR_INT)) == 0) {
    *val = 0;
    return -1;
  }
  if (data->b.value_type & S_VAR_DOUBLE) {
    *val = (POOH_INT) data->b.value.double_value;
    return 0;
  }
  *val = data->b.value.long_value;
  return 0;
}

M_INLINE void BINDING_DATA_set_int( BINDING_DATA *data, POOH_INT val )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if (data->b.value_type != S_VAR_INT ) {
    BINDING_DATA_init( data, S_VAR_INT );
  }
  data->b.value.long_value = val;
}

int BINDING_DATA_get_double( BINDING_DATA *data, double *val );

M_INLINE void BINDING_DATA_set_double( BINDING_DATA *data, double val )
{ 
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if (data->b.value_type != S_VAR_DOUBLE) {
    BINDING_DATA_init( data, S_VAR_DOUBLE );
  }
  data->b.value.double_value = val;
}

M_INLINE int BINDING_DATA_is_zero( BINDING_DATA *data )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  assert( data->b.value_type & (S_VAR_INT | S_VAR_DOUBLE ));
  if ( data->b.value_type & S_VAR_DOUBLE) {
    return data->b.value.double_value == 0;
  }
  return data->b.value.long_value == 0;
}


M_INLINE VALFUNCTION  *BINDING_DATA_get_fun( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & S_VAR_CODE) == 0) {
    return 0;
  }
  return data->b.value.func_value;
}


M_INLINE VALARRAY *BINDING_DATA_get_arr( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & S_VAR_LIST) == 0) {
    return 0;
  }
  return &data->b.value.array_value;
}

M_INLINE VALHASH *BINDING_DATA_get_hash( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & S_VAR_HASH) == 0) {
    return 0;
  }
  return &data->b.value.hash_value;
}

M_INLINE VALSTRING *BINDING_DATA_get_string( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & S_VAR_STRING) == 0) {
    return 0;
  }
  return &data->b.value.string_value;
}

M_INLINE void BINDING_DATA_set_string( BINDING_DATA *data, const char *svalue, size_t svalue_len )
{ 
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if (data->b.value_type != S_VAR_STRING) {
    BINDING_DATA_init( data, S_VAR_STRING );
  }
  VALSTRING_set( &data->b.value.string_value, svalue, svalue_len );
}

M_INLINE void BINDING_DATA_set_tmp_string( BINDING_DATA *data, const char *svalue, size_t svalue_size)
{
    VALSTRING *str;

    BINDING_DATA_init( data, S_VAR_STRING );
    str = &data->b.value.string_value;

    if (svalue_size == (size_t) -1) {
	svalue_size = strlen( svalue );
    }

    VALSTRING_set_tmp_str( str, (char *) svalue, svalue_size );  
}


/* ==================================================================================== */

typedef struct tagBINDING_DATA_MEM {
  DRING heap_entry; // list of all in memory values (for garbage collector to walk over 'all values')
  BINDING_DATA data;
} BINDING_DATA_MEM;


// allocate new binding on the garbage collected heap of given type.
BINDING_DATA *BINDING_DATA_MEM_new(  AST_VAR_TYPE value_type );

// free value from garbage collected heap; unlink from list of all objects; caller must check that there are no reference to this binding.
void BINDING_DATA_MEM_free( BINDING_DATA *pdata );

/* ==================================================================================== */

typedef struct tagDATA_REF
{
  BINDING_DATA *data;
  int is_stack;
  size_t idx;
  struct tagEVAL_THREAD *thread;
} DATA_REF;

void DATA_REF_set( DATA_REF *ref, BINDING_DATA *data );
BINDING_DATA *DATA_REF_get( DATA_REF *ref );

/* ==================================================================================== */

#if 0
typedef enum  {
  TSTATE_INIT,	    // initial state, thread created, yet running.
  TSTATE_RUNNING,   // thread is running,
  TSTATE_RUNNING_REQUEST_EXIT, // thread is running, has been resumed, so that thread can clean up and exit.
  
  TSTATE_WAITING,   // parent thread waits for child thread to yield/return.
  TSTATE_SUSPENDED, // thread has called yield, wait to be resumed.
  TSTATE_IDLE,	    // thread finished, not running.
} TSTATE;
#endif

typedef struct tagEVAL_THREAD {
  size_t  current_function_frame_start; 
  ARRAY binding_data_stack;	    // for evaluation of expressions and passing of function parameters and return values.

  struct tagAST_BASE *instr;	    // current instruction in current function

  struct tagEVAL_CONTEXT *context;
  size_t current_activation_record; // pointer to current activation record of current function. 
 
#if 0 
  struct tagEVAL_THREAD *parent;    // the co-routine that created this co-routine.  
  TSTATE tstate;		    // co-routine state.
#endif
  DRING threads;		    // co-routines started by this co-routine instance.

  BINDING_DATA *yield_rvalue;	    // co-routine yield value - passed from thread to caller
  BINDING_DATA *resume_value;       // co-routine resume value - passsed from caller to thread.

  int  loop_collection_cnt;	    // while checking for loops during printing of container: counter of nested collections. 
  HASH  print_check_ref_loops;	    // set for checking loops during printing of container.
  size_t has_loops;		    // if currently printed structure has been found to contain loops.

  void  *eval_impl;		    // evaluator's implementation data (opaque)

} EVAL_THREAD;

// create a new thread.
int EVAL_THREAD_init(EVAL_THREAD *thread, EVAL_THREAD *parent, struct tagEVAL_CONTEXT *context );

// free a thread
void EVAL_THREAD_free(EVAL_THREAD *thread);


// get the binding that is at the top of the stack.
M_INLINE BINDING_DATA *EVAL_THREAD_get_stack_top( EVAL_THREAD *thread )
{
   BINDING_DATA *data;
   size_t nsize;

   nsize = ARRAY_size( &thread->binding_data_stack ); 
   data = (BINDING_DATA *) ARRAY_at( &thread->binding_data_stack, nsize - 1 );
   assert( data != 0);
   return data;
}


M_INLINE size_t EVAL_THREAD_get_stack_top_pos( EVAL_THREAD *thread ) {
  return  ARRAY_size( & thread->binding_data_stack ) - 1;

}


BINDING_DATA *EVAL_THREAD_pop_stack( EVAL_THREAD *thread );

M_INLINE void EVAL_THREAD_discard_pop_stack( EVAL_THREAD *thread )
{
  BINDING_DATA *r;
  
  r = EVAL_THREAD_pop_stack( thread );
  BINDING_DATA_free( r );
}


BINDING_DATA *EVAL_THREAD_push_stack( EVAL_THREAD *thread , AST_VAR_TYPE value_type );

void EVAL_THREAD_push_stack2( EVAL_THREAD *thread, BINDING_DATA *entry);

M_INLINE BINDING_DATA *EVAL_THREAD_stack_offset( EVAL_THREAD *thread, size_t pos )
{
   if (pos >= ARRAY_size( &thread->binding_data_stack )) {
      assert(0);
   }
   BINDING_DATA *data = (BINDING_DATA *) ARRAY_at( &thread->binding_data_stack, pos ); //ARRAY_size( &thread->binding_data_stack ) - 1 - stack_offset );
   assert( data != 0);
   return data;
}



// get binding indexed from the start of the current frame onward. (stack_offset == 0 - return address. )
BINDING_DATA *EVAL_THREAD_stack_frame_offset( EVAL_THREAD *thread, size_t stack_offset );

// sets binding inex from start of current frame onward.
void EVAL_THREAD_set_stack_frame_offset( EVAL_THREAD *thread, size_t stack_offset, BINDING_DATA *data );


// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
size_t EVAL_THREAD_prepare_xcall( EVAL_THREAD *thread, AST_XFUNC_DECL *decl );

// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
size_t EVAL_THREAD_prepare_call( EVAL_THREAD *thread, AST_FUNC_DECL *decl );


size_t EVAL_THREAD_prepare_func_call( EVAL_THREAD *thread, VALFUNCTION *func  );

BINDING_DATA * EVAL_THREAD_proceed_func_call( EVAL_THREAD *thread, size_t frame_start, VALFUNCTION *func  );


// prepare co-routine call: create new stack and push ref to call parameters.
EVAL_THREAD * EVAL_THREAD_prepare_coroutine( EVAL_THREAD *thread, size_t frame_start, VALFUNCTION *func, AST_BASE *fdcl);

BINDING_DATA *EVAL_THREAD_parameter( EVAL_THREAD *thread, size_t frame_start, size_t param_num, AST_VAR_TYPE type );
BINDING_DATA *EVAL_THREAD_parameter_ref( EVAL_THREAD *thread, size_t frame_start, size_t param_num );

// precondition: call prepared and arguments have been pushed to the stack, make function call.
void EVAL_THREAD_call_xfunc( EVAL_THREAD *thread, size_t frame_start, AST_XFUNC_DECL *decl, VALFUNCTION *function );

// make a new frame
// precondition: call prepared and arguments have been pushed to the stack. 
// postcondition: local variable are set to NULL, activation record on the stack.
void EVAL_THREAD_call_func( EVAL_THREAD *thread, size_t frame_start, AST_FUNC_DECL *decl, VALFUNCTION *function );

// finish function call, return to calling frame. returns -1 if top of stack reached.
int EVAL_THREAD_pop_frame ( EVAL_THREAD *thread );

// set value as return value of current stack trace.
int EVAL_THREAD_return_value( EVAL_THREAD *thread, BINDING_DATA *data );

// print stack backtrace.
int EVAL_THREAD_print_stack_trace( FILE *out, EVAL_THREAD *thread );

// returns 1 if this function is called as a co-routine.
int EVAL_THREAD_is_threadmain( EVAL_THREAD *thread );

void EVAL_THREAD_set_current_thread( EVAL_THREAD *thread );
EVAL_THREAD * EVAL_THREAD_get_current_thread();


void EVAL_THREAD_copy_binding( BINDING_DATA *to, BINDING_DATA *from );

/* ==================================================================================== */

union tagBINDING_DATA;
struct tagEVAL_THREAD;

typedef struct tagXCALL_DATA {
  struct tagEVAL_THREAD *thread;
  size_t frame_offset;
  size_t num_arguments;

} XCALL_DATA;

M_INLINE BINDING_DATA *XCALL_rvalue( XCALL_DATA *xcall )
{
   return ((BINDING_DATA *) xcall->thread->binding_data_stack.buffer) +  xcall->frame_offset;
}

BINDING_DATA *XCALL_this( XCALL_DATA *xcall );


M_INLINE BINDING_DATA *XCALL_param( XCALL_DATA *xcall, size_t num_param )
{
   if (num_param >= xcall->num_arguments) {
     return 0;
   }
   return ((BINDING_DATA *) xcall->thread->binding_data_stack.buffer) +  xcall->frame_offset + num_param + 2;
}  

M_INLINE size_t XCALL_nparams( XCALL_DATA *xcall)
{
  return xcall->num_arguments;
}

/* ==================================================================================== */

/** callback function set by evaluator - for management of co-routines ***/
typedef int (*THREAD_YIELD_CB)     ( BINDING_DATA *yield_value, BINDING_DATA **resume_msg, void *eval_ctx );
typedef int (*THREAD_RESUME_CB)    ( BINDING_DATA *function, BINDING_DATA *resume_msg, BINDING_DATA **yield_value, void *eval_ctx );
typedef int (*THREAD_EXIT_CB)      ( void *eval_ctx );
typedef int (*THREAD_KILL_CB)	    ( BINDING_DATA *function, void *eval_ctx );

/** callback function set by evaluator - invocation of interpreter function ***/

typedef void (*INVOKE_FUNCTION_CB)  ( size_t frame_start, AST_BASE *fdecl, VALFUNCTION *valfunc, void *eval_ctx  );

typedef struct tagEVAL_CONTEXT {
  DRING gc_heap;		      // list of all garbage collectd heap entries
  DRING threads;		      // list of all threads (interpreter threads)
  EVAL_THREAD main;		      // the main thread
  EVAL_THREAD *current_thread;	      // current thread
  ARRAY bindings_global;	      // all global bindings (indexed array)
  int trace_on;

  jmp_buf jmpbuf;		      // runtime errors - if runtime error it bails out to mark.
  int  is_jmpbuf_set;

  SHOW_SOURCE_LINE show_source_line;  // callback function - print current location in program
  void *show_source_line_ctx;	      // context data of callback.

  THREAD_YIELD_CB   thread_yield_cb;  // co-routines: yield value to calling thread
  THREAD_RESUME_CB  thread_resume_cb; // co-routines: resume suspended thread
  THREAD_EXIT_CB    thread_exit_cb;   // co-routines: exit current thread
  THREAD_KILL_CB    thread_kill_cb;   // co-routines: kill a given thread

  INVOKE_FUNCTION_CB invoke_function_cb; // callback: call interpreter to run a function of the script.
  void *thread_ctx;

  BINDING_DATA *env,*argv;	      // global values of environment and cmdline arguments, used to always mark these values, even if they are not used right now.
  
  struct tagCTHREAD *event_loop;      // event loop for socket handling. (cooperative thread);

} EVAL_CONTEXT;

M_INLINE int EVAL_CONTEXT_is_main_scope( EVAL_CONTEXT *context )
{ 
  return context->current_thread == &context->main && context->current_thread->current_function_frame_start == 2 ;
}


int  EVAL_CONTEXT_init( EVAL_CONTEXT *context, size_t num_globals );
void EVAL_CONTEXT_free( EVAL_CONTEXT *context );
void EVAL_CONTEXT_start( EVAL_CONTEXT *context , char *argv[], int argc );
void EVAL_CONTEXT_gc( EVAL_CONTEXT *context );
void EVAL_CONTEXT_runtime_error( EVAL_CONTEXT *context, const char *format, ... );
void EVAL_CONTEXT_print_stack_trace_all( FILE *out, EVAL_CONTEXT *context );



AST_XFUNC_DECL *get_each_in_array_xfunc();

#endif



