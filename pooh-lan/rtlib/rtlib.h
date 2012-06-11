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

#include <pooh-lan/ast.h>

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
typedef enum {
  CP_REF,
  CP_VALUE,
} CP_KIND;


/* ==================================================================================== */

// dynamic array value as part of runtime
typedef struct tagVALARRAY {
  union tagBINDING_DATA *data; // includes back pointer to collection, so that reference to element results in reference to the collection.
  size_t size; // number of non null        
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

// get array slot; returns 0 if index is out of bound; here index is from 0 
union tagBINDING_DATA *VALARRAY_get( VALARRAY *arr, size_t idx );


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

void VALSTRING_set_capacity( VALSTRING *string, size_t capacity );
void VALSTRING_set( VALSTRING *string, const char  *ptr, size_t length);
void VALSTRING_free( VALSTRING *string );
int  VALSTRING_copy( VALSTRING *to, VALSTRING *from );
int  VALSTRING_append( VALSTRING *to, VALSTRING *from );
int  VALSTRING_find( VALSTRING *hay, VALSTRING *needle );
int  VALSTRING_substr( VALSTRING *to, VALSTRING *from, size_t offset, size_t length);
int  VALSTRING_cmp( VALSTRING *argA, VALSTRING *argB);
size_t VALSTRING_hash( VALSTRING *string );
void VALSTRING_print( FILE *out, VALSTRING *data );

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
    struct tagEVAL_THREAD *thread;
  } value;
  SRING next;

} VALFUNCTION_CAPTURE;

M_INLINE int CAPTURE_IS_ON_HEAP( VALFUNCTION_CAPTURE *cap )
{
  return cap->next.next == 0;
}

typedef struct tagVALFUNCTION {
  union tagBINDING_DATA *this_environment;
  struct tagAST_BASE *fdecl;
  ARRAY  captures; 
} VALFUNCTION;

// init function object without captures.
M_INLINE void VALFUNCTION_init( VALFUNCTION *func )
{
  func->fdecl = 0;
  func->this_environment = 0;

  ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), 0 );
}

// init function object - must be known up front what number of captures are present in funcion.
void VALFUNCTION_init_cap( VALFUNCTION *func, size_t num_captures );

// Make a capture ; if binding is on stack then forward reference is created.
void VALFUNCTION_make_capture( VALFUNCTION *func, union tagBINDING_DATA *data, int data_entry );

int  VALFUNCTION_equal( VALFUNCTION *cmpa, VALFUNCTION *cmpb);
void VALFUNCTION_print( FILE *out, VALFUNCTION *func );


#if 0
// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
void VALFUNCTION_prepare_call(VALFUNCTION *fcall )

// call via function object (if function object is marked as S_VAR_CODE_THREAD then new thread is dispatched)
void VALFUNCTION_call( VALFUNCTION *func );

// call via this object will be executed in a new thread.
int VALFUNCTION_mark_as_thread( VALFUNCTION *func );

// check if thread is active.
 struct tagEVAL_THREAD *VALFUNCTION_thread( VALFUNCTION *func )
#endif

/* ==================================================================================== */
typedef struct tagVALACTIVATION {
   struct tagAST_BASE *fdecl; // how this function looks like in AST.
   size_t function_frame_start; // index of return value for this thread.
   struct tagAST_BASE *ret_instr; // return address; after returning to function resume with this instruction.
   struct tagVALFUNCTION *function_object; // pointer to function object (if available). 
   struct tagVALACTIVATION *parent_function; // - this value is overlayed with container field in BINDING_DATA structure.
} VALACTIVATION;

/* ==================================================================================== */


#define S_VAR_REF_HEAP			0x01	// reference to heap value
#define S_VAR_REF_STACK2STACK		0x02	// reference on stack to stack value (offset to stack location relative to start of stack)
#define S_VAR_REF_HEAP2STACK		0x04	// reference on heap to stack value (treat like special case of capture)
#define S_VAR_CAPTURE_REF		0x08    // refers to a captured value. (index into function objects' array of captured values)

#define S_VAR_COLL_ENTRY	        0x10    // collection entry.
#define S_VAR_HEAP_VALUE		0x20	// heap entry.

#define S_VAR_ARG_COW_REF		0x40    // function argument - copy on write ref; new copy of argument will be created when it is modified.
						// used with S_VAR_REF_STACK2STACK
		
