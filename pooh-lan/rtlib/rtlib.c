
EVAL_CONTEXT *g_context;


#define RUNTIME_ERROR_MSG_LEN 1024
#define ARRAY_GROW_CAPACITY_BY 10
#define STRING_GROW_CAPACITY_BY 20
/* ==================================================================================== */

void *HEAP_alloc( size_t size )
{
  void *rt;
  
  rt = malloc( size );
  if (!rt) {
     EVAL_CONTEXT_gc( g_context );
     
     rt = malloc(size );
     if (!rt) {
       EVAL_CONTEXT_runtime_error( g_context, "out of memory");
     }
  } 
  return rt;
}

void HEAP_realloc( void **ptr, size_t size )
{
  void *ret;

  ret = realloc( *ptr, size );
  if (!ret) {
    EVAL_CONTEXT_gc( g_context );
    
    ret = realloc( *ptr, size );
    if (!ret) {
      EVAL_CONTEXT_runtime_error( g_context, "out of memory");
    }
  }
  *ptr = ret;
}

void HEAP_free( void *ptr )
{
  free(ptr);
}

/* ==================================================================================== */

int VALARRAY_set_capacity( VALARRAY *arr, size_t capacity )
{
  if (arr->capacity < capacity ) {
    capacity += ARRAY_GROW_CAPACITY_BY;
    HEAP_realloc( *arr->data, sizeof( BINDING_DATA) * capacity );
    arr->capacity = capacity;
  }
  return 0;
} 


void VALARRAY_copy( VALARRAY *to, VALARRAY *from, int deep_copy_values )
{
   size_i i;
   BINDING_DATA *binding, *newbinding;
   
   VALARRAY_set_capacity( to, from->size );
 
   for( i = 0; i < from->size; i++) {
     binding =  &to->data[ i ];
     newbinding = &from->data[  i ];

     if ( ! IS_NULL( newbinding ) ) {
       BINDING_DATA_copy( binding, newbinding );
     } else {
       BINDING_DATA_free( binding );
     }
   }
   for( ; i < to->size; i++) {
     if ( ! IS_NULL( binding ) ) {
       BINDING_DATA_free( binding );
     }
   }
   from->size = to->size;
}

void VALARRAY_free( VALARRAY *arr )
{
  BINDING_DATA *binding;
  size_t i;

  for( i = 0; i < arr->size; i++ ) {
    binding = &arr->data[i];
    if (! IS_NULL( binding ) ) {
      BINDING_DATA_free( arr->data );
    }
  }
  HEAP_free( arr->data );    
}


void VALARRAY_set( VALARRAY *arr, size_t idx, struct tagBINDING_DATA *mem, int copy_value)
{
   size_t nsize = idx + 1;

   VALARRAY_set_capacity( arr, nsize );
   if (arr->size < nsize) {
      for( i = arr->size; i < nsize ; i++)  {
        BINDING_DATA_init( arr->data + i, S_VAR_NULL );
      }
      arr->size =  nsize;
   }

   BINING_DATA_copy( mem, arr->data + i, copy_value );
   mem->b.container = arr;
} 

int VALARRAY_equal( VALARRAY *cmpa, VALARRAY *cmpb )
{
   if (cmpa->size != cmpb->size){
     return -1;
   }
   for(i =0; i < cmpa->size; i++) {
     ptra = &cmpa->data[ i ];
     ptrb = &cmpb->data[ i ];

     if (ptra != 0 && ptrb != 0) {
	if (BINDING_DATA_equal( ptra, ptrb ) ) {
	  return -1;
    	}
     }
     if (ptra != ptrb) {
       return -1;
     }
   }
   return 0;
}


size_t VALARRAY_hash( VALARRAY *arr )
{
  size_t i;
  BINDING_DATA *data;
  size_t ret = 0;

  for(i = 0; i < arr->size; i++) {
     data = &arr->data[ i ];
     ret += BINDING_DATA_hash( data );
  }
  return ret;
}

/* ==================================================================================== */

