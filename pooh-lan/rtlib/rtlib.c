#include "rtlib.h"
#include "stdio.h"
#include <cutils/util.h>
#include <stdarg.h>

static EVAL_CONTEXT *g_context; // the current interpreter context.
static EVAL_THREAD  *g_cur_thread;

#define RUNTIME_ERROR_MSG_LEN 1024
#define ARRAY_GROW_CAPACITY_BY 10
#define STRING_GROW_CAPACITY_BY 20
#define HASH_INIT_SIZE 20


/* ==================================================================================== */
typedef struct tagPRINT_CHECK_LOOP {
  HASH_Entry entry;
  void *key;
  void *to;
} PRINT_CHECK_LOOP;

// check if array contains self references.
void VALARRAY_check_ref( VALARRAY *arr );


// check if hash contains self references.
void VALHASH_check_ref( VALHASH *hash );

int  BINDING_DATA_check_ref( BINDING_DATA *data );

void EVAL_THREAD_clear_check_ref( EVAL_THREAD *thread );
void EVAL_THREAD_print_ref_add_loop( BINDING_DATA *from, BINDING_DATA *to);

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

void VALARRAY_set_capacity( VALARRAY *arr, size_t capacity )
{
  if (arr->capacity < capacity ) {
    capacity += ARRAY_GROW_CAPACITY_BY;
    HEAP_realloc( (void **) &arr->data, sizeof( BINDING_DATA) * capacity );
    arr->capacity = capacity;
  }
} 


void VALARRAY_copy( VALARRAY *to, VALARRAY *from, CP_KIND deep_copy_values  )
{
   size_t i;
   BINDING_DATA *tobinding,*frombinding;
   
   VALARRAY_set_capacity( to, from->size );
 
   for( i = 0; i < from->size; i++) {
     tobinding =  &to->data[ i ];
     frombinding = &from->data[  i ];

     if ( ! IS_NULL( frombinding ) ) {
       BINDING_DATA_init( tobinding, S_VAR_NULL );
       BINDING_DATA_copy( tobinding, frombinding, deep_copy_values );
 
       tobinding->b.container = _OFFSETOF(to, BINDING_DATA_VALUE, value );
       tobinding->b.value_flags |= S_VAR_COLL_ENTRY;	 

     } else {
       BINDING_DATA_free( tobinding );
     }
   }
   for( ; i < to->size; i++) {
     tobinding =  &to->data[ i ];
     if ( ! IS_NULL( tobinding ) ) {
       BINDING_DATA_free( tobinding );
     }
   }
   to->size = from->size;
}

void VALARRAY_free( VALARRAY *arr )
{
  BINDING_DATA *binding;
  size_t i;

  for( i = 0; i < arr->size; i++ ) {
    binding = &arr->data[i];
    if (! IS_NULL( binding ) ) {
      binding->b.container = 0;
      BINDING_DATA_free( binding );
    }
  }
  HEAP_free( arr->data );    
}

union tagBINDING_DATA *VALARRAY_get( VALARRAY *arr, size_t idx )
{
  if (idx >= arr->size) {
    return 0;
  }
  return &arr->data[ idx ];
}


void VALARRAY_set( VALARRAY *arr, size_t idx, union tagBINDING_DATA *mem,  CP_KIND copy_value)
{
   size_t nsize = idx + 1, i;
   BINDING_DATA *data;

   VALARRAY_set_capacity( arr, nsize );
   if (arr->size < nsize) {
      for( i = arr->size; i < nsize ; i++)  {
        data = &arr->data[ i ];  
        BINDING_DATA_init( data, S_VAR_NULL );
        data->b.container = _OFFSETOF(arr, BINDING_DATA_VALUE, value );
        data->b.value_flags |= S_VAR_COLL_ENTRY;	 
      }
      arr->size =  nsize;
   }

   data = &arr->data[ idx ];  
   BINDING_DATA_copy( data, mem, copy_value );
   data->b.container = _OFFSETOF(arr, BINDING_DATA_VALUE, value );
   data->b.value_flags |= S_VAR_COLL_ENTRY;	 


} 

int VALARRAY_equal( VALARRAY *cmpa, VALARRAY *cmpb )
{
  size_t i;

  if (cmpa->size != cmpb->size) {
    return -1;
  }

  for( i = 0; i < cmpa->size; i++ ) {
     if (!IS_NULL( &cmpa->data[ i ] ) && !IS_NULL( &cmpb->data[ i ] )) {
	
	if ( BINDING_DATA_cmp( &cmpa->data[ i ], &cmpb->data[ i ] ) != 0) {
	  return -1;
	}
     } else {
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
  size_t hval;

  for(i = 0; i < arr->size; i++) {
     data = &arr->data[ i ];
     hval = BINDING_DATA_hash( data ); 
     ret = ret ^ ( ( hval << 1 ) | (hval & 1 ) );
  }
  return ret;
}

#define DISPLAY_MIN_RANGE_LENGTH 3

void VALARRAY_print( FILE *out, VALARRAY *arr, int level )
{
  BINDING_DATA *data;
  size_t i;
#if 1  
  long  val;
  size_t j;
  BINDING_DATA *cdata;
#endif

  fprintf( out,"[ ");
  for( i = 0; i < VALARRAY_size( arr ) ; i++ ) {
    if (i != 0) {
      fprintf( out, ", " );
    }
    data = VALARRAY_get( arr , i ); 

    assert( data );
  
    assert( data->b.container == _OFFSETOF( arr, BINDING_DATA_VALUE, value ) );
  
#if 1  
    if (data->b.value_type == S_VAR_INT ) {
       // check if there is a sub range of integers.
       for( j = i + 1, val = data->b.value.long_value + 1; j < VALARRAY_size( arr ); j ++, val++) {
	  cdata = VALARRAY_get( arr, j );
	  
	  assert( cdata->b.container ==  _OFFSETOF(arr, BINDING_DATA_VALUE, value ) );

	  if ( cdata->b.value_type != S_VAR_INT || cdata->b.value.long_value != val) {
	    break;
	  }

	  if ( g_cur_thread->has_loops && HASH_find( &g_cur_thread->print_check_ref_loops, cdata, sizeof(void *) ) != 0) {
	    break;
	  }

       }

       if ( (j - DISPLAY_MIN_RANGE_LENGTH ) >= i) {
	  fprintf( out, "%ld-%ld", data->b.value.long_value, val - 1 );
	  i = j - 1;
	  continue;
       }
    }
#endif

//fprintf( out, " ~%p~ ", data);
    BINDING_DATA_print( out, data, level + 1 );
  } 
  fprintf( out," ]");
}

void  VALARRAY_check_ref( VALARRAY *arr )
{
  size_t i;
  BINDING_DATA *data;

  for( i = 0; i < VALARRAY_size( arr ) ; i++ ) {
    data = VALARRAY_get( arr , i );  
    if (!IS_NULL( data )) {
      BINDING_DATA_check_ref(  data );
    }
  }

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
  HASH_VALUE_ENTRY *cur_entry;
  SRING *bucket,*cur, *next;

  if  (hash->size == 0) {
     nbuck =  UTIL_round_to_power_of_n( capacity );
     hash->buckets = HEAP_alloc( nbuck * sizeof(SRING) );
     hash->numbuckets = nbuck;

     for( i = 0; i < nbuck; i++ ) {
	SRING_init( &hash->buckets[ i ] );
     }
     return;
  }

  VALHASH_init( &tmp_hash );
  VALHASH_set_capacity( &tmp_hash, capacity );

  for(i = 0; i < hash->numbuckets; i++) {
    bucket = &hash->buckets[ i ];

    SRING_FOREACH_SAVE(cur, next, bucket ) {
       cur_entry = (HASH_VALUE_ENTRY *) cur;
       SRING_push_front( &tmp_hash.buckets[ i ], &cur_entry->link );
    }
  }
  free(hash->buckets);
  memcpy( hash, &tmp_hash, sizeof( VALHASH ) );

}

void VALHASH_copy( VALHASH *to, VALHASH *from, CP_KIND copy_by_value )
{
   size_t i;
   SRING *fcur,*from_bucket, *copy_bucket;
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
       SRING_FOREACH( fcur, from_bucket )  {
         from_entry = (HASH_VALUE_ENTRY *) fcur;

         copy_entry = HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );

	 copy_entry->hash_value = from_entry->hash_value;
	 
	 BINDING_DATA_init( &copy_entry->key, S_VAR_NULL );
	 BINDING_DATA_copy( &copy_entry->key, &from_entry->key, 1 );
	 BINDING_DATA_init( &copy_entry->value, S_VAR_NULL );
	 BINDING_DATA_copy( &copy_entry->value, &from_entry->value, copy_by_value ); 

	 copy_entry->key.b.container = _OFFSETOF( to, BINDING_DATA_VALUE, value );
   	 copy_entry->key.b.value_flags |= S_VAR_COLL_ENTRY;	 

	 copy_entry->value.b.container = _OFFSETOF( to, BINDING_DATA_VALUE, value );
   	 copy_entry->value.b.value_flags |= S_VAR_COLL_ENTRY;	 

	 SRING_insert_after( copy_bucket, &copy_entry->link );
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
      bucket = &hash->buckets[ i ];

      SRING_FOREACH(cur, bucket ) {
         cur_entry = (HASH_VALUE_ENTRY *) cur;

         cur_entry->key.b.container = 0;
	 BINDING_DATA_free( &cur_entry->key );
	 cur_entry->value.b.container = 0;
	 BINDING_DATA_free( &cur_entry->value );
      }	 
     }
     free(hash->buckets);
  }
}

