#ifndef __EVAL_H__
#define __EVAL_H__

/* ==================================================================================== */

// allocate memory block, if allocation fails then run garbage collection, if it fails aftrward then issue runtime error.
void *HEAP_alloc( size_t size );

// reallocate block; also do gc...
void HEAP_realloc( void **ptr, size_t size );


/* ==================================================================================== */

// dynamic array value as part of runtime
typedef struct tagVALARRAY {
  struct tagBINDING_DATA *data; // includes back pointer to collection, so that reference to element results in reference to the collection.
  size_t size; // number of non null        
  size_t capacity; // size can grow up to capacity - without reallocating the array. 

} VALARRAY;

// initialise and empty array
M_INLINE void VALARRAY_init( VALARRAY *arr, size_t capacity )
{
  arr->data = 0;
  arr->size = 0;
  arr->capacity = 0;
}

// reserve capacity - reserve room for bindings.
void VALARRAY_set_capacity( VALARRAY *arr, size_t capacity )

// copy array from into array to . capacity of from is not changes.
void VALARRAY_copy( VALARRAY *to, VALARRAY *from, int deep_copy_values );

// free array and all the bindings that it contai s.
void VALARRAY_free( VALARRAY *arr );

// copy binding into array slot; here index is from 0
void VALARRAY_set( VALARRAY *arr, size_t idx, struct tagBINDING_DATA *mem, int copy_by_value);

// get array slot; returns 0 if index is out of bound; here index is from 0 
M_INLINE struct tagBINDING_DATA *VALARRAY_get( VALARRAY *arr, size_t idx );
{
  if (idx >= arr->size) {
    return 0;
  }
  return &arr->data[ idx ];
}


// compares two arrays; the two are equal if they are of the same size and all elements are equal
int VALARRAY_equal( VALARRAY *cmpa, VALARRAY *cmpb );


// return hash of array object ( used if array is a key value )
size_t VALARRAY_hash( VALARRAY *arr );


// returns number of elements in array
M_INLINE size_t VALARRAY_size( VALARRAY *arr )
{
  return arr->size;
}

/* ==================================================================================== */