typedef struct tagHASH_VALUE_ENTRY {
  SRING link;
  size_t hash_value;
   
  BINDING_DATA key,value;  // includes pointer to hash structure, so that reference to element results in reference to the collection.
} HASH_VALUE_ENTRY;


void VALHASH_set_capacity( VALHASH *hash, size_t capacity )
{ 
  size_t nbuck,i;
  VALHASH tmp_hash;
  BINDING_DATA *key, *value;
  size_t hash_value;
  HASH_VALUE_ENTRY *entry,*cur_entry;
  SRING *bucket,*cur;

  if  (arr->size == 0) {
     nbuck =  UTIL_round_to_power_of_n( capacity );
     hash->buckets = HEAP_alloc( nbuck * sizeof(SRING) );

     for( i = 0; i < nbuck; i++ ) {
	SRING_init( &hash->buckets[ i ] );
     }
     return;
  }

  VALHASH_init( &tmp_hash );
  VALHASH_set_capacity( &tmp_hash, capacity );

  for(i = 0; i < hash->numbuckets; i++) {
    bucket = hash->buckets[ i ];

    SRING_FOREACH(cur, bucket ) {
       cur_entry = (HASH_VALUE_ENTRY *) cur;
       SRING_push_back( &tmp_hash->buckets[ hash_value & (tmp_hash->numbuckets - 1) ], &cur_entry->link );
    }
  }
  free(hash->buckets);
  memcpy( hash, &tmp_hash );

}

void VALHASH_copy( VALHASH *to, VALHASH *from, int copy_by_value )
{
   size_t i;
   SRING *fcur,*from_bucket;
   HASH_VALUE_ENTRY *from_entry,*copy_entry;

   VALHASH_free( to );

   to->buckets = HEAP_alloc( from->numbuckets * sizeof(SRING) );
   memset( to->buckets, 0, from->numbuckets * sizeof(SRING) ); 

   to->numbuckets = from->numbuckets;
   to->size = from->size;

   for( i = 0; i < from->size; i++ ) {
     copy_bucket = &to->buckets[ i ];
     from_bucket = &from->buckets[ i ];

     if (from_bucket) {
       SRING_FORACH( fcur, from_bucket )  {
         from_entry = (HASH_VALUE_ENTRY *) fcur;

         copy_entry = HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );

	 copy_entry->hash_value = from_entry->hash_value;
	 
	 BINDING_DATA_init( &copy_entry->key, S_VAR_NULL );
	 BINDING_DATA_copy( &copy_entry->key, &from_entry->key, 1 );
	 BINDING_DATA_init( &copy_entry->value, S_VAR_NULL );
	 BINDING_DATA_copy( &copy_entry->value, &from_entry->value, copy_by_value ); 

	 copy_entry->key.b.container = hash;
   	 copy_entry->value.b.container = hash;

	 SRING_insert_after( copy_bucket, copy_entry );
	 copy_bucket = copy_bucket->next;
       }
     } 	
   }
}

void VALHASH_free( VALHASH *hash )
{
  size_t i;
  HASH_VALUE_ENTRY *cur_entry;
  SRING *bucket, *cur;


  if ( hash->ordered_keys ) {
    free( hash->ordered_keys);  
  }
  if (hash->buckets) {
    
    for(i = 0; i < hash->numbuckets; i++) {
      bucket = hash->buckets[ i ];

      SRING_FOREACH(cur, bucket ) {
         cur_entry = (HASH_VALUE_ENTRY *) cur;

	 BINDING_DATA_free( &cur_entry->key );
	 BINDING_DATA_free( &cur_entry->value );
     }
     free(hash->buckets);
  }
}

int VALHASH_set( VALHASH *hash, struct tagBINDING_DATA *key, struct tagBINDING_DATA *value , int copy_by_value)
{
  size_t threshold;
  HASH_VALUE_ENTRY *entry,*cur_entry;
  SRING *bucket,*cur,*prev;

  if (hash->bucket == 0) {
    VALHASH_set_capacity( hash, HASH_INIT_SIZE );
  } else {

     threshold = (hash->size >> 1) + (hash->size >> 2);
     if (hash->size > threshold) {
  	  VALHASH_set_capacity( hash, hash->size << 1 );
     }
  }

  entry = (HASH_VALUE_ENTRY *) HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );
  
  BINDING_DATA_copy( key, &entry->key, 1 );
  BINDING_DATA_copy( key, &entry->value, copy_by_value );
  entry->hash_value = BINDING_DATA_hash( &entry->key );
  
  bucket = hash->buckets[ entry->hash_value & (hash->numbuckets - 1) ];
  
  prev = bucket;

  if (hash->ordered_keys) {
    free( hash->ordered_keys );
    hash->ordered_keys = 0;
  }


  prev = bucket;
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == entry->hash_value) {
      SRING_unlink_after(prev);
      SRING_insert_after(prev,entry); 
      // invalidate key,value and free old entry
      return 1; 
    }
    prev= prev->next;
  } 

  SRING_push_front(bucket, entry);
  hash->size++;

  return 2;
}