int VALHASH_set( VALHASH *hash, union tagBINDING_DATA *key, union tagBINDING_DATA *value , CP_KIND copy_by_value)
{
  size_t threshold,idx;
  HASH_VALUE_ENTRY *entry,*cur_entry;
  SRING *bucket,*cur,*prev;

  if (hash->buckets == 0) {
    VALHASH_set_capacity( hash, HASH_INIT_SIZE );
  } else {

     threshold = (hash->size >> 1) + (hash->size >> 2);
     if (hash->size > threshold) {
  	  VALHASH_set_capacity( hash, hash->size << 1 );
     }
  }

  entry = (HASH_VALUE_ENTRY *) HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );
  
  BINDING_DATA_init( &entry->key, S_VAR_NULL );
  BINDING_DATA_copy( &entry->key, key, 1 );
  BINDING_DATA_init( &entry->value, S_VAR_NULL );
  BINDING_DATA_copy( &entry->value, value, copy_by_value );

  entry->key.b.container = _OFFSETOF(hash, BINDING_DATA_VALUE, value );
  entry->key.b.value_flags |= S_VAR_COLL_ENTRY;	 

  entry->value.b.container = _OFFSETOF(hash, BINDING_DATA_VALUE, value );
  entry->value.b.value_flags |= S_VAR_COLL_ENTRY;	 
 
  entry->hash_value = BINDING_DATA_hash( &entry->key );
  
  idx = entry->hash_value & (hash->numbuckets - 1);  
  bucket = &hash->buckets[ idx ];
  
  prev = bucket;

  if (hash->ordered_keys) {
    free( hash->ordered_keys );
    hash->ordered_keys = 0;
  }


  // if value with same key exists - delete the old value.
  prev = bucket;
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == entry->hash_value) {
      SRING_unlink_after(prev);
      SRING_insert_after(prev, &entry->link); 
      // invalidate key,value and free old entry
      return 1; 
    }
    prev= prev->next;
  } 

  SRING_push_front( bucket, &entry->link );
  hash->size++;

  return 2;
}

int VALHASH_delete_key( VALHASH *hash, union tagBINDING_DATA *key )
{
  size_t hash_value;
  SRING *bucket,*cur,*prev;
  HASH_VALUE_ENTRY *cur_entry;
  
  hash_value = BINDING_DATA_hash( key );
  bucket = &hash->buckets[ hash_value & (hash->numbuckets - 1) ];
 
  prev = bucket;
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == hash_value) {
      hash->size --;
      SRING_unlink_after(prev);
      
      // invalidate key,value and free old entry
      return 0;
    }
    prev= prev->next;
  } 
  return -1;
}

int VALHASH_find( VALHASH *hash, union tagBINDING_DATA *key, union tagBINDING_DATA **rval )
{
  size_t hash_value;
  SRING *bucket,*cur;
  HASH_VALUE_ENTRY *cur_entry;

  hash_value = BINDING_DATA_hash( key );
  bucket = &hash->buckets[ hash_value & (hash->numbuckets - 1) ];
  
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == hash_value) {
	  if (BINDING_DATA_cmp( &cur_entry->key, key )  == 0 ) {
	    *rval = &cur_entry->value;
	    return 0;
	  }
	}
   }
   *rval = 0;
   return -1;
}
     