#define S_VAR_HEAP_GC_MARK		0x100	// heap location has been marked.
#define S_VAR_HASH_HAS_SORTED_VALUES	0x200	// hash has sorted values
#define S_VAR_PRINT_CHECK_LOOPS		0x400

// if any of these bits are set then it is not a stack entry.
#define S_MASK_NSTACK			0x3F


typedef struct tagBINDING_DATA_VALUE {
       short value_type;  // AST_VAR_TYPE  
       short value_flags;

       union {

	    union tagBINDING_DATA *value_ref; // reference to binding on heap.

	    size_t	stack2stack_ref; // if reference between two stack locations, then reference is via offset from start of stack.

	    size_t      capture_ref;

	    VALFUNCTION_CAPTURE  heap2stack_ref; // if reference between collection element to stack - then treat like capture.

	    long	long_value;

	    double	double_value;

	    VALSTRING   string_value;
    
	    VALARRAY    array_value;

	    VALHASH     hash_value;

	    VALFUNCTION func_value;

	} value;
    
        void *container; // if value is in container then garbage collection mark stage has to mark the collection 'in use' if an item in the collection is 'in use'

} BINDING_DATA_VALUE;

typedef union tagBINDING_DATA {
    BINDING_DATA_VALUE b;	
    
    VALACTIVATION activation_record;

} BINDING_DATA;

// initialise stack value biding of given type.
void BINDING_DATA_init( BINDING_DATA *binding, AST_VAR_TYPE value_type);

// show value of a data binding; formatting details are in global context.
void BINDING_DATA_print( FILE *out, BINDING_DATA *data , int level );

// copy binding value from to binding value to  - either by value or by reference (copy_by_value)
void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from , CP_KIND copy_by_value  );

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
  return ( binding->b.value_flags & S_MASK_NSTACK )  == 0;
}

// returns TRUE if binding is a reference
M_INLINE int IS_REF( BINDING_DATA *binding )
{
    return binding->b.value_flags & ( S_VAR_REF_HEAP | S_VAR_REF_STACK2STACK | S_VAR_REF_HEAP2STACK | S_VAR_CAPTURE_REF ); 
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

M_INLINE int BINDING_DATA_get_int( BINDING_DATA *data, long *val )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & (S_VAR_DOUBLE | S_VAR_INT)) == 0) {
    *val = 0;
    return -1;
  }
  if (data->b.value_type & S_VAR_DOUBLE) {
    *val = (long) data->b.value.double_value;
    return 0;
  }
  *val = data->b.value.long_value;
  return 0;
}

M_INLINE void BINDING_DATA_set_int( BINDING_DATA *data, long val )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if (data->b.value_type != S_VAR_INT ) {
    BINDING_DATA_init( data, S_VAR_INT );
  }
  data->b.value.long_value = val;
}

M_INLINE int BINDING_DATA_get_double( BINDING_DATA *data, double *val )
{
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_type & (S_VAR_DOUBLE | S_VAR_INT)) == 0) {
    *val = 0;
    return -1;
  }
  if (data->b.value_type & S_VAR_DOUBLE) {
    *val = data->b.value.double_value;
    return 0;
  }
  *val = data->b.value.long_value;
  return 0;
}

M_INLINE void BINDING_DATA_set_double( BINDING_DATA *data, long val )
{ 
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
  if (data->b.value_type != S_VAR_DOUBLE) {
    BINDING_DATA_init( data, S_VAR_DOUBLE );
  }
  data->b.value.double_value = val;
}

M_INLINE VALARRAY *BINDING_DATA_get_arr( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_flags & S_VAR_LIST) == 0) {
    return 0;
  }
  return &data->b.value.array_value;
}

M_INLINE VALHASH *BINDING_DATA_get_hash( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_flags & S_VAR_HASH) == 0) {
    return 0;
  }
  return &data->b.value.hash_value;
}