int VALHASH_delete_key( VALHASH *hash, struct tagBINDING_DATA *key )
{
  hash_value = BINDING_DATA_hash( &entry->key );
  bucket = hash->buckets[ hash_value & (hash->numbuckets - 1) ];
 
  prev = bucket;
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == entry->hash_value) {
      hash->size --;
      SRING_unlink_after(prev);
      
      // invalidate key,value and free old entry
      return 0;
    }
    prev= prev->next;
  } 
  return -1;
}

int VALHASH_find( VALHASH *hash, struct tagBINDING_DATA *key, struct tagBINDING_DATA **rval )
{
  size_t hash_value;

  hash_value = BINDING_DATA_hash( key );
  bucket = hash->buckets[ hash_value & (hash->numbucket - 1) ];
  
  SRING_FORACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == hash_value) {
      if (BINDING_DATA_cmp( &cur_entry->key, key ) ) {
	*rval = cur_entry->value;
	return 0;
      }
    }
  }
  return -1;
}
 
int VALHASH_iterate( VALHASH *hash, struct tagBINDING_DATA **key, struct tagBINDING_DATA **value, VALHASHPOS *pos )
{
  HASH_VALUE_ENTRY *entry;

  if (!pos->bucket) {
    pos->bucket = hash->bucket;
    pos->bucket_pos = hash->bucket;
  }

  while(1) {
    if ( (pos->bucket_pos = SRING_get_next( pos->bucket, pos->bucket_pos )) != 0) {
      entry = (HASH_VALUE_ENTRY *) pos->bucket_pos;

      *key = &entry->key;
      *value = &entry->value;
	
      return 1;
    }

    pos->bucket_pos = ++ pos->bucket;

    if (pos->bucket == hash->bucket + hash->numbuckets) {
       *key = *value = 0;
       return 0;
    }
  }

  // never get here
  return 1;
}


size_t VALHASH_hash( VALHASH *hash )
{
  VALHASHPOS pos;
  BINDING_DATA *key,*value;
  size_t ret = 0;

  VALHASHPOS_init( &pos );

  while( VALHASH_iterate( hash, &key, &value, &pos ) ) {
    ret += BINDING_DATA_hash( key );
    ret += BINDING_DATA_hash( value );
  }
  return ret;
}

/* ==================================================================================== */

void  VALSTRING_init( VALSTRING *string )
{
  binding->data.value.string_value.string = 0;
  binding->data.value.string_value.length = 0;
  binding->data.value.string_value.capacity = 0;
}


void VALSTRING_set_capacity( VALSTRING *to, size_t capacity )
{
  if (to->capacity < capacity) {
    capacity += STRING_GROW_CAPACITY_BY; 
    HEAP_realloc( &string->string, capacity );
    to->capacity = capacity;
    return 1;
  }
  return 0;
}

void VALSTRING_set(VALSTRING *string, void *ptr, size_t length)
{
  VALSTRING_set_capacity( string, length );
  memcpy( string->string, ptr, length );a
  string->length = length;
}

void VALSTRING_free(VALSTRING *string )
{
  if (string->string) {
    free(string->string);
  }
  VALSTRING_init_empty( string );  
}