int VALHASH_iterate( VALHASH *hash, union tagBINDING_DATA **key, union tagBINDING_DATA **value, VALHASHPOS *pos )
{
  HASH_VALUE_ENTRY *entry;

  if (!pos->bucket) {
    pos->bucket = hash->buckets;
    pos->bucket_pos = hash->buckets;
  }

  while(1) {
    if ( (pos->bucket_pos = SRING_get_next( pos->bucket, pos->bucket_pos )) != 0) {
      entry = (HASH_VALUE_ENTRY *) pos->bucket_pos;

      *key = &entry->key;
      *value = &entry->value;
	    
      return 1;
    } 

    pos->bucket_pos = ++ pos->bucket;

    if (pos->bucket == hash->buckets + hash->numbuckets) {
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
    ret ^= BINDING_DATA_hash( key );
    ret ^= BINDING_DATA_hash( value );
  }
  return ret;
}

int VALHASH_sort_keys_cb( const void *ka, const void *kb )
{
  BINDING_DATA *keya, *keyb; 

  keya = * ((BINDING_DATA **) ka);
  keyb = * ((BINDING_DATA **) kb);

  return BINDING_DATA_cmp( keya, keyb );
}

void VALHASH_make_sorted_keys( VALHASH *hash )
{ 
  VALHASHPOS pos;
  size_t i;
  BINDING_DATA *key,*value;
  hash->ordered_keys = (BINDING_DATA **) HEAP_alloc( VALHASH_size( hash ) * sizeof( void *) );
  
  VALHASHPOS_init( &pos );

  for( i = 0;  VALHASH_iterate( hash, &key, &value, &pos ); i++ ) {
    hash->ordered_keys[ i ] =  key;
  }

  qsort( hash->ordered_keys, VALHASH_size( hash ), sizeof(void *),  VALHASH_sort_keys_cb );
}


int VALHASH_equal( VALHASH *cmpa, VALHASH *cmpb )
{
   size_t sza = VALHASH_size( cmpa );
   size_t szb = VALHASH_size( cmpb );
   size_t i;
   BINDING_DATA *bnda,*bndb;
   int rt;
   
   if (sza != szb) {
      return -1;
   }

   if (cmpa->ordered_keys == 0) {
     VALHASH_make_sorted_keys( cmpa );
   }
   if (cmpb->ordered_keys == 0) {
     VALHASH_make_sorted_keys( cmpb );
   }

   for( i = 0; i < sza; i++ ) {
      bnda = cmpa->ordered_keys[ i ];
      bndb = cmpb->ordered_keys[ i ];

      rt = BINDING_DATA_cmp( bnda, bndb );
      if (rt != 0) {
        return rt;
      }
   }
   return 0;
}

void VALHASH_print( FILE *out, VALHASH *data, int level  )
{
   VALHASHPOS pos;
   BINDING_DATA *key,*value;
   int i;

   VALHASHPOS_init( &pos );

   fprintf(out,"{ ");
   
   for( i = 0 ; VALHASH_iterate( data, &key, &value, &pos ) ; i++ ) {
      if (i > 0) {
         fprintf( out, ", " );
      }

      assert( key->b.container == _OFFSETOF( data, BINDING_DATA_VALUE, value ) );
      assert( value->b.container == _OFFSETOF( data, BINDING_DATA_VALUE, value ) );

      BINDING_DATA_print( out, key, level + 1 );
      fprintf( out, " : " );
      BINDING_DATA_print( out, value, level + 1 );
   
   }
   
   fprintf(out," }");
}

void VALHASH_check_ref( VALHASH *hash )
{
   VALHASHPOS pos;
   BINDING_DATA *key,*value;
 
   VALHASHPOS_init( &pos );
   for( ; VALHASH_iterate( hash, &key, &value, &pos ) ; ) {
     BINDING_DATA_check_ref( key );
     BINDING_DATA_check_ref( value );
   }
}

/* ==================================================================================== */

int  VALSTRING_append( VALSTRING *to, VALSTRING *from )
{
  VALSTRING_set_capacity( to, to->length + from->length ); 
  memcpy( to->string + to->length, from->string, from->length ); 
  to->length += from->length;
  return 0;
}

int  VALSTRING_appends( VALSTRING *to, const char *from )
{
  size_t from_len = strlen( from );
  VALSTRING_set_capacity( to, to->length + from_len ); 
  memcpy( to->string + to->length, from, from_len ); 
  to->length += from_len;
  return 0;
}


void VALSTRING_set_capacity( VALSTRING *to, size_t capacity )
{
  if (to->capacity < capacity) {
    capacity += STRING_GROW_CAPACITY_BY; 
    HEAP_realloc( (void **) &to->string, capacity );
    to->capacity = capacity;
  }
}

void VALSTRING_set(VALSTRING *string, const char *ptr, size_t length)
{
  VALSTRING_set_capacity( string, length );
  memcpy( string->string, ptr, length );
  string->length = length;
}

void VALSTRING_free(VALSTRING *string )
{
  if (string->string) {
    free(string->string);
  }
  VALSTRING_init( string );  
}

int VALSTRING_copy( VALSTRING *to, VALSTRING *from  )
{
  VALSTRING_set_capacity( to, from->capacity );
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

int VALSTRING_substr( VALSTRING *to, VALSTRING *from, size_t offset, size_t length)
{

  offset = from->length < offset ? from->length : offset;

  if ((offset + length) > from->length) {
    length = from->length - offset;
  }

  VALSTRING_set( to, from->string + offset, length ); 
  return 0;
}

int  VALSTRING_cmp( VALSTRING *argA, VALSTRING *argB)
{
  return strcmp( argA->string != 0 ? argA->string : "", argB->string != 0 ? argB->string : "" );
}
#include <cutils/hashfunction.h>

void VALSTRING_print( FILE *out, VALSTRING *data )
{

  // ??? null terminator
  fprintf( out, "\"%.*s\"", data->length, data->string );
}

size_t VALSTRING_hash( VALSTRING *string )
{
   return HASHFUNCTION_Bob_Jenkins_one_at_a_time( string->string, string->length );
}

/* ==================================================================================== */

int VALFUNCTION_capture_equal( ARRAY *cmpa, ARRAY *cmpb )
{
    VALFUNCTION_CAPTURE *ca,*cb;
    size_t sza = ARRAY_size( cmpa );
    size_t szb = ARRAY_size( cmpb );
    size_t i;

    if (sza != szb ) {
	return -1;
    }	    

   
    for( i = 0; i < sza; i++ ) {
	ca = (VALFUNCTION_CAPTURE *) ARRAY_at( cmpa, i );
	cb = (VALFUNCTION_CAPTURE *) ARRAY_at( cmpb, i );
	
	if (ca->next.next != cb->next.next || ca->value.ref != cb->value.ref) {
	    return -1;
	}
    }
    return 0;
}
int  VALFUNCTION_equal( VALFUNCTION *cmpa, VALFUNCTION *cmpb)
{
  if (cmpa == cmpb) {
    return 0;
  }

  if (cmpa->fdecl == cmpb->fdecl) {
      if (cmpa->this_environment == cmpb->this_environment) {
          if ( VALFUNCTION_capture_equal( &cmpa->captures, &cmpb->captures ) ) {
	     return 0;
	   }
      }
  } 
  return -1;
}	
  

void VALFUNCTION_init_cap( VALFUNCTION *func, size_t num_captures )
{
  func->fdecl = 0;
  func->this_environment = 0;

  if (ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), num_captures )) {
    EVAL_CONTEXT_gc( g_context );
    if (ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), num_captures )) {
       EVAL_CONTEXT_runtime_error( g_context, "out of memory");
    }
  } 
}

void VALFUNCTION_init_outer_refs( struct tagEVAL_THREAD *cthread, VALFUNCTION *fnc, AST_FUNC_DECL *fdecl )
{
  AST_VECTOR *refs;
  VALFUNCTION_CAPTURE *cap_entry;
  size_t i, num_outer_refs;
  BINDING_DATA *data;
  BINDING_ENTRY *entry;
  AST_EXPRESSION *expr;

  refs = &fdecl->outer_refs;
  num_outer_refs = AST_VECTOR_size( refs );
  VALFUNCTION_init_cap( fnc, num_outer_refs );

  for( i = 0; i < num_outer_refs; i++ ) {
    expr = (AST_EXPRESSION *) AST_VECTOR_get( refs, i );
  
    cap_entry  = (VALFUNCTION_CAPTURE *) ARRAY_at( &fnc->captures, i );

    entry = expr->val.ref.binding;
    assert( entry );
    data = EVAL_THREAD_stack_frame_offset( cthread, entry->stack_offset ); 
    assert( data );

    VALFUNCTION_make_capture( fnc, data, 0 /* ??? ddd */  );
  }
}


// create function object for each level of closures / walk tree of functions
void VALFUNCTION_init_outer_ref_tree( struct tagEVAL_THREAD *cthread, VALFUNCTION *fnc, AST_FUNC_DECL *fdecl )
{
    TREENODE *node;
    BINDING_DATA *nested;   
    
    VALFUNCTION_init_outer_refs( cthread, fnc, fdecl );

    for( node = TREE_first_child( &fdecl->funcs ); (node = TREE_right_sibling( node )) != 0;  ) {
	
        nested = BINDING_DATA_MEM_new( S_VAR_CODE ); 
	fdecl = _OFFSETOF(  nested,  AST_FUNC_DECL, funcs );
	VALFUNCTION_init_outer_ref_tree( cthread, &nested->b.value.func_value, fdecl );

	ARRAY_push_back( &fnc->nested_closures, &nested, sizeof(BINDING_DATA *) );
    }
}


void make_capture( VALFUNCTION_CAPTURE *pentry, union tagBINDING_DATA *data, int data_entry )
{
   if  ( ! IS_STACK_VALUE( data ) )  {
 
      // capture entry points to heap reference.
      pentry->next.next = 0;
      pentry->value.ref = data; 
      
   } else { 
   
      // reference to stack location ; make forwared reference to it.
      SRING *first = (SRING *) data->b.container;

      pentry->value.ref = data;

      SRING_push_front( first, &pentry->next );
   }

   if (data_entry) {
      * ((size_t *) pentry->next.next) |= 1;
   }
}

void VALFUNCTION_make_capture( VALFUNCTION *func, union tagBINDING_DATA *data, int data_entry )
{
   VALFUNCTION_CAPTURE *pentry;

   data = BINDING_DATA_follow_ref( data );
  
   func->captures.elmcount ++;
   assert( func->captures.elmcount < func->captures.elmmaxcount );
      
   pentry = (VALFUNCTION_CAPTURE *) ARRAY_at( &func->captures, ARRAY_size( &func->captures ) - 1 );
 
   make_capture( pentry, data, data_entry );
}