M_INLINE VALSTRING *BINDING_DATA_get_string( BINDING_DATA *data )
{
  if (IS_REF( data )) {
    data = BINDING_DATA_follow_ref( data );
  }
  if ((data->b.value_flags & S_VAR_STRING) == 0) {
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


typedef enum  {
  TSTATE_IDLE,	    // thread initialised or finished, not running.
  TSTATE_RUNNING,   // thread is running,
  TSTATE_SUSPENDED, // thread not running, somebody else is running.

} TSTATE;

typedef struct tagEVAL_THREAD {
  size_t  current_function_frame_start; 
  ARRAY binding_data_stack; // for evaluation of expressions and passing of function parameters and return values.

  struct tagAST_BASE *instr; // current instruction in current function

  struct tagEVAL_CONTEXT *context;
  VALACTIVATION *current_activation_record; // pointer to current activation record of current function. 
 
  DRING threads;
  int trace_on;
  TSTATE tstate;

  HASH  print_check_ref_loops;	// set for checking loops during printing of container.
  size_t has_loops; // if currently printed structure has been found to contain loops.

} EVAL_THREAD;

// create a new thread.
int EVAL_THREAD_init(EVAL_THREAD *thread, struct tagEVAL_CONTEXT *context );

// free a thread
void EVAL_THREAD_free(EVAL_THREAD *thread);


// get the binding that is at the top of the stack.
BINDING_DATA *EVAL_THREAD_get_stack_top( EVAL_THREAD *thread );

// get binding indexed from the start of the current frame onward. (stack_offset == 0 - return address. )
BINDING_DATA *EVAL_THREAD_stack_frame_offset( EVAL_THREAD *thread, size_t stack_offset );

// allocates new binding on stack 
BINDING_DATA *EVAL_THREAD_stack_alloc( EVAL_THREAD *thread, AST_VAR_TYPE vtype );

// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
void EVAL_THREAD_prepare_xcall( EVAL_THREAD *thread, AST_XFUNC_DECL *decl );

// prepare call; make sure the stack has enough room for call; reserve a slot for return value.
void EVAL_THREAD_prepare_call( EVAL_THREAD *thread, AST_FUNC_DECL *decl );

// precondition: call prepared and arguments have been pushed to the stack, make function call.
void EVAL_THREAD_call_xfunc( EVAL_THREAD *thread, AST_XFUNC_DECL *decl );

// make a new frame
// precondition: call prepared and arguments have been pushed to the stack. 
// postcondition: local variable are set to NULL, activation record on the stack.
void EVAL_THREAD_call_func( EVAL_THREAD *thread, AST_FUNC_DECL *decl, VALFUNCTION *function );

// finish function call, return to calling frame. returns -1 if top of stack reached.
int EVAL_THREAD_pop_frame ( EVAL_THREAD *thread );

// set value as return value of current stack trace.
int EVAL_THREAD_return_value( EVAL_THREAD *thread, BINDING_DATA *data );

// print stack backtrace.
int EVAL_THREAD_print_stack_trace( FILE *out, EVAL_THREAD *thread );

// yield value to calling function
int EVAL_THREAD_yield_value( EVAL_THREAD *thread, BINDING_DATA *data, BINDING_DATA **msg_val );

// activate a suspended thread (from calling thread)
int EVAL_THREAD_activate( EVAL_THREAD *thread, BINDING_DATA *msg, BINDING_DATA **msg_val );


// returns 1 if this frame is a top level frame.
int EVAL_THREAD_is_top_level_frame( EVAL_THREAD *thread );

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

M_INLINE BINDING_DATA *XCALL_param( XCALL_DATA *xcall, size_t num_param )
{
   if (num_param >= xcall->num_arguments) {
     return 0;
   }
   return ((BINDING_DATA *) xcall->thread->binding_data_stack.buffer) +  xcall->frame_offset + 1 + num_param;
}  

M_INLINE size_t XCALL_nparams( XCALL_DATA *xcall)
{
  return xcall->num_arguments;
}

/* ==================================================================================== */

typedef struct tagEVAL_CONTEXT {
  DRING gc_heap;  // list of all garbage collectd heap entries
  DRING threads; // list of all threads (interpreter threads)
  EVAL_THREAD main;  // the main thread
  EVAL_THREAD *current_thread; // current thread
  ARRAY bindings_global; // all global bindings (indexed array)

  jmp_buf jmpbuf; // runtime errors - if runtime error it bails out to mark.
  int  is_jmpbuf_set;

} EVAL_CONTEXT;

int  EVAL_CONTEXT_init( EVAL_CONTEXT *context );
void EVAL_CONTEXT_free( EVAL_CONTEXT *context );
void EVAL_CONTEXT_start( EVAL_CONTEXT *context );
void EVAL_CONTEXT_gc( EVAL_CONTEXT *context );
void EVAL_CONTEXT_runtime_error( EVAL_CONTEXT *context, const char *format, ... );
void EVAL_CONTEXT_print_stack_trace_all( FILE *out, EVAL_CONTEXT *context );


#endif