int VALSTRING_copy( VALSTRING *to, VALSTRING *from )
{
  VALSTRING_set_capacity( from->capacity );
  VALSTRING_set( to, from->string, from->length );
  return 0;
}

int VALSTRING_find( VALSTRING *hay, VALSTRING *needle )
{
   char *ret;

   if (!hay->string || !needle->string) {
     return -1;
   }

   ret = (char *) memmem( hay->string, hay->length, needle->string, needle->length );

   if (!ret) {
     return -1;
   }

   return ret - hay->string;
}

int VALSTRING_substr( VALSTRING *from, size_t offset, size_t length, VALSTRING *to)
{
  offset = min( from->size, offset );

  if ((offset + length) > from->size) {
    length = from->size - offset;
  }

  VALSTRING_set( to, from->string + offset, length ); 
  return 0;
}

int  VALSTRING_cmp( VALSTRING *argA, VALSTRING *argB)
{
  return strcmp( argA->string != 0 ? argA->string : "", argB->string != 0 ? argB->string : "" );
}

/* ==================================================================================== */
void BINDING_DATA_init( BINDING_DATA *binding, AST_VAR_TYPE value_type)
{
   binding->b.value_type = value_type;
   binding->b.value_flags = 0;
   binding->b.container = 0;
   switch( value_type ) {
    case S_VAR_INT:
      binding->b.value.long_value = 0;
      break;
    case S_VAR_DOUBLE:
      binding->b.value.double_value = 0;
      break;
    case S_VAR_STRING:
      VALSTRING_init_empty( &binding->b.value.string_value );
      break;
    case S_VAR_CODE:
      VALFUNCTION_init( &binding->b.value.func_value );
      break;
    case S_VAR_HASH:
      if (HASH_init( &binding->b.value.hash_value, 10, 0, binding_hash_compare, 0 ) ) {
        EVAL_CONTEXT_gc( g_context );
        if (HASH_init( &binding->b.value.hash_value, 10, 0, binding_hash_compare, 0 ) ) {
	  EVAL_CONTEXT_runtime_error( g_context, "out of memory");
        }
      }
      break;
    case S_VAR_LIST:
      if (ARRAY_init( &binding->b.value.array_value, 0, sizeof( BINDING_DATA ) ) ) {
        EVAL_CONTEXT_gc( g_context );
        if (ARRAY_init( &binding->b.value.array_value, 0, sizeof( BINDING_DATA ) ) ) {
	  EVAL_CONTEXT_runtime_error( g_context, "out of memory");
	}
      }
      break;
    default:
   }
}


void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from, int copy_by_value  )
{
   BINDING_DATA_free( to );

   switch( from->value_type ) {
    case S_VAR_INT:
      to->b.value.long_value = from->b.value.long_value;
      break;
    case S_VAR_DOUBLE:
      to->b.value.double_value = to->b.value.doubl_value;
      break;
    case S_VAR_STRING:
      VALSTRING_init( to );
      VALSTRING_copy( to, from );
      break;
    case S_VAR_CODE:
      break;
    case S_VAR_HASH:
      VALHASH_init( to );
      VALHASH_copy( to, from, 1 );
      break;
    case S_VAR_LIST:
      VALARRAY_init( to );
      VALARRAY_copy( to, from, 1 );
      break;
    default:
   }
 }

void BINDING_DATA_free( BINDING_DATA *binding )
{
   switch( from->value_type ) {
    case S_VAR_STRING:
      VALSTRING_free( binding );
      break;
    case S_VAR_CODE:
      break;
    case S_VAR_HASH:
      VALHASH_free( binding );
      break;
    case S_VAR_LIST:
      VALARRAY_free( binding );
      break;
    default:
   }
   BINDING_DATA_init( S_VAR_NULL ) 
}

/* ==================================================================================== */

BINDING_DATA *BINDING_DATA_MEM_new(  AST_VAR_TYPE value_type ) 
{
  BINDING_DATA_MEM *mem;

  mem = (BINDING_DATA_MEM *) HEAP_alloc( sizeof( BINDING_DATA_MEM ) );

  DRING_push_back( &g_context->gc_heap, &mem->heap_entry );
 
  BINDING_DATA_init( &mem->data, value_type );

  mem->value_flags |= S_VAR_HEAP_VALUE;

  return &mem->data;
}