int VALFUNCTION_mark_as_thread( VALFUNCTION *func )
{
   VALFUNCTION_CAPTURE entry;
   memset( &entry, 0, sizeof(VALFUNCTION_CAPTURE ));
   ARRAY_push_back( &func->captures, &entry, sizeof(VALFUNCTION_CAPTURE ) );
   return 0;
}

struct tagEVAL_THREAD *VALFUNCTION_thread( VALFUNCTION *func )
{
   VALFUNCTION_CAPTURE *entry;

   entry = (VALFUNCTION_CAPTURE *) ARRAY_at( &func->captures, ARRAY_size( &func->captures ) - 1 );  
   if (!entry) {
     return 0;
   }
   return entry->value.thread;
}

void  VALFUNCTION_print( FILE *out, VALFUNCTION *func )
{
   AST_FUNC_DECL *fdecl;    
   AST_XFUNC_DECL *fxdecl;
   AST_EXPRESSION *param_decl;
   size_t i;

   if (func->fdecl->type == S_FUN_DECL) {

     fdecl = (AST_FUNC_DECL *) func->fdecl;

     fprintf( out, "%s(", fdecl->f_name );

     for( i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
       if (i > 0) {
         fprintf(out, " , " );
       }

       param_decl = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
      
       fprintf( out, "%s", param_decl->val.ref.lhs );
     }
   } else {
     assert( func->fdecl->type == S_XFUN_DECL );
     fxdecl = (AST_XFUNC_DECL *) func->fdecl;

     fprintf( out, "%s(", fxdecl->f_name );

   }  
   fprintf( out, ")" );
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
      VALSTRING_init( &binding->b.value.string_value );
      break;
    case S_VAR_CODE:
      VALFUNCTION_init( &binding->b.value.func_value );
      break;
    case S_VAR_HASH:
      VALHASH_init( &binding->b.value.hash_value );
      break;
    case S_VAR_LIST:
      VALARRAY_init( &binding->b.value.array_value ); 
      break;
    case S_VAR_NULL:     
      break;
    default: 
      assert(0);
      break;
   }
}

BINDING_DATA *BINDING_DATA_follow_ref(  BINDING_DATA *arg )
{
    short value_flags;     
    BINDING_DATA *binding,*arecord;
    size_t cap_idx,pos;
    VALFUNCTION_CAPTURE *cap;
    VALFUNCTION *fobject;  

    for( binding = arg ; ;  ) {
      value_flags = binding->b.value_flags;

      if ( (value_flags & ( S_VAR_REF_HEAP | S_VAR_REF_HEAP2STACK | S_VAR_REF_STACK2STACK | S_VAR_CAPTURE_REF ) ) == 0) {
        return binding;
      }

      if (value_flags & S_VAR_REF_HEAP) {
        binding = binding->b.value.value_ref;
      } else if (value_flags & S_VAR_REF_HEAP2STACK) {
        pos = binding->b.value.heap2stack_ref.value.stack_ref;
	assert( pos < ARRAY_size( &g_cur_thread->binding_data_stack ) );
	binding= ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + pos;
      } else if (value_flags & S_VAR_REF_STACK2STACK) { 
	pos = binding->b.value.stack2stack_ref;
	assert( pos < ARRAY_size( &g_cur_thread->binding_data_stack ) );
	binding = ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + pos;
      } else if (value_flags & S_VAR_CAPTURE_REF) {
        arecord = ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + g_cur_thread->current_function_frame_start + 1;
        cap_idx = binding->b.value.capture_ref;	

	fobject = arecord->activation_record.function_object;
	if (!fobject) {
	    EVAL_CONTEXT_runtime_error( g_context, "Cant access capture from function without function object (go figure)");
	}

	cap = (VALFUNCTION_CAPTURE *) ARRAY_at( &fobject->captures, cap_idx );

	if (cap->next.next == 0) {
	  binding = cap->value.ref;
	} else {
	  pos = cap->value.stack_ref;
	  binding = ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + pos;
        } 
      } 
#ifndef NDEBUG      
      else {
        assert(0);
      }
#endif   
     assert(binding != 0);

   }
   // never gets here.
   return 0;
}

void BINDING_DATA_move( BINDING_DATA *to, BINDING_DATA *from)
{
  memcpy( to, from, sizeof( BINDING_DATA ) );
  BINDING_DATA_init( from, S_VAR_NULL );
  to->b.value_flags &= ~( S_VAR_COLL_ENTRY );
}

void BINDING_DATA_return_value( BINDING_DATA *ret_slot, BINDING_DATA *rvalue  )
{
   if (IS_REF( rvalue )) {
      // if reference is to stack value
      if (rvalue->b.value_flags & (S_VAR_REF_HEAP | S_VAR_REF_STACK2STACK)) {
	BINDING_DATA_move( ret_slot, rvalue);
	return;
      }
      
      // capture is copied by value.
      BINDING_DATA_copy( ret_slot, rvalue, 1 );
      return;
   }
   BINDING_DATA_move( ret_slot, rvalue );
}


void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from, CP_KIND copy_by_value  )
{
   BINDING_DATA_free( to );
   size_t pos;

   if ( IS_REF( from ) ) {
     memcpy( to, from, sizeof( BINDING_DATA ) );

     if (from->b.value_flags & S_VAR_REF_HEAP2STACK) {
        SRING_insert_after( &from->b.value.heap2stack_ref.next, &to->b.value.heap2stack_ref.next );
     }
     return;
   }

   if ( ! copy_by_value ) {
     
     to->b.value_type = 0;
     if (IS_STACK_VALUE( from ) ) {
	
	 pos =  from - ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer); 
	 if (IS_STACK_VALUE( to )) {
	    to->b.value_flags = S_VAR_REF_STACK2STACK;
	    to->b.value.stack2stack_ref = pos;
	 } else {
	    to->b.value_flags = S_VAR_REF_HEAP2STACK;
	    make_capture(  &to->b.value.heap2stack_ref, from, 1 );
	 }
     } else if (from->b.value_flags & (S_VAR_HEAP_VALUE | S_VAR_COLL_ENTRY) ) {
        to->b.value_flags = S_VAR_REF_HEAP;
	to->b.value.value_ref = from;
     } else {
	assert(0);
     }	
     return;
   }
  
 
   to->b.value_flags = 0;
   to->b.value_type = from->b.value_type;
   switch( from->b.value_type ) {
    case S_VAR_INT:
      to->b.value.long_value = from->b.value.long_value;
      break;
    case S_VAR_DOUBLE:
      to->b.value.double_value = to->b.value.double_value;
      break;
    case S_VAR_STRING:
      VALSTRING_init( &to->b.value.string_value );
      VALSTRING_copy( &to->b.value.string_value, &from->b.value.string_value );
      break;
    case S_VAR_CODE:
      // Can funtion object be copied at all ?
      break;
    case S_VAR_HASH:
      VALHASH_init( &to->b.value.hash_value );
      VALHASH_copy( &to->b.value.hash_value, &from->b.value.hash_value, 1 );
      break;
    case S_VAR_LIST:
      VALARRAY_init( &to->b.value.array_value );
      VALARRAY_copy( &to->b.value.array_value, &from->b.value.array_value, 1 );
      break;
    default:
      assert(0);
      break;
   }
 }