// hash value as part of runtime
typedef struct tagVALHASH {
  SRING *buckets; // array of buckets.
  size_t numbuckets; // number of buckets
  size_t size; // number of elements in hash
  struct tagBINDING_DATA **ordered_keys; // if two hash tables are compared for equality (hash entry is part of a key ... )
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
M_INLINE void  VALHASH_init( VALHASH *hash );
{
  hash->buckets = 0;
  hash->numbuckets = 0;
  hash->size = 0;
  hash->ordered_keys = 0;
}

// reserve capacity - reserve room for bindings.
void VALHASH_set_capacity( VALHASH *hash, size_t capacity )

// copy hash from into hash to 
void VALHASH_copy( VALHASH *to, VALHASH *from, int deep_copy_values );

// free hash and all contained bindings (keys and values)
void VALHASH_free( VALHASH *hash );

// insert new key value pair. lookup of value by key is supposed to be fast. returns 1 if existing key updated; 2 if new key inserted.
int  VALHASH_set( VALHASH *hash, struct tagBINDING_DATA *key, struct tagBINDING_DATA *value , int copy_by_value);

// delete a key value pair identified by key. return 0 if entry has been deleted; -1 if no such entry.
int VALHASH_delete_key( VALHASH *hash, struct tagBINDING_DATA *key )

// lookup of value by key; returns 0 if entry has been found; -1 if no such entry.
int VALHASH_find( VALHASH *hash, struct tagBINDING_DATA *key, struct tagBINDING_DATA **rval );

// iterate over hash; the position of iteration (pos) is passed as argument; key and value of current position is returned and position incremented; returns 0 on eof iteration
int VALHASH_iterate( VALHASH *hash, struct tagBINDING_DATA **key, struct tagBINDING_DATA **value, VALHASHPOS *pos );

// return hash of hash object ( if hash is a key value )
size_t VALHASH_hash( VALHASH *hash );

// returns number of elements in hash
M_INLINE size_t VALHASH_size( VALHASH *hash ) 
{
  return hash->size;
}

int VALHASH_equal( VALHASH *cmpa, VALHASH *cmpb );


#define S_VAR_VALUE_REF    0x1
#define S_VAR_HEAP_VALUE   0x2
#define S_VAR_HEAP_GC_MARK 0x4
#define S_VAR_HEAP_COLLECTION_ENTRY 0x8
#define S_VAR_HASH_HAS_SORTED_VALUES 0x16

/* ==================================================================================== */
typedef struct tagVALSTRING {
  char   *string;
  size_t  length;
  size_t  capacity;
} VALSTRING;

M_INLINE void VALSTRING_init( VALSTRING *string );
{
  string->string = 0;
  string->length = 0;
  string->capacity = 0;
}

void VALSTRING_set_capacity( VALSTRING *string, size_t capacity );
void VALSTRING_set( VALSTRING *string, void *ptr, size_t length);
void VALSTRING_free( VALSTRING *string );
int  VALSTRING_copy( VALSTRING *to, VALSTRING *from );
int  VALSTRING_find( VALSTRING *hay, VALSTRING *needle );
int  VALSTRING_substr( VALSTRING *from, size_t offset, size_t length, VALSTRING *to);
int  VALSTRING_cmp( VALSTRING *argA, VALSTRING *argB);
size_t VALSTRING_hash( VALSTRING *string );

/* ==================================================================================== */

typedef struct tagVALFUNCTION {
  struct tagBINDING_DATA *this_environment;
  struct tagAST_FUNC_DECL *fdecl;
} VALFUNCTION;

void VALFUNCTION_init( VALFUNCTION *func )
{
  func->fdecl = 0;
  func->this_environment = 0;
}

/* ==================================================================================== */
typedef struct tagVALACTIVATION {
   struct tagAST_FUNC_DECL *fdecl; // how this function looks like in AST.
   size_t function_frame_start; // index of return value for this thread.
   struct tagAST_BASE *ret_instr; // return address; after returning to function resume with this instruction.
   struct tagVALFUNCTION *function_object; // pointer to function object (if available). 
   struct tagBINDING_DATA *parent_function; // - this value is overlayed with container field in BINDING_DATA structure.
} VALACTIVATION;

/* ==================================================================================== */
typedef union tagBINDING_DATA {
    struct {
       short value_type;  // AST_VAR_TYPE  
       short value_flags;

       union {

	    struct tagBINDING_DATA *value_ref;

	    long	long_value;

	    double	double_value;

	    VALSTRING   string_value;
    
	    VALARRAY    array_value;

	    VALHASH     hash_value;

	    VALFUNCTION func_value;


	} value;

        struct tagBINDING_DATA *container; // if value is in container then garbage collection mark stage has to mark the collection 'in use' if an item in the collection is 'in use'
    } b;	
    
    VALACTIVAION activation_record;

} BINDING_DATA;

void BINDING_DATA_init( BINDING_DATA *binding, AST_VAR_TYPE value_type);
void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from , int copy_by_value);
void BINDING_DATA_free( BINDING_DATA *binding );
int  BINDING_DATA_equal( BINDING_DATA *cma, BINDING_DATA *cmpb );
size_t BINDING_DATA_hash( BINDING_DATA *hash );


// returns true if binding is a null value
M_INLINE int IS_NULL( BINDING_DATA *binding )    
{
  return binding->b.value_type != S_VAR_NULL;
}


/* ==================================================================================== */

typedef struct tagBINDING_DATA_MEM {
  DRING heap_entry; // list of all in memory values (for garbage collector to walk over 'all values')
  BINDING_DATA data;
} BINDING_DATA_MEM;

/* ==================================================================================== */

typedef struct tagEVAL_THREAD {
  size_t  current_function_frame_start; 
  ARRAY binding_data_stack; // for evaluation of expressions and passing of function parameters and return values.

  struct tagAST_BASE *instr; // current instruction in current function

  struct tagEVAL_CONTEXT *context;
  struct tagBINGIND_data *current_activation_record; // pointer to current activation record of current function. 
 
  DRING threads;
  int trace_on;
  
} EVAL_THREAD;

/* ==================================================================================== */

typedef struct tagEVAL_CONTEXT {
  DRING gc_heap;  // list of all garbage collectd heap entries
  DRING threads; // list of all threads (interpreter threads)
  EVAL_THREAD *main;  // the main thread
  EVAL_THREAD *current_thread; // current thread
  ARRAY bindings_global; // all global bindings (indexed array)

  jmp_buf jmpbuf; // runtime errors - if runtime error it bails out to mark.
  int  is_jmpbuf_set;

} EVAL_CONTEXT;

int EVAL_CONTEXT_gc( EVAL_CONTEXT *context );
int EVAL_CONTEXT_runtime_error( EVAL_CONTEXT *context, const char *format, ... );
int EVAL_CONTEXT_print_stack_trace_all( EVAL_CONTEXT *context );

#endif