void BINDING_DATA_MEM_free( BINDING_DATA *pdata ) 
{
  BINDING_DATA_MEM *mem;

  mem = _OFFSETOF( pdata, BINDING_DATA_MEM, data );
  DRING_unlink( &mem->heap_entry );
  free(mem);
}

/* ==================================================================================== */

int EVAL_THREAD_init(EVAL_THREAD *thread, EVAL_CONTEXT *context )
{
   thread->current_function_frame_start = -1;
   if (ARRAY_init( &thread->binding_data_stack, sizeof(BINDING_DATA), 10 ) ) {
     return -1;
   }
   thread->context = context;
   thread->current_activation_record = 0;
   DRING_init( &thrad->threads );
   thread->trace_on = 0;
   return 0;
}

void EVAL_THREAD_push_frame( EVAL_THREAD *thread, AST_FUNC_DECL *decl, VALFUNCTION *function )
{
  size_t frame_size_min = FRAME_START_SIZE + decl->
}

void EVAL_THREAD_pop_frame ( EVAL_THREAD *thread )
{ 

  // clear all local variables.
  
  //
  thread->current_activation_record = thread->current_activation_record->parent;
  thread->instr = thread->current_activation_record->instr;
  thread->current_function_frame_start = thread->current_activation_record->function_frame_start;
}

BINDING_DATA *EVAL_THREAD_stack_at( EVAL_THREAD *thread, size_t stack_offset )
{
   BINDING_DATA **data = (BINDING_DATA **) ARRAY_at( &thread->binding_data_stack, ARRAY_size( &thread->binding_data_stack ) - 1 - stack_offset );
   return *data;
}

BINDING_DATA *EVAL_THREAD_stack_frame_offset( EVAL_THREAD *thread, size_t stack_offset )
{
   size_t pos; 

   pos = thread->current_function_frame_start + stack_offset; 

   if (thread->current_function_frame_start == (size_t) -1 || pos > ARRAY_size( &thread->binding_data_stack )) {
      assert(0);
   }
   BINDING_DATA **data = (BINDING_DATA **) ARRAY_at( &thread->binding_data_stack, ARRAY_size( &thread->binding_data_stack ) - 1 - stack_offset );
   return *data;
}

int EVAL_THREAD_print_stack_trace( EVAL_THRAD *thread)
{
  BINDING_DATA *activation_record;
  YYLTYPE *location;
  AST_BASE *ret_instr;
  int first;

  for( first = 1, activation_record = thread->current_activation_record; activation_record != 0; activation_record = activation_record->container )
  {
     ret_instr = thread ? thread->instr : activation_record->value.func_activation_record.ret_instr;
     location = ret_instr->base.location;

     fprintf("%s:%d %s\n",  LEXER_get_file_name(LEXCONTEXT *pc, location->file_id), location->first_line,  activation_record->value.func_activation_record.fdecl->fname );
     first = 0;
  }
  return 0;
}


/* ==================================================================================== */
int EVAL_CONTEXT_init( EVAL_CONTEXT *context )
{
  if (ARRAY_init( context->bindings_global, sizeof(BINDING_DATA), 10 ) ) {
    return -1;
  }
  if (EVAL_THREAD_init( &context->main, context )) {
    return -1;
  }
  DRING_init( &context->gc_heap );
  DRING_init( &context->threads );

  context->is_jmpbuf_set = 0;

  return 0;
}