// a captured variable points to a local; this local goes out of scope; 
// Now the local is 
//	- moved to the gc heap.
//	- all capture reference now point to the heap entry. More then one reference may be linked up in list.
void BINDING_DATA_release_captures( BINDING_DATA *binding )
{
   SRING *ring = (SRING *) binding->b.container;
   SRING *cur, *next;
   BINDING_DATA *mem;
   VALFUNCTION_CAPTURE *capture_cur;
   BINDING_DATA_VALUE *bdata;

   mem = BINDING_DATA_MEM_new( binding->b.value_type );

#if 0
   SRING_FOR_SAVE( cur, next, ring ) {
     capture_cur = (VALFUNCTION_CAPTURE *) cur;
 
     capture->ref = mem;
     capture->next.next = 0;
   }
#endif

#define CLEAR_LOW_BIT( ptr )  ( (SRING *) ( * ( (size_t *) ptr ) & ~1 ) )
#define PTR_LOW_BIT_SET( ptr ) ( * ( (size_t * ) ptr ) & 1  )

   for( cur = ring->next, next = CLEAR_LOW_BIT( cur->next ) ;  
	cur != ring;  
	cur = next , next = CLEAR_LOW_BIT( next->next ) ) {

     capture_cur = (VALFUNCTION_CAPTURE *) cur;
    
     capture_cur->value.ref = mem;
     capture_cur->next.next = 0;

     if (PTR_LOW_BIT_SET( cur ) ) {
	bdata = _OFFSETOF( cur , BINDING_DATA_VALUE, value ); 		
	bdata->value_flags = S_VAR_REF_HEAP;
     }
   }
}

void BINDING_DATA_free( BINDING_DATA *binding )
{ 
   // if this is a pointer to a stack value and pointers to captures exist - then move this value to the stack and adjust capture reference to point to new heap entry.
   if (IS_STACK_VALUE( binding ) && binding->b.container != 0 ) {
     BINDING_DATA_release_captures( binding );
     BINDING_DATA_init( binding, S_VAR_NULL );
     return;
   }
   
   
   switch( binding->b.value_type ) {
    case S_VAR_STRING:
      VALSTRING_free( &binding->b.value.string_value );
      break;
    case S_VAR_CODE:
      break;
    case S_VAR_HASH:
      VALHASH_free( &binding->b.value.hash_value );
      break;
    case S_VAR_LIST:
      VALARRAY_free( &binding->b.value.array_value );
      break;
    default:
      break;
   }
   BINDING_DATA_init( binding, S_VAR_NULL );
}

size_t BINDING_DATA_hash( BINDING_DATA *binding )
{
   uint32_t *uptr;

   switch( binding->b.value_type ) {
    case S_VAR_INT:
      return (size_t) binding->b.value.long_value;
    case S_VAR_DOUBLE:
      uptr = (uint32_t *) &binding->b.value.double_value;
      return uptr[0] ^ uptr[1];
    case S_VAR_STRING:
      return VALSTRING_hash( &binding->b.value.string_value );
    case S_VAR_CODE:
      return (size_t) binding->b.value.func_value.fdecl;
    case S_VAR_HASH:
      return VALHASH_hash( &binding->b.value.hash_value );
    case S_VAR_LIST:
      return VALARRAY_hash( &binding->b.value.array_value );
    default:
      return 0;
   }
 }


#if 0
  double numva, numvb;

  if (cmpa->b.value_type != cmpb->b.value_type) {
      if ( ! IS_NUMBER( cmpa ) ||  ! IS_NUMBER( cmpb ) ) {
	  EVAL_CONTEXT_runtime_error( g_context, "Can't compare %s to %s", get_type_name( cmpa->b.value_type ), get_type_name( cmpb->b.value_type ) );
      }
      
      numva = cmpa->b.value_type == S_VAR_INT ?  cmpa->b.value.long_value : cmpa->b.value.double_value;
      numvb = cmpb->b.value_type == S_VAR_INT ?  cmpb->b.value.long_value : cmpb->b.value.double_value;

      if (numva < numvb) { 
        return -1;
      }
      if (numva > numvb) {
        return 1;
      }
      return 0;
  } 
#endif


int  BINDING_DATA_cmp( BINDING_DATA *cmpa, BINDING_DATA *cmpb )
{
  if (cmpa->b.value_type < cmpb->b.value_type) {
    return -1;
  }
  if (cmpa->b.value_type > cmpb->b.value_type) {
    return 1;
  }

  // both values of the same type, can compare them.
  switch( cmpa->b.value_type ) {
    case S_VAR_INT:
      if (cmpa->b.value.long_value == cmpb->b.value.long_value ) {
        return -1;
      }
      break;

    case S_VAR_DOUBLE:
      if (cmpa->b.value.double_value == cmpb->b.value.double_value ) {
        return 0;
      }
      break;

    case S_VAR_STRING:
      if (VALSTRING_cmp( &cmpa->b.value.string_value, &cmpb->b.value.string_value ) == 0) {
        return 0;
      }
      break;
    
    case S_VAR_CODE:
      if (VALFUNCTION_equal( &cmpa->b.value.func_value, &cmpb->b.value.func_value ) == 0) {
        return 0;
      }
      break;

    case S_VAR_HASH:
      if (VALHASH_equal( &cmpa->b.value.hash_value, &cmpb->b.value.hash_value ) == 0) {
        return 0;
      }
      break;
    
    case S_VAR_LIST:
      if (VALARRAY_equal( &cmpa->b.value.array_value, &cmpb->b.value.array_value ) == 0) {
        return 0;
      }
      break;

    default:  
      EVAL_CONTEXT_runtime_error( g_context, "Invalid type %d", cmpa->b.value_type );
   }

   if (cmpa < cmpb) {
     return -1;
   }
   return 1;
}

int BINDING_DATA_check_ref( BINDING_DATA *data )
{
  BINDING_DATA *ref;

  if (data->b.value_flags & S_VAR_PRINT_CHECK_LOOPS) {
    return 0;
  }
  data->b.value_flags |= S_VAR_PRINT_CHECK_LOOPS;	 
  
#if 0  
  if (data->b.container != 0) {
    BINDING_DATA_check_ref( data->b.container );
    // ?? what to do about captures ???
  }
#endif  


  switch( data->b.value_type ) {
    case S_VAR_HASH:
        VALHASH_check_ref( &data->b.value.hash_value );
	break;
    case S_VAR_LIST:
	VALARRAY_check_ref( &data->b.value.array_value );
	break;
    default:
      if (IS_REF(data)) {
         ref = BINDING_DATA_follow_ref( data );

         if (ref->b.value_flags & S_VAR_PRINT_CHECK_LOOPS || 
	     ref->b.value_flags & S_VAR_COLL_ENTRY) {
           EVAL_THREAD_print_ref_add_loop( data, ref );
	 }
	 BINDING_DATA_check_ref( ref );
      }
      break;
  }
  return 0;
}

void BINDING_DATA_print_check_loops( BINDING_DATA *data )
{
  // mark all refrences
  BINDING_DATA_check_ref( data );
  g_cur_thread->has_loops = HASH_size( &g_cur_thread->print_check_ref_loops );
}

int BINDING_DATA_print_ref( FILE *out, BINDING_DATA *data, int prefix) 
{
   PRINT_CHECK_LOOP *cloop;

   if (g_cur_thread->has_loops != 0) {
       // check if reference is circular
       //
       cloop = (PRINT_CHECK_LOOP *) HASH_find( &g_cur_thread->print_check_ref_loops, data, sizeof(void *) ); 
       if (cloop != 0) {
         g_cur_thread->has_loops --;
	 if (cloop->to == 0) {
	     fprintf(out, "<%p> ", cloop->key );
	  } else {
	    fprintf( out, "%s(%p)", (prefix ? "->" : "") , cloop->to );
	    return 1;
	 }  
      }
   }
   return 0;
}



void BINDING_DATA_print( FILE *out, BINDING_DATA *data , int level )
{
  int free_check_loops = 0;


  // ??? self eferential structures - what is the format to print them ?;
  if (level == 0) {
     g_cur_thread->has_loops = 0;
     if (data->b.value_type == S_VAR_HASH || data->b.value_type == S_VAR_LIST || IS_REF(data)) {
       BINDING_DATA_print_check_loops( data );
       free_check_loops = 1;
     }
  }

  data->b.value_flags &= ~S_VAR_PRINT_CHECK_LOOPS;

  if (BINDING_DATA_print_ref( out, data, 1 )) {
    return;
  }
  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
    assert( data != 0 );
 
     fprintf(out, "-> ");
     if (BINDING_DATA_print_ref( out, data, 0 )) {
      return;
    }  
  }
 
  switch( data->b.value_type ) {
    case S_VAR_INT:
        fprintf( out, "%ld", data->b.value.long_value );
	break;
    case S_VAR_DOUBLE:
        fprintf( out, "%f", data->b.value.double_value );
	break;
    case S_VAR_STRING:
        VALSTRING_print( out, &data->b.value.string_value  );
	break;
    case S_VAR_CODE:
        VALFUNCTION_print( out, &data->b.value.func_value  );
	break;
    case S_VAR_HASH:
        VALHASH_print( out, &data->b.value.hash_value, level + 1  );
	break;
    case S_VAR_LIST:
	VALARRAY_print( out, &data->b.value.array_value, level + 1  );
	break;
    case S_VAR_NULL:
        fprintf( out, "Nill" );
        break;
    default:
	assert(0);
	break;
  }

  if (free_check_loops) {
    EVAL_THREAD_clear_check_ref( g_cur_thread );
  }
}


static ssize_t dbuf_cookie_write( void *cookie, const char *buf, size_t size )
{
  DBUF *out = (DBUF *) cookie;

  DBUF_add(  out, buf, size );
  return size;
}

void BINDING_DATA_prints( DBUF *out, BINDING_DATA *data , int level )
{
  _IO_cookie_io_functions_t impl;
  FILE *fp;
  
  memset( &impl, 0, sizeof( _IO_cookie_io_functions_t ) );

  impl.write = dbuf_cookie_write;

  fp = fopencookie( out, "w", impl );
  BINDING_DATA_print( fp, data, level );
  fclose(fp);
  
//DBUF_add_null( out );
}


/* ==================================================================================== */

BINDING_DATA *BINDING_DATA_MEM_new(  AST_VAR_TYPE value_type ) 
{
  BINDING_DATA_MEM *mem;

  mem = (BINDING_DATA_MEM *) HEAP_alloc( sizeof( BINDING_DATA_MEM ) );

  DRING_push_back( &g_context->gc_heap, &mem->heap_entry );
 
  BINDING_DATA_init( &mem->data, value_type );

  mem->data.b.value_flags = S_VAR_HEAP_VALUE;

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
static int check_loop_cmp( HASH_Entry *entry, const void *key, ssize_t key_len)
{
   PRINT_CHECK_LOOP *lhs = (PRINT_CHECK_LOOP *) entry;
   
   (void) key_len;

   if (lhs->key == (void *) key) {
     return 0;
   }
   return 1;
}

static uint32_t check_loop_hash( const void *key, ssize_t klen)
{
  (void) klen;
  return (uint32_t) key;
}

int EVAL_THREAD_init(EVAL_THREAD *thread, EVAL_CONTEXT *context )
{
   VALACTIVATION *activation_record;

   if (ARRAY_init( &thread->binding_data_stack, sizeof(BINDING_DATA), 10 ) ) {
     EVAL_CONTEXT_runtime_error( g_context, "out of memory");
     return -1;
   }
   thread->context = context;
  
   thread->tstate = TSTATE_IDLE;
   DRING_push_back( &context->threads, &thread->threads );
 
   if (HASH_init( &thread->print_check_ref_loops, 10, 0, check_loop_cmp, check_loop_hash )) {
     return -1;
   }
   activation_record =  (VALACTIVATION *) thread->binding_data_stack.buffer;
   memset( activation_record, 0, sizeof( VALACTIVATION ));
   activation_record->function_frame_start = 1;
   activation_record->parent_function = (size_t) -1;
   thread->current_activation_record = 0;
   thread->current_function_frame_start = 1;
   thread->binding_data_stack.elmcount = 1;
   thread->instr = 0;
   return 0;
}

void free_binding_array( ARRAY *array )
{
  size_t i;

  for( i = 0; i < ARRAY_size( array ); i++ ) {
     BINDING_DATA_free( (BINDING_DATA *) ARRAY_at( array, i ) );
  } 
 
}

void EVAL_THREAD_free(EVAL_THREAD *thread)
{
  free_binding_array( &thread->binding_data_stack );
 
  ARRAY_free( &thread->binding_data_stack );
  DRING_unlink( &thread->threads );
}

void EVAL_THREAD_print_ref_add_loop( BINDING_DATA *from, BINDING_DATA *to)
{
  PRINT_CHECK_LOOP *cloop;

//fprintf(stderr,"<%p-%p> ", from, to );

  // circular reference has been found.
  cloop = (PRINT_CHECK_LOOP *) HEAP_alloc( sizeof( PRINT_CHECK_LOOP ) );
  cloop->key = from;
  cloop->to = to;
    
  HASH_insert( &g_cur_thread->print_check_ref_loops, &cloop->entry, cloop->key, sizeof(void *) ); 
 
  cloop = (PRINT_CHECK_LOOP *) HEAP_alloc( sizeof( PRINT_CHECK_LOOP ) );
  cloop->key = to;
  cloop->to = 0;

  HASH_insert( &g_cur_thread->print_check_ref_loops, &cloop->entry, cloop->key, sizeof(void *) ); 
}

void EVAL_THREAD_clear_check_ref( EVAL_THREAD *thread )
{  
   HASH_Entry *cur;
   
   HASH_DELETEALL( cur, &thread->print_check_ref_loops )
     free( cur );
   HASH_DELETEALL_END
}

#define FRAME_START_SIZE 2

void EVAL_THREAD_reserve_stack( EVAL_THREAD *thread, size_t frame_size )
{
  if ((thread->binding_data_stack.elmcount + frame_size ) >  thread->binding_data_stack.elmmaxcount) {
    if (ARRAY_resize( &thread->binding_data_stack, 2 * (thread->binding_data_stack.elmcount + frame_size) ) ) {
       EVAL_CONTEXT_gc( g_context );
       if (ARRAY_resize( &thread->binding_data_stack, 2 * (thread->binding_data_stack.elmcount + frame_size) ) ) {
	  EVAL_CONTEXT_runtime_error( g_context, "out of memory");
       }
    }
  }
}

void EVAL_THREAD_make_activation_record( EVAL_THREAD *thread, AST_BASE *fdecl, VALFUNCTION *function, size_t function_frame_start )
{
  VALACTIVATION *activation_record;

  activation_record =  (VALACTIVATION *) ( ((BINDING_DATA *) thread->binding_data_stack.buffer) + function_frame_start + 1);
  activation_record->fdecl = fdecl;
  activation_record->function_object = function;
  activation_record->parent_function = thread->current_activation_record;
  activation_record->function_frame_start = function_frame_start;
  activation_record->ret_instr = thread->instr;

  thread->current_activation_record = function_frame_start + 1;
}

size_t EVAL_THREAD_prepare_xcall( EVAL_THREAD *thread, AST_XFUNC_DECL *decl )
{
  size_t nsize;
  size_t i;

  // make room for this function call
   EVAL_THREAD_reserve_stack( thread, FRAME_START_SIZE + decl->nparams );
 
  nsize = ARRAY_size( &thread->binding_data_stack );
  
  // allocate stack slot for return value.
  BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize , S_VAR_NULL );

  // set all arguments to NULL.
  for(i = 0; i < decl->nparams; i++ ) {
    BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize + FRAME_START_SIZE + i , S_VAR_NULL );
  }
  thread->binding_data_stack.elmcount += FRAME_START_SIZE + decl->nparams;

  return nsize;
}