void EVAL_CONTEXT_mark_binding( BINDING_DATA *binding )
{
  VALARRAY    *parray_value;
  VALHASH     *phash_value;
  VALFUNCTION *pfunc_value;
  BINDING_DATA *key, *value;
  void *pos_iter;

  while (1)   {

    if (binding->b.value_flags & S_VAR_VALUE_REF) {  
      binding = binding->b.value.value_ref;
    } else if (binding->b.value_flags & S_VAR_HEAP_COLLECTION_ENTRY ) {
      binding = binding->b.container;
    } else {
      break;
    }
  }

  if ( ! binding->b.value_flags & S_VAR_HEAP_VALUE ||
	 binding->b.value_flags & S_VAR_HEAP_GC_MARK) {
    return;
  }
  
  binding->b.value_flags |= S_VAR_HEAP_GC_MARK; 

  switch( binding->b.value_type ) {

    case S_VAR_CODE:
      pfunc_value = &binding->b.value.func_value;
      EVAL_CONTEXT_mark_binding( pfunc_value->this_environment ); 
      break; 

    case S_VAR_HASH:
      phash_value = &binding->b.value.hash_value;
      for( pos_iter = 0; VALHASH_iterate( phash_value, &key, &value, &pos_iter ) ; )
      {
        EVAL_CONTEXT_mark_binding( key ); 
        EVAL_CONTEXT_mark_binding( value ); 
      }
      break;

    case S_VAR_LIST:
      parray_value = &binding->value.array_value;
      for( pcur = parray_value->data, pend = parray_value->size; pcur != pend; ++pcur )
      {
	EVAL_CONTEXT_mark_binding( pcur ); 
      }
      break;

    default:
  }

  if (binding->container != 0) {
     EVAL_CONTEXT_mark_binding( binding->container ); 
  }
}

void EVAL_CONTEXT_mark_bindings( ARRAY *bindings )
{
  size_t i;
  BINDING_DATA *binding, *start;

  binding = ARRAY_at( &binding, ARRAY_size( &binding ) - 1 );
  start = ARRAY_at( &binding, 0 );

  if (binding != start) 
  do {

    // examine current binding
    EVAL_CONTEXT_mark_binding( binding );

    if (binding == start) {
      break;
    }  
    binding --;

  } while( 1 );
}


int EVAL_CONTEXT_mark( EVAL_CONTEXT *context )
{
   DRING *cur_thread_base;
   EVAL_THREAD *cur_thread;
   
   EVAL_CONTEXT_mark_bindings( &context->bindings_global );	
   
   // for each thread
   DRING_FOREACH( cur_thread_base, &context->threads )
   {  
     cur_thread = _OFFSETOF( cur_thread_base, EVAL_THREAD, threads );
     EVAL_CONTEXT_mark_bindings( &cur_thread->binding_data_stack );	
   //}

}

void EVAL_CONTEXT_sweep( EVAL_CONTEXT *context )
{
  DRING *cur, *next;
  BINDING_DATA_MEM *mem;

  DRING_FOREACH_SAVE( cur, next, &context->heap )
  {
    mem  = _OFFSETOF( cur, BINDING_DATA_MEM, data );
    if (! mem->data.b.value_flags & S_VAR_HEAP_GC_MARK) {
      // container and its members are marked independently, so do not
      // need to delete contained items for container. sweep will take care of them.s
      BINDING_DATA_MEM_free( cur );
    } else {
      mem->data.b.value_flags &= ~S_VAR_HEAP_GC_MARK;
    }   
  }
}

int EVAL_CONTEXT_gc( EVAL_CONTEXT *context )
{
  EVAL_CONTEXT_mark( context );
  EVAL_CONTEXT_sweep( context );
}


int EVAL_CONTEXT_runtime_error( EVAL_CONTEXT *context, const char *format, ... )
{
  char msg[RUNTIME_ERROR_MSG_LEN];
  int len;
  va_list ap;

  va_start(ap, format);
  len = vsnprintf( msg, sizeof(msg) - 1, format, ap );

  fprintf( stderr, "%s\n", msg );

  EVAL_CONTEXT_print_stack_trace_all( context );

  if (context->is_jmpbuf_set) {
    longjmp( context->jmpbuf, 1 ); 
  } else {
    exit(1);
  }
}

int EVAL_CONTEXT_print_stack_trace_all( EVAL_CONTEXT *context )
{
   DRING *cur_thread_base;
   EVAL_THREAD *cur_thread;
  
   DRING_FOREACH( cur_thread_base, &context->threads )
   {  
     cur_thread = _OFFSETOF( cur_thread_base, EVAL_THREAD, threads );
     EVAL_THREAD_print_stack_trace( cur_thread );
   }    
}