size_t EVAL_THREAD_prepare_call( EVAL_THREAD *thread, AST_FUNC_DECL *decl )
{
  size_t nsize, nparams = AST_VECTOR_size( decl->func_params ) ;
  size_t frame_size = FRAME_START_SIZE + nparams + HASH_size( &decl->scope_map_name_to_binding );
 size_t i;


  // make room for this function call
  EVAL_THREAD_reserve_stack( thread, frame_size );

  nsize = ARRAY_size( &thread->binding_data_stack );

  // allocate stack slot for return value.
  BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize , S_VAR_NULL );
 
  // set all arguments to NULL.
  for(i = 0; i < nparams; i++ ) {
    BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize + FRAME_START_SIZE + i , S_VAR_NULL );
  }
  thread->binding_data_stack.elmcount += FRAME_START_SIZE + nparams;

  return nsize;
}

BINDING_DATA *EVAL_THREAD_parameter( EVAL_THREAD *thread, size_t frame_start, size_t param_num, AST_VAR_TYPE type )
{ 
  BINDING_DATA *rval;
  size_t idx = frame_start + param_num + FRAME_START_SIZE;
  rval = ((BINDING_DATA *) thread->binding_data_stack.buffer) + idx;
  BINDING_DATA_init( rval, type );
  return rval;
}

BINDING_DATA *EVAL_THREAD_parameter_ref( EVAL_THREAD *thread, size_t frame_start, size_t param_num )
{ 
  BINDING_DATA *rval;
  size_t idx = frame_start + param_num + FRAME_START_SIZE;
  rval = ((BINDING_DATA *) thread->binding_data_stack.buffer) + idx;
  return rval;
}


void EVAL_THREAD_call_func( EVAL_THREAD *thread, size_t frame_start, AST_FUNC_DECL *decl, VALFUNCTION *function )
{
  size_t  i, nsize, ncount;

  nsize = ARRAY_size( &thread->binding_data_stack );
  assert( nsize == frame_start + FRAME_START_SIZE + AST_VECTOR_size( decl->func_params ) );


  // make activation record.
  EVAL_THREAD_make_activation_record( thread, &decl->base, function, frame_start );
  

  // init local variables.
  ncount = HASH_size( &decl->scope_map_name_to_binding );
  for( i = 0; i < ncount; i++) {
    BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize + i + 1 , S_VAR_NULL );
  }

  thread->current_function_frame_start = frame_start;
  thread->binding_data_stack.elmcount += ncount + 1; 

}


void EVAL_THREAD_call_xfunc( EVAL_THREAD *thread, size_t frame_start, AST_XFUNC_DECL *decl )
{
   size_t nsize;
   XCALL_DATA xdata;
   
   nsize = ARRAY_size( &thread->binding_data_stack );
   assert( nsize == frame_start + FRAME_START_SIZE + decl->nparams );


   EVAL_THREAD_make_activation_record( thread, &decl->base, 0, frame_start );
   
   thread->current_function_frame_start = frame_start;
   thread->binding_data_stack.elmcount += 1;
 

   xdata.thread = thread;
   xdata.frame_offset = frame_start;
   xdata.num_arguments = decl->nparams;
   
   decl->xcall( &xdata );

   EVAL_THREAD_pop_frame ( thread );
}


int EVAL_THREAD_pop_frame ( EVAL_THREAD *thread )
{
  size_t i, nsize, top;
  VALACTIVATION *activation_record,*pframe;
  size_t parent_activation_record;
  BINDING_DATA *data;

  activation_record = (VALACTIVATION *) EVAL_THREAD_stack_offset( thread, thread->current_activation_record );
  if (!activation_record) {
    return -1;
  }
  
  nsize = ARRAY_size( &thread->binding_data_stack );
  top = thread->current_function_frame_start;  
  // clear all arguments + local variables.
  for( i = top + 2; i < nsize ; i++ ) {
     data = ((BINDING_DATA *) thread->binding_data_stack.buffer) + i; 
     BINDING_DATA_free( data ); 
  }
  
  parent_activation_record = activation_record->parent_function;
  if (parent_activation_record == (size_t) -1) {
    return 1;
  }
  thread->current_activation_record = parent_activation_record;

  pframe = (VALACTIVATION *) EVAL_THREAD_stack_offset( thread, parent_activation_record );
 
  thread->instr = pframe->ret_instr;
  thread->binding_data_stack.elmcount = top + 1;
  thread->current_function_frame_start = pframe->function_frame_start;
 
  return 0;
}

BINDING_DATA *EVAL_THREAD_get_stack_top( EVAL_THREAD *thread )
{
   BINDING_DATA *data;
   size_t nsize;

   nsize = ARRAY_size( &thread->binding_data_stack ); 
   data = (BINDING_DATA *) ARRAY_at( &thread->binding_data_stack, nsize - 1 );
   assert( data != 0);
   return data;
}

BINDING_DATA *EVAL_THREAD_stack_frame_offset( EVAL_THREAD *thread, size_t stack_offset )
{
   size_t pos; 

   pos = thread->current_function_frame_start + stack_offset; 

   if (thread->current_function_frame_start == (size_t) -1 || pos > ARRAY_size( &thread->binding_data_stack )) {
      assert(0);
   }
   BINDING_DATA *data = (BINDING_DATA *) ARRAY_at( &thread->binding_data_stack, pos ); //ARRAY_size( &thread->binding_data_stack ) - 1 - stack_offset );
   assert( data != 0);
   return data;
}

BINDING_DATA *EVAL_THREAD_pop_stack( EVAL_THREAD *thread )
{
  size_t nsize;
  BINDING_DATA *rvalue;
 
  nsize = ARRAY_size( &thread->binding_data_stack );
  rvalue = ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize - 1;
  thread->binding_data_stack.elmcount -= 1;

  return rvalue;
}

BINDING_DATA *EVAL_THREAD_push_stack( EVAL_THREAD *thread , AST_VAR_TYPE vtype)
{
  BINDING_DATA *binding;

  if (thread->binding_data_stack.elmcount == thread->binding_data_stack.elmmaxcount) {
  // EVAL_THREAD_reserve_stack( thread, ARRAY_size( & thread->binding_data_stack ) + 10 );
     EVAL_THREAD_reserve_stack( thread, 50 );
   }
  binding = ((BINDING_DATA *) thread->binding_data_stack.buffer) + thread->binding_data_stack.elmcount ++;
  BINDING_DATA_init( binding, vtype );
  return binding;
}



int EVAL_THREAD_print_stack_trace( FILE *out, EVAL_THREAD *thread)
{
  VALACTIVATION *arecord;
  YYLTYPE *location;
  AST_BASE *ret_instr, *fdcl;
  AST_XFUNC_DECL *xfdecl;
  AST_FUNC_DECL *fdecl;
  AST_EXPRESSION *expr;
  BINDING_DATA *binding;
  int frame;
  size_t i;

  for( frame = 1 , arecord = (VALACTIVATION *) EVAL_THREAD_stack_offset( thread, thread->current_activation_record); 
       arecord->parent_function != 0; 
       arecord = (VALACTIVATION *) EVAL_THREAD_stack_offset( thread, arecord->parent_function), ++frame )
  {
     fdcl = arecord->fdecl;
     if (fdcl->type == S_XFUN_DECL) {
	 xfdecl = (AST_XFUNC_DECL *) fdcl;
 
         fprintf( out, "#%d\t%s(", frame, xfdecl->f_name );

	 for( i = 0; i < xfdecl->nparams; i++ ) {
	    if (i) {
	       fprintf( out, " , " );
	    }
	    fprintf( out, "~%s ", xfdecl->params[ i ].param_name );
	    binding =  ((BINDING_DATA *) thread->binding_data_stack.buffer) + arecord->function_frame_start + i + 2;
	    BINDING_DATA_print( out, binding, 0);
	 }
  
     } else {
         fdecl = (AST_FUNC_DECL *) fdcl;

         fprintf( out, "#%d\t%s(", frame, fdecl->f_name );
 
	 ret_instr = frame == 1 ? thread->instr : arecord->ret_instr;
         location = &ret_instr->location;
	 
	 for (i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
	   expr = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
	   binding = ((BINDING_DATA *) thread->binding_data_stack.buffer) + arecord->function_frame_start + i + 2;
	 
	   if (i) {
	      fprintf( out, " , " );
           }
	   fprintf( out, "~%s ", expr->val.ref.lhs );
	   BINDING_DATA_print( out, binding, 0);
	 }
     }
     fprintf( out, ")\n");

     // ??
     //fprintf("%s:%d %s\n",  LEXER_get_file_name( LEXCONTEXT *pc, location->file_id), location->first_line,  activation_record->value.func_activation_record.fdecl->fname );
  }
  return 0;
}

int EVAL_THREAD_yield_value( EVAL_THREAD *thread, BINDING_DATA *data, BINDING_DATA **msg_val )
{
  (void) thread;
  (void) data;
  (void) msg_val;

  return 0;
}

int EVAL_THREAD_activate( EVAL_THREAD *thread, BINDING_DATA *msg, BINDING_DATA **msg_val )
{
  (void) thread;
  (void) msg;
  (void) msg_val;

  return 0;
}


// returns 1 if this frame is a top level frame.
int EVAL_THREAD_is_top_level_frame( EVAL_THREAD *thread )
{
  (void) thread;

  return 0;
}


/* ==================================================================================== */
int EVAL_CONTEXT_init( EVAL_CONTEXT *context )
{
  if (ARRAY_init( &context->bindings_global, sizeof(BINDING_DATA), 10 ) ) {
    return -1;
  }
  DRING_init( &context->gc_heap );
  DRING_init( &context->threads );

  context->is_jmpbuf_set = 0;
  context->current_thread = &context->main;
 
  if (EVAL_THREAD_init( &context->main, context )) {
    return -1;
  }
 
  return 0;
}

void EVAL_CONTEXT_free( EVAL_CONTEXT *context )
{
   DRING *cur, *next;

   // free global bindings
   free_binding_array( &context->bindings_global );
   ARRAY_free( &context->bindings_global );
 
   DRING_FOREACH_SAVE( cur, next, &context->threads ) {
     EVAL_THREAD_free( _OFFSETOF( cur, EVAL_THREAD, threads ) );
   }

   DRING_FOREACH_SAVE( cur, next, &context->gc_heap ) {
     BINDING_DATA_MEM_free(  & ((BINDING_DATA_MEM *) cur) -> data );
   }
}


void EVAL_CONTEXT_start( EVAL_CONTEXT *context )
{
  g_context = context;
  g_cur_thread = context->current_thread; 
}

void EVAL_CONTEXT_mark_binding( BINDING_DATA *binding, int is_stack )
{
  VALARRAY    *parray_value;
  VALHASH     *phash_value;
  VALFUNCTION *pfunc_value;
  BINDING_DATA *key, *value,*pcur,*pend;
  VALHASHPOS pos_iter;
  size_t i;    
  VALFUNCTION_CAPTURE *capture;
 
  while (1)   {

    if (binding->b.value_flags & S_VAR_REF_HEAP) {  
      binding = binding->b.value.value_ref;
    } else if (binding->b.value_flags & (S_VAR_REF_STACK2STACK | S_VAR_REF_HEAP2STACK)) {
      return;
    } else {
      break;
    }
  }

  if ( binding->b.value_flags & S_VAR_HEAP_GC_MARK) {
    return;
  }
  
  binding->b.value_flags |= S_VAR_HEAP_GC_MARK; 

  switch( binding->b.value_type ) {

    case S_VAR_CODE:
      pfunc_value = &binding->b.value.func_value;
      EVAL_CONTEXT_mark_binding( pfunc_value->this_environment, 0 ); 

      // visit captures as well (may include pointers to heap values).
      for( i = 0; i < ARRAY_size( &pfunc_value->captures ); i++ ) {
	 capture = (VALFUNCTION_CAPTURE *) ARRAY_at( &pfunc_value->captures, i );
         if (capture->next.next == 0) {
	    EVAL_CONTEXT_mark_binding( capture->value.ref, 0 );
	 }
      }
      break; 

    case S_VAR_HASH:
      phash_value = &binding->b.value.hash_value;

      VALHASHPOS_init( &pos_iter );

      for( ; VALHASH_iterate( phash_value, &key, &value, &pos_iter ) ; )
      {
        EVAL_CONTEXT_mark_binding( key, 1 ); 
        EVAL_CONTEXT_mark_binding( value, 1 ); 
      }
      break;

    case S_VAR_LIST:
      parray_value = &binding->b.value.array_value;
      for( pcur = parray_value->data, pend = pcur + parray_value->size; pcur != pend; ++pcur )
      {
        if (pcur) {
	 EVAL_CONTEXT_mark_binding( pcur, 1 ); 
        }
      }
      break;

    default:
      break;
  }

  if (!is_stack && binding->b.container != 0) {
     EVAL_CONTEXT_mark_binding( (BINDING_DATA *) binding->b.container, 0 ); 
  }
}


void EVAL_CONTEXT_mark_bindings( ARRAY *bindings, int is_stack)
{
  size_t sz;
  BINDING_DATA *binding, *start;

  sz = ARRAY_size( bindings ); 

  if (sz == 0) {
    return;
  }

  binding = (BINDING_DATA *) ARRAY_at( bindings, sz - 1 );
  start = (BINDING_DATA *) ARRAY_at( bindings, 0 );

  do {

    // examine current binding
    EVAL_CONTEXT_mark_binding( binding, is_stack );

    if (binding == start) {
      break;
    }  
    binding --;

  } while( 1 );
}



void EVAL_CONTEXT_mark( EVAL_CONTEXT *context )
{
   DRING *cur_thread_base;
   EVAL_THREAD *cur_thread;
   
   EVAL_CONTEXT_mark_bindings( &context->bindings_global, 0 );	
   
   // for each thread
   DRING_FOREACH( cur_thread_base, &context->threads )
   {  
     cur_thread = _OFFSETOF( cur_thread_base, EVAL_THREAD, threads );
     EVAL_CONTEXT_mark_bindings( &cur_thread->binding_data_stack, 1 );	
   }

}

void EVAL_CONTEXT_sweep( EVAL_CONTEXT *context )
{
  DRING *cur, *next;
  BINDING_DATA_MEM *mem;

  DRING_FOREACH_SAVE( cur, next, &context->gc_heap )
  {
    mem  = (BINDING_DATA_MEM *) cur;
    if ( (mem->data.b.value_flags & S_VAR_HEAP_GC_MARK) == 0) {
      // container and its members are marked independently, so do not
      // need to delete contained items for container. sweep will take care of them.s
      DRING_unlink( cur );
      free(cur);
    } else {
      mem->data.b.value_flags &= ~S_VAR_HEAP_GC_MARK;
    }   
  }
}


void EVAL_CONTEXT_gc( EVAL_CONTEXT *context )
{
  EVAL_CONTEXT_mark( context );
  EVAL_CONTEXT_sweep( context );
}


void EVAL_CONTEXT_runtime_error( EVAL_CONTEXT *context, const char *format, ... )
{
  char msg[RUNTIME_ERROR_MSG_LEN];
  int len;
  va_list ap;

  va_start(ap, format);
  len = vsnprintf( msg, sizeof(msg) - 1, format, ap );

  fprintf( stderr, "%s\n", msg );

  EVAL_CONTEXT_print_stack_trace_all( stderr, context );

  if (context->is_jmpbuf_set) {
    longjmp( context->jmpbuf, 1 ); 
  } else {
    exit(1);
  }
}

void EVAL_CONTEXT_print_stack_trace_all( FILE *out, EVAL_CONTEXT *context )
{
   DRING *cur_thread_base;
   EVAL_THREAD *cur_thread;
  
   DRING_FOREACH( cur_thread_base, &context->threads )
   {  
     cur_thread = _OFFSETOF( cur_thread_base, EVAL_THREAD, threads );
     EVAL_THREAD_print_stack_trace( out, cur_thread );
   }
}

