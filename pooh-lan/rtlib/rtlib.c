#include "rtlib.h"
#include "stdio.h"
#include <cutils/util.h>
#include <stdarg.h>
#include <nutils/ioutils.h>

#if defined(__CYGWIN__) || defined(_WIN32)
#include <bits/wordsize.h>
#endif

static EVAL_CONTEXT *g_context; // the current interpreter context.
static EVAL_THREAD  *g_cur_thread;
static BINDING_DATA finalize_key_name;

#define ARRAY_GROW_CAPACITY_BY 10
#define STRING_GROW_CAPACITY_BY 20


#define CLEAR_LOW_BIT( ptr )  ( (SRING *) ( * ( (size_t *) &ptr ) & ~1 ) )
#define PTR_LOW_BIT_SET( ptr ) ( * ( (size_t * ) &ptr ) & 1  )


/* ==================================================================================== */
typedef struct tagPRINT_CHECK_LOOP {
  HASH_Entry entry;
  void *key;
  void *to;
} PRINT_CHECK_LOOP;

// check if array contains self references.
void VALARRAY_check_ref( VALARRAY *arr, int stage);


// check if hash contains self references.
void VALHASH_check_ref( VALHASH *hash, int stage );

void BINDING_DATA_unlink_ref( BINDING_DATA *src, BINDING_DATA *ref );

int  BINDING_DATA_check_ref( BINDING_DATA *data, int stage );

void BINDING_DATA_move( BINDING_DATA *to, BINDING_DATA *from);

void EVAL_THREAD_clear_check_ref( EVAL_THREAD *thread );
void EVAL_THREAD_print_ref_add_loop( BINDING_DATA *from, BINDING_DATA *to);
void make_capture( VALFUNCTION_CAPTURE *pentry, union tagBINDING_DATA *data, int data_entry, struct tagEVAL_THREAD *thread );

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

static BINDING_DATA *STATIC_NULL;

union tagBINDING_DATA *get_CONST_NULL()
{
  return STATIC_NULL;
}

/* ==================================================================================== */

void VALARRAY_set_capacity( VALARRAY *arr, size_t capacity )
{
  if (arr->capacity < capacity ) {
    capacity += ARRAY_GROW_CAPACITY_BY;
    HEAP_realloc( (void **) &arr->data, sizeof( BINDING_DATA * ) * capacity );
    arr->capacity = capacity;
  }
} 


void VALARRAY_copy( VALARRAY *to, VALARRAY *from, CP_KIND deep_copy_values  )
{
   size_t i;
   
   VALARRAY_set_capacity( to, from->size );
    
   for( i = 0; i < from->size; i++) {

     if (from->data[ i ] != 0) {

       to->data[ i ] = BINDING_DATA_MEM_new( S_VAR_NULL );
       BINDING_DATA_copy( to->data[ i ], from->data[ i ], deep_copy_values );

//     tobinding->b.container = _OFFSETOF(to, BINDING_DATA_VALUE, value );
//     tobinding->b.value_flags |= S_VAR_COLL_ENTRY;	 

     } else {
       to->data[ i ] = 0;
     }
   }
   for( ; i < to->size; i++) {
     to->data[ i ] = 0;
   }
   to->size = from->size;
}

void VALARRAY_free( VALARRAY *arr )
{
  BINDING_DATA *binding;
  size_t i;

  for( i = 0; i < arr->size; i++ ) {
    binding = arr->data[i];
    if (binding != 0 && ! IS_NULL( binding ) ) {
//    binding->b.container = 0;
      BINDING_DATA_free( binding );
    }
  }
  HEAP_free( arr->data );    
}

union tagBINDING_DATA *VALARRAY_get( VALARRAY *arr, size_t idx )
{
  BINDING_DATA *rval;

  if (idx >= arr->size) {
    // TODO: null value can be changed by reference, so must introduce notion of CONST data. 
    return STATIC_NULL;
  }
  rval = arr->data[ idx ];
  if (rval == 0) {
    // TODO: null value can be changed by reference, so must introduce notion of CONST data. 
    return STATIC_NULL;
  }
  return rval;
}

union tagBINDING_DATA *VALARRAY_get_n( VALARRAY *arr, size_t idx )
{
  BINDING_DATA *rval;

  if (idx >= arr->size) {
    return 0;
  }
  rval = arr->data[ idx ];
  if (rval == 0) {
    return 0;
  }
  return rval;
}


void set_outer_ref( BINDING_DATA *mem, void *arr, AST_VAR_TYPE ty, int set_ref )
{
  BINDING_DATA *env; //,*this_ref;

  assert( mem->b.value_type ==  S_VAR_CODE );
  
  env = (BINDING_DATA *) _OFFSETOF( arr, BINDING_DATA_VALUE, value );
  assert( env->b.value_type == (short) ty );

  if (set_ref) {
    //this_ref = BINDING_DATA_MEM_new( S_VAR_NULL );
    //BINDING_DATA_copy( this_ref, env, CP_REF );
    //mem->b.value.func_value.this_environment = this_ref;

    mem->b.value.func_value.this_environment = env;
    env->b.value_flags_ref |= S_VAR_OBJECT;  
 
  } else {
    mem->b.value.func_value.this_environment = env;
    
    //this_ref = mem->b.value.func_value.this_environment;
    //assert( this_ref->b.value_flags_ref != 0 );
 
    //if ((this_ref->b.value_flags_ref & S_VAR_REF_HEAP2STACK) != 0) {
    //  BINDING_DATA_unlink_ref( env, this_ref );
    //}
    //BINDING_DATA_copy( this_ref, env, CP_REF );
  }

}

void VALARRAY_update_this_env( VALARRAY *arr )
{
  BINDING_DATA *binding;
  size_t i;

  for( i = 0; i < arr->size; i++ ) {
    binding = arr->data[i];
    if (binding != 0  && binding->b.value_type == S_VAR_CODE ) {
      set_outer_ref( binding, arr, S_VAR_LIST, 0 );
    }
  }
}


union  tagBINDING_DATA  *VALARRAY_set_entry( VALARRAY *arr, size_t idx )
{
   size_t nsize = idx + 1; //, i;
   BINDING_DATA *data;

   VALARRAY_set_capacity( arr, nsize );
   if (arr->size < nsize) {
     memset( &arr->data[ arr->size ], 0 , sizeof(void *) * (nsize - arr->size) );
     arr->size =  nsize;
   } else {
     data = arr->data[ idx ]; 	
     if (data != 0)
        return data;
   }

   data = BINDING_DATA_MEM_new( S_VAR_NULL );
   arr->data[ idx ] = data;  
   return data;
}

void VALARRAY_set( VALARRAY *arr, size_t idx, union tagBINDING_DATA *mem,  CP_KIND copy_value)
{
   size_t nsize = idx + 1; //, i;
   BINDING_DATA *data;

   VALARRAY_set_capacity( arr, nsize );
   if (arr->size < nsize) {
     memset( &arr->data[ arr->size ], 0 , sizeof(void *) * (nsize - arr->size) );
     arr->size =  nsize;
   }

   data = BINDING_DATA_MEM_new( S_VAR_NULL );
   arr->data[ idx ] = data;   
   
   if (mem->b.value_type != S_VAR_CODE) {
     BINDING_DATA_copy( data, mem, copy_value );
   } else {
     BINDING_DATA_copy( data, mem, CP_VALUE );
     set_outer_ref( data, arr, S_VAR_LIST, 1 );
   } 
   
   assert( ( data->b.value_flags_val & S_VAR_HEAP_VALUE ) != 0);
// data->b.container = _OFFSETOF(arr, BINDING_DATA_VALUE, value );
// data->b.value_flags |= S_VAR_COLL_ENTRY;	 


} 

int VALARRAY_equal( VALARRAY *cmpa, VALARRAY *cmpb )
{
  size_t i;

  if (cmpa->size != cmpb->size) {
    return -1;
  }

  for( i = 0; i < cmpa->size; i++ ) {
     if (!IS_NULL( cmpa->data[ i ] ) && !IS_NULL( cmpb->data[ i ] )) {
	
	if ( BINDING_DATA_cmp( cmpa->data[ i ], cmpb->data[ i ] ) != 0) {
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
     data = arr->data[ i ];
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
#if 0  
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
  
//  assert( data->b.container == _OFFSETOF( arr, BINDING_DATA_VALUE, value ) );
  
#if 0  
    if (data->b.value_type == S_VAR_INT ) {
       // check if there is a sub range of integers.
       for( j = i + 1, val = data->b.value.long_value + 1; j < VALARRAY_size( arr ); j ++, val++) {
	  cdata = VALARRAY_get( arr, j );
	  
//	  assert( cdata->b.container ==  _OFFSETOF(arr, BINDING_DATA_VALUE, value ) );

	  if ( cdata->b.value_type != S_VAR_INT || cdata->b.value.long_value != val) {
	    break;
	  }

	  if ( g_cur_thread->has_loops && HASH_find( &g_cur_thread->print_check_ref_loops, cdata, sizeof(void *) ) != 0) {
	    break;
	  }

       }

       if ( (j - i) >= DISPLAY_MIN_RANGE_LENGTH) {
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

void  VALARRAY_check_ref( VALARRAY *arr, int stage )
{
  size_t i;
  BINDING_DATA *data;

  for( i = 0; i < VALARRAY_size( arr ) ; i++ ) {
    data = VALARRAY_get( arr , i );  
    if (!IS_NULL( data )) {
      BINDING_DATA_check_ref(  data, stage );
    }
  }

}



/* ==================================================================================== */

typedef struct tagHASH_VALUE_ENTRY {
  SRING link;
  size_t hash_value;
   
  BINDING_DATA *key,*value;  // includes pointer to hash structure, so that reference to element results in reference to the collection.
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
  tmp_hash.size = hash->size;  
  memcpy( hash, &tmp_hash, sizeof( VALHASH ) );
}

void VALHASH_free_imp( VALHASH *hash, int discard )
{
  size_t i;
  HASH_VALUE_ENTRY *cur_entry;
  SRING *cur, *next_link;


  if ( hash->ordered_keys ) {
    free( hash->ordered_keys);  
  }
  if (hash->buckets) {
    
    for(i = 0; i < hash->numbuckets; i++) {

      SRING_FOREACH_SAVE(cur, next_link, &hash->buckets[ i ] ) {
        cur_entry = (HASH_VALUE_ENTRY *) cur;

	if (discard) {
	  cur_entry->key = 0;
	  cur_entry->value = 0;
	} else {  
              BINDING_DATA_free( cur_entry->key );
	  BINDING_DATA_free( cur_entry->value );
	} 
	free(cur_entry);
      }	 
     }
     free(hash->buckets);
  }
}

void VALHASH_free( VALHASH *hash )
{
  VALHASH_free_imp( hash, 0 );
}

static HASH_VALUE_ENTRY *VALHASH_find_impl( VALHASH *hash, union tagBINDING_DATA *key )
{
  size_t hash_value;
  SRING *bucket,*cur;
  HASH_VALUE_ENTRY *cur_entry;

  if (hash->buckets == 0) {
    return 0;
  }

  hash_value = BINDING_DATA_hash( key );


  bucket = &hash->buckets[ hash_value & (hash->numbuckets - 1) ];
  
  SRING_FOREACH( cur, bucket )  {
    cur_entry = (HASH_VALUE_ENTRY *) cur;
    if (cur_entry->hash_value == hash_value) {
	  if (BINDING_DATA_cmp( cur_entry->key, key )  == 0 ) {
	    return cur_entry;
	  }
	}
   }
   return 0;
}

union tagBINDING_DATA * VALHASH_set_entry( VALHASH *hash, union tagBINDING_DATA *key )
{
  size_t threshold,idx;
  HASH_VALUE_ENTRY *entry;
  BINDING_DATA *pvalue;
  SRING *bucket;
  int rt;

  key = BINDING_DATA_follow_ref( key );

  if ((entry = VALHASH_find_impl( hash, key )) == 0) {
    if (hash->buckets == 0) {
      VALHASH_set_capacity( hash, HASH_INIT_SIZE );
    } else {

      threshold = (hash->numbuckets >> 1) + (hash->numbuckets >> 2);  /* if 3/4 full -> resize */
      if (hash->size > threshold) {
  	  VALHASH_set_capacity( hash, hash->size << 1 ); /* new capacity twice the current value */
      }
    }
    entry = (HASH_VALUE_ENTRY *) HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );
    hash->size ++;
    entry->value = 0;
    rt = 0;
  } else { // an entry with exactly the same key already exists.
    rt = 1;
  }

  if (!entry->value) {
    entry->value = BINDING_DATA_MEM_new( S_VAR_NULL ); 
  }
  pvalue = entry->value; 
 
 
  if (hash->ordered_keys) {
    free( hash->ordered_keys );
    hash->ordered_keys = 0;
  }

  if ( ! rt ) {
    entry->key = BINDING_DATA_MEM_new( S_VAR_NULL );
   
    BINDING_DATA_copy( entry->key, key, CP_VALUE ); //  table must have a copy of the key, so that it can't be modified by references while it is in the table.
   entry->hash_value = BINDING_DATA_hash( entry->key );

 
    idx = entry->hash_value & (hash->numbuckets - 1);  
    bucket = &hash->buckets[ idx ];
    SRING_push_front( bucket, &entry->link );
  } 


  return pvalue;
}


int VALHASH_set( VALHASH *hash, union tagBINDING_DATA *key, union tagBINDING_DATA *value , CP_KIND copy_by_value)
{
  BINDING_DATA *pvalue =  VALHASH_set_entry( hash, key );
 
  if (value->b.value_type != S_VAR_CODE) {
    BINDING_DATA_copy( pvalue, value, copy_by_value );
  } else {
    BINDING_DATA_copy( pvalue, value, CP_VALUE );
    set_outer_ref( pvalue, hash, S_VAR_HASH, 1 );
  } 

//entry->key.b.container = _OFFSETOF(hash, BINDING_DATA_VALUE, value );
//entry->key.b.value_flags |= S_VAR_COLL_ENTRY;	 

//entry->value.b.container = _OFFSETOF(hash, BINDING_DATA_VALUE, value );
//entry->value.b.value_flags |= S_VAR_COLL_ENTRY;	 
 
  return 0;
}

void VALHASH_copy( VALHASH *to, VALHASH *from, CP_KIND copy_by_value )
{
   size_t i;
   SRING *fcur,*from_bucket, *copy_bucket;
   HASH_VALUE_ENTRY *from_entry,*copy_entry;

   VALHASH_free_imp( to, 1 );

   to->buckets = HEAP_alloc( from->numbuckets * sizeof(SRING) );
   memset( to->buckets, 0, from->numbuckets * sizeof(SRING) ); 

   to->numbuckets = from->numbuckets;
   to->size = from->size;

   for( i = 0; i < from->numbuckets; i++ ) {
     copy_bucket = &to->buckets[ i ];
     from_bucket = &from->buckets[ i ];

     SRING_init( copy_bucket );

     if (from_bucket) {
       SRING_FOREACH( fcur, from_bucket )  {
         from_entry = (HASH_VALUE_ENTRY *) fcur;

         copy_entry = HEAP_alloc( sizeof( HASH_VALUE_ENTRY ) );

	 copy_entry->hash_value = from_entry->hash_value;
	 
	 copy_entry->key = BINDING_DATA_MEM_new( S_VAR_NULL );
	 copy_entry->value = BINDING_DATA_MEM_new( S_VAR_NULL );
 
	 BINDING_DATA_copy( copy_entry->key,  from_entry->key, 1 );
	 BINDING_DATA_copy( copy_entry->value, from_entry->value, copy_by_value ); 
	 if (copy_entry->value->b.value_type == S_VAR_CODE) {
           set_outer_ref( copy_entry->value, to,  S_VAR_HASH, 1 );
	 }


//	 copy_entry->key.b.container = _OFFSETOF( to, BINDING_DATA_VALUE, value );
//   	 copy_entry->key.b.value_flags |= S_VAR_COLL_ENTRY;	 

//	 copy_entry->value.b.container = _OFFSETOF( to, BINDING_DATA_VALUE, value );
//   	 copy_entry->value.b.value_flags |= S_VAR_COLL_ENTRY;	 

	 SRING_insert_after( copy_bucket, &copy_entry->link );
	 copy_bucket = copy_bucket->next;
       }
     } 	
   }
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
  HASH_VALUE_ENTRY *cur_entry;

  key = BINDING_DATA_follow_ref( key );

  cur_entry = VALHASH_find_impl( hash, key );
  if (!cur_entry) {
    *rval = 0;
    return -1;
  }
  *rval = cur_entry->value;
  return 0;
}
     
int VALHASH_iterate( VALHASH *hash, union tagBINDING_DATA **key, union tagBINDING_DATA **value, VALHASHPOS *pos )
{
  HASH_VALUE_ENTRY *entry;

  if (!pos->bucket) {
    if (hash->buckets == 0) {
      *key = *value = 0;
      return 0;
    }
    pos->bucket = hash->buckets;
    pos->bucket_pos = hash->buckets;
  }

  while(1) {
    if ( (pos->bucket_pos = SRING_get_next( pos->bucket, pos->bucket_pos )) != 0) {
      entry = (HASH_VALUE_ENTRY *) pos->bucket_pos;

      *key = entry->key;
      *value = entry->value;
	    
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

//    assert( key->b.container == _OFFSETOF( data, BINDING_DATA_VALUE, value ) );
//    assert( value->b.container == _OFFSETOF( data, BINDING_DATA_VALUE, value ) );

      BINDING_DATA_print( out, key, level + 1 );
      fprintf( out, " : " );
      BINDING_DATA_print( out, value, level + 1 );
   
   }
   
   fprintf(out," }");
}

void VALHASH_check_ref( VALHASH *hash, int stage )
{
   VALHASHPOS pos;
   BINDING_DATA *key,*value;
 
   VALHASHPOS_init( &pos );
   for( ; VALHASH_iterate( hash, &key, &value, &pos ) ; ) {
     BINDING_DATA_check_ref( key, stage );
     BINDING_DATA_check_ref( value, stage );
   }
}


void VALHASH_upate_this_env( VALHASH *hash )
{
  size_t i;
  HASH_VALUE_ENTRY *cur_entry;
  SRING *cur, *next_link;
  BINDING_DATA *binding;

  if (hash->buckets) {
   for(i = 0; i < hash->numbuckets; i++) {

      SRING_FOREACH_SAVE(cur, next_link, &hash->buckets[ i ] ) {
        cur_entry = (HASH_VALUE_ENTRY *) cur;

	binding = cur_entry->value;
        if (binding != 0  && binding->b.value_type == S_VAR_CODE ) {
          set_outer_ref( binding, hash, S_VAR_HASH, 0 );
        }
      }	 
    }
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
  size_t len = argA->length < argB->length ? argA->length : argB->length;
  int rt = strncmp( argA->string != 0 ? argA->string : "", argB->string != 0 ? argB->string : "" , len );
  if (rt == 0) { 
    return rt;
  }
  if (argA->length < argB->length) {
    return -1;
  }
  return 1;
}
#include <cutils/hashfunction.h>

void VALSTRING_print( FILE *out, VALSTRING *data )
{

  // ??? null terminator
  fprintf( out, "'%.*s'", (int) data->length, data->string );
}

size_t VALSTRING_hash( VALSTRING *string )
{
   return HASHFUNCTION_Bob_Jenkins_one_at_a_time( string->string, string->length );
}

void VALSTRING_make_null_terminated( VALSTRING *data )
{
   if (data->capacity == data->length)
	VALSTRING_set_capacity( data, data->capacity + 1 );
   data->string[ data->length ] = '\0';
}


/* ==================================================================================== */

void VALFUNCTION_free( VALFUNCTION *fnc )
{
  VALFUNCTION_CAPTURE *cap;
  int i;

  free( fnc->pnested_closures );

  // check if no capture is in pending list -> linked to definition source, unlink if necessary
  for( i = 0; i < fnc->ncaptures; i++ ) {
     cap = &fnc->pcaptures[ i ];
     if (cap->next.next != 0) {
       DDDLIST_unlink( &cap->next );
     }
  }
  free( fnc->pcaptures );
}

void VALFUNCTION_copy( VALFUNCTION *to, VALFUNCTION *from )
{
  memset( to, 0, sizeof( VALFUNCTION ) );
  to->fdecl = from->fdecl;
  to->this_environment = from->this_environment;
}

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
//         if ( VALFUNCTION_capture_equal( &cmpa->captures, &cmpb->captures ) ) {
	     return 0;
//	   }
      }
  } 
  return -1;
}	
  

void VALFUNCTION_init_cap( VALFUNCTION *func, size_t num_captures, size_t num_nested_closures )
{
//func->fdecl = 0;
  func->this_environment = 0;

#if 0
  if (ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), num_captures )) {
    EVAL_CONTEXT_gc( g_context );
    if (ARRAY_init( &func->captures, sizeof( VALFUNCTION_CAPTURE ), num_captures )) {
       EVAL_CONTEXT_runtime_error( g_context, "out of memory");
    }
  } 
#endif 

  if (num_captures != 0) {
    func->pcaptures = malloc( sizeof( VALFUNCTION_CAPTURE ) * num_captures );
    if (func->pcaptures == 0) {
      EVAL_CONTEXT_gc( g_context );
      func->pcaptures = malloc( sizeof( VALFUNCTION_CAPTURE ) * num_captures );
      if (!func->pcaptures) {
         EVAL_CONTEXT_runtime_error( g_context, "out of memory");
      }
    }
    memset( func->pcaptures, 0, sizeof( VALFUNCTION_CAPTURE ) * num_captures );
  }
  func->ncaptures = (uint16_t) num_captures;

  if (num_nested_closures != 0) {
    func->pnested_closures = malloc( sizeof( void * ) * num_nested_closures );
    if (func->pnested_closures == 0) {
      EVAL_CONTEXT_gc( g_context );
      func->pnested_closures = malloc( sizeof( void * ) * num_nested_closures );
      if (!func->pnested_closures) {
        EVAL_CONTEXT_runtime_error( g_context, "out of memory");
      }
    }
    memset( func->pnested_closures, 0, sizeof( void * ) * num_nested_closures );
  }  
  func->nclosures = (uint16_t) num_nested_closures;
}

void VALFUNCTION_init_outer_refs( struct tagEVAL_THREAD *cthread, VALFUNCTION *fnc, AST_FUNC_DECL *fdecl, int scope_nesting )
{
  (void) cthread;
  (void) fnc;
  (void) fdecl;

  ARRAY *refs;
  BINDING_ENTRY *entry, *capture_source;
  BINDING_DATA *data;
  VALFUNCTION_CAPTURE *capture_entry;
  size_t num_outer_refs, i;
  

  refs = &fdecl->outer_refs;
  num_outer_refs = ARRAY_size( refs );
 
  for(i = 0; i < num_outer_refs; i ++ ) {
    void **tmp = (void **) ARRAY_at( refs, i );   
    entry = (BINDING_ENTRY *) tmp[0];
    assert( entry->scope == REF_SCOPE_CLOSURE );
    if (entry->scope_nesting == scope_nesting) {
        // capture reference resolves to local of the current scope;
        capture_entry = &fnc->pcaptures[ i ];
        
        capture_source = entry->source;
        assert( capture_source );
        data = EVAL_THREAD_stack_frame_offset( cthread, capture_source->stack_offset ); 
        assert( data );
        data = BINDING_DATA_follow_ref( data );
        
        make_capture( capture_entry, data, 0, g_cur_thread );
    }
  }
}


// create function object for each level of closures / walk tree of functions
void VALFUNCTION_init_outer_ref_tree( struct tagEVAL_THREAD *cthread, VALFUNCTION *fnc, AST_FUNC_DECL *fdecl, int nesting )
{
    TREENODE *node;
    BINDING_DATA *nested_func;   
    AST_FUNC_DECL *fdecl_child;
    VALFUNCTION *nested_func_obj;

    int i;
    
    // resolve all capture value (that can be resolved at this stage)
    VALFUNCTION_init_outer_refs( cthread, fnc, fdecl, nesting );

    for( i = 0, node = TREE_first_child( &fdecl->funcs ); node != 0 ; node = TREE_right_sibling( node ), i++ ) {
        assert( i < fdecl->num_nested_func_decl );

        // function declaration nested in the current one.
	fdecl_child = _OFFSETOF(  node,  AST_FUNC_DECL, funcs );
  
        // function object for nested declaration created.
        nested_func = BINDING_DATA_MEM_new( S_VAR_CODE ); 
    
        nested_func_obj = &nested_func->b.value.func_value; 
        nested_func_obj->fdecl = (AST_BASE *) fdecl_child;
        VALFUNCTION_init_cap( nested_func_obj, ARRAY_size( &fdecl_child->outer_refs ), fdecl_child->num_nested_func_decl );
     

        // nested function object registered into this function object.
        fnc->pnested_closures[ i ] = nested_func;
	
        // recurse one declaration level down.
        VALFUNCTION_init_outer_ref_tree( cthread, nested_func_obj, fdecl_child, nesting + 1 );
    }
}

#define SIZE_HIGH_BIT 0x80000000
#define SIZE_MASK     0x7FFFFFFF


void make_capture( VALFUNCTION_CAPTURE *pentry, union tagBINDING_DATA *data, int data_entry, struct tagEVAL_THREAD *thread )
{
   size_t pos;

   if  ( ! IS_STACK_VALUE( data ) )  {
 
      // capture entry points to heap reference.
      pentry->next.next = 0;
      pentry->value.ref = data; 
      pentry->thread = 0;
      
   } else { 
      DDDLIST *first = &data->b.backlink; //container;
      
      pos =  data - ((BINDING_DATA *) thread->binding_data_stack.buffer); 
      pentry->value.stack_ref = pos;
      pentry->thread = thread;

      DDDLIST_insert_after( first, &pentry->next );

   }

   if (data_entry) {
      pentry->value.stack_ref |= SIZE_HIGH_BIT;
   }
}

static VALFUNC_PRINT_MODE g_show_method_sig_mode;

void VALFUNCTION_print_mode( VALFUNC_PRINT_MODE show_method_sig )
{
   g_show_method_sig_mode = show_method_sig;
}

void  VALFUNCTION_print( FILE *out, VALFUNCTION *func )
{
   AST_FUNC_DECL *fdecl;    
   AST_XFUNC_DECL *fxdecl;
   AST_EXPRESSION *param_decl;
   AST_XFUNC_PARAM_DECL *xparam_decl;
   size_t i;


   if (func->fdecl->type == S_FUN_DECL) {

     fdecl = (AST_FUNC_DECL *) func->fdecl;

     fprintf( out, "sub %s", fdecl->f_name != 0 ? fdecl->f_name : "" );
     if (!g_show_method_sig_mode) {
       return;
     }
     fprintf( out,"(" );
 
     for( i = 0; i < AST_VECTOR_size( fdecl->func_params ); i++ ) {
       if (i > 0) {
         fprintf(out, " , " );
       }

       param_decl = (AST_EXPRESSION *) AST_VECTOR_get( fdecl->func_params, i );
      
       fprintf( out, "~%s", param_decl->val.ref.lhs );
     }
   } else {
     assert( func->fdecl->type == S_XFUN_DECL );
     fxdecl = (AST_XFUNC_DECL *) func->fdecl;

     fprintf( out, "sub %s(", fxdecl->f_name );
     if (!g_show_method_sig_mode) {
       return;
     }
 
     for( i = 0; i < fxdecl->nparams; i++ ) {
       if (i > 0) {
         fprintf(out, " , " );
       }
       xparam_decl = &fxdecl->params[ i ];
       fprintf( out, "~%s", xparam_decl->param_name );
    } 
   }
   fprintf( out, ")" );
}

/* ==================================================================================== */
void BINDING_DATA_init( BINDING_DATA *binding, AST_VAR_TYPE value_type)
{
   binding->b.value_type = value_type;
   binding->b.value_flags_ref = 0;
   binding->b.value_flags_val = 0;
   DDDLIST_init( &binding->b.backlink ); //container = 0;
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

BINDING_DATA *BINDING_DATA_follow_ref_imp(  BINDING_DATA *arg, int copy_on_write, BINDING_DATA **pprev )
{
    short value_flags;     
    BINDING_DATA *binding,*arecord, *prev;
    size_t cap_idx,pos;
    VALFUNCTION_CAPTURE *cap;
    VALFUNCTION *fobject;  

    for( prev = 0, binding = arg ; ;  ) {

      value_flags = binding->b.value_flags_ref;

      if ( (value_flags & ( S_VAR_REF_HEAP | S_VAR_REF_HEAP2STACK | S_VAR_REF_STACK2STACK | S_VAR_REF_GLOB | S_VAR_CAPTURE_REF ) ) == 0) {
        *pprev = prev;
        return binding;
      }

      prev = binding;

      if (copy_on_write && (binding->b.value_flags_ref & REF_COPY_ON_WRITE) != 0 ) {

	 arecord = BINDING_DATA_MEM_new( S_VAR_NULL ); 
	 binding->b.value_flags_ref = S_VAR_REF_HEAP;
	 binding->b.value.value_ref = arecord;

        *pprev = prev;
	 return arecord;
      }
 

      if (value_flags & S_VAR_REF_HEAP) {
        binding = binding->b.value.value_ref;
      } else if (value_flags & S_VAR_REF_HEAP2STACK) {
        EVAL_THREAD *thread;
	
	thread = binding->b.value.heap2stack_ref.thread;
	pos = binding->b.value.heap2stack_ref.value.stack_ref & SIZE_MASK;
	assert( pos < ARRAY_size( &thread->binding_data_stack ) );
	binding= ((BINDING_DATA *) thread->binding_data_stack.buffer) + pos;
      } else if (value_flags & S_VAR_REF_STACK2STACK) { 
	pos = binding->b.value.stack2stack_ref;
	assert( pos < ARRAY_size( &g_cur_thread->binding_data_stack ) );
	binding = ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + pos;
      } else if (value_flags & S_VAR_REF_GLOB) { 
     	pos = binding->b.value.stack2stack_ref;
	assert( pos < ARRAY_size( &g_context->bindings_global ) );
	binding = ((BINDING_DATA *) g_context->bindings_global.buffer) + pos;
      } else if (value_flags & S_VAR_CAPTURE_REF) {
        arecord = ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer) + g_cur_thread->current_function_frame_start + 1;
        cap_idx = binding->b.value.stack2stack_ref & SIZE_MASK;	

	fobject = arecord->activation_record.function_object;
	if (!fobject) {
	    EVAL_CONTEXT_runtime_error( g_context, "Cant access capture from function without function object (go figure)");
	}
	cap = (VALFUNCTION_CAPTURE *) &fobject->pcaptures[ cap_idx ];

	if (cap->next.next == 0) {
	  binding = cap->value.ref;
	} else {
	  pos = cap->value.stack_ref & SIZE_MASK;
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

BINDING_DATA *BINDING_DATA_follow_ref(  BINDING_DATA *arg )
{
  BINDING_DATA *prev;  
  return BINDING_DATA_follow_ref_imp( arg, 0, &prev );
}

void BINDING_DATA_cp( BINDING_DATA *to, BINDING_DATA *from)
{
  unsigned char tmp;

  //assert( (from->b.value_flags_ref & S_VAR_REF_HEAP2STACK) == 0 );
 
  tmp =  to->b.value_flags_val; 

  memcpy( to, from, sizeof( BINDING_DATA ) );
  
  if ( (to->b.value_flags_ref & S_VAR_OBJECT) != 0) {
     switch( to->b.value_type )
     {
       case S_VAR_HASH:
         VALHASH_upate_this_env( &to->b.value.hash_value );
         break;
       case S_VAR_LIST:
         VALARRAY_update_this_env( &to->b.value.array_value );
         break;
       default:
         assert( 0 );
         break;
     }
  }
  to->b.value_flags_val = tmp;
}

void BINDING_DATA_copy_ext( BINDING_DATA *to, BINDING_DATA *from , EVAL_REF_KIND ty )
{
  BINDING_DATA *prev;

  switch( ty ) {

   case PARAM_BY_REF: 
     BINDING_DATA_copy( to, from, CP_REF );
     break;
   
   case PARAM_BY_VAL:
     if (!IS_REF( from ) && (from->b.value_type & (S_VAR_INT | S_VAR_DOUBLE | S_VAR_NULL) ) != 0 )    
       BINDING_DATA_copy( to, from, CP_VALUE );
     else {
       BINDING_DATA_copy( to, from, CP_REF );
       assert( IS_REF( to ) );
       to->b.value_flags_ref |= REF_COPY_ON_WRITE;
     }
     break;
   
   case COPY_EVAL_BY_VAL:
     if (!IS_REF( from ) && (from->b.value_type & (S_VAR_INT | S_VAR_DOUBLE | S_VAR_NULL) ) != 0 )    
       BINDING_DATA_copy( to, from, CP_VALUE );
     else 
       BINDING_DATA_copy( to, from, CP_REF );
     break;

   case COPY_EVAL_BY_REF:
     from = BINDING_DATA_follow_ref_imp( from, 0, &prev );
     if (prev) {
       from = prev;
     }
     BINDING_DATA_copy( to, from, CP_REF );
     break;
   
   case COPY_SINGLE_ASSIGN_BY_REF:
     to = BINDING_DATA_follow_ref_imp( to, 1, &prev );
     BINDING_DATA_copy( to, from, CP_MOVE );
     break;
   
   case COPY_SINGLE_ASSIGN_BY_VAL:
     to = BINDING_DATA_follow_ref_imp( to, 1, &prev );
     if (!IS_REF( from ))
        BINDING_DATA_copy( to, from, CP_MOVE );
     else {
       from = BINDING_DATA_follow_ref( from );
       BINDING_DATA_copy( to, from, CP_VALUE);
     }
     break;

   case COPY_MULTI_ASSIGN_BY_REF:
     to = BINDING_DATA_follow_ref_imp( to, 1, &prev );
     BINDING_DATA_copy( to, from, CP_REF );
     break;

   case COPY_MULTI_ASSIGN_BY_VAL:
     to = BINDING_DATA_follow_ref_imp( to, 1, &prev );
     from = BINDING_DATA_follow_ref( from );
     BINDING_DATA_copy( to, from, CP_VALUE);
     break;
   }

}


void BINDING_DATA_move( BINDING_DATA *to, BINDING_DATA *from)
{
  BINDING_DATA_cp( to, from);
  
  BINDING_DATA_init( from, S_VAR_NULL );
}

void BINDING_DATA_return_value( BINDING_DATA *ret_slot, BINDING_DATA *rvalue  )
{
   if (IS_REF( rvalue )) {
      
      if (rvalue->b.value_flags_ref & (S_VAR_REF_HEAP | S_VAR_REF_GLOB)) {
	BINDING_DATA_move( ret_slot, rvalue);
	return;
      }
      // copy the value if S_VAR_REF_STACK2STACK | S_VAR_REF_HEAP2STACK | S_VAR_CAPTURE_REF
      BINDING_DATA_copy( ret_slot, rvalue, CP_VALUE );
      return;
   }
   BINDING_DATA_move( ret_slot, rvalue );
}


void BINDING_DATA_copy( BINDING_DATA *to, BINDING_DATA *from, CP_KIND copy_by_value  )
{
   size_t pos;

   if (to->b.value_type == S_VAR_LIST || to->b.value_type ==  S_VAR_HASH) {
     DATA_REF refto, reffrom;

     DATA_REF_set( &reffrom, from );
     DATA_REF_set( &refto, to );
     
     BINDING_DATA_free( to );
  
     from = DATA_REF_get( &reffrom );
     to = DATA_REF_get( &refto );
    } else
     BINDING_DATA_free( to );
  
   if (copy_by_value == CP_MOVE) {
     BINDING_DATA_move( to, from );
     return;
   }

   if ( IS_REF( from ) ) {
     int is_heap2stack;

     assert( (from->b.value_flags_ref &  S_VAR_CAPTURE_REF)  == 0 ); // have to think about this case
        
     is_heap2stack = from->b.value_flags_ref & S_VAR_REF_HEAP2STACK; 
    
     //from = BINDING_DATA_follow_ref( from ); // ???? must be wrong somewhere.
     
     if (! (is_heap2stack && IS_STACK_VALUE( to ) ) ) {
    
       BINDING_DATA_cp( to, from);
 
      //memcpy( to, from, sizeof( BINDING_DATA ) );

       if (is_heap2stack) {
         DDDLIST_insert_after( &from->b.value.heap2stack_ref.next, &to->b.value.heap2stack_ref.next );
       }
     } else {
      to->b.value_flags_ref = S_VAR_REF_HEAP;
      to->b.value.value_ref = from;
     }
     
#if 0   
     int is_heap2stack;
         
     assert( (from->b.value_flags_ref &  S_VAR_CAPTURE_REF)  == 0 ); // have to think about this case

     is_heap2stack = from->b.value_flags_ref & S_VAR_REF_HEAP2STACK; 
     
     if (! (is_heap2stack && IS_STACK_VALUE( to ) ) ) {
    
       BINDING_DATA_cp( to, from);
 
     //memcpy( to, from, sizeof( BINDING_DATA ) );

       if (is_heap2stack) {
         DDDLIST_insert_after( &from->b.value.heap2stack_ref.next, &to->b.value.heap2stack_ref.next );
       }
     } else {
      to->b.value_flags_ref = S_VAR_REF_HEAP;
      to->b.value.value_ref = from;
     }
#endif     
     return;
   }
   
   if ( ! copy_by_value ) {
    
     to->b.value_type = 0;
 
     if (IS_HEAP_VALUE(from) ) {

        to->b.value_flags_ref = S_VAR_REF_HEAP;
	to->b.value.value_ref = from;
     
     } else if (IS_GLOBAL_VALUE(from)) {
       	
	pos =  from - ((BINDING_DATA *) g_context->bindings_global.buffer); 
        to->b.value_flags_ref = S_VAR_REF_GLOB;
	to->b.value.stack2stack_ref = pos;
 
     } else if ( IS_STACK_VALUE( from ) ) {
	
	 if (IS_STACK_VALUE( to )) {
	    pos =  from - ((BINDING_DATA *) g_cur_thread->binding_data_stack.buffer); 
	    to->b.value_flags_ref = S_VAR_REF_STACK2STACK;
	    to->b.value.stack2stack_ref = pos;
	  //to->b.value.heap2stack_ref = pos;
	 
	 } else {
	    to->b.value_flags_ref = S_VAR_REF_HEAP2STACK;
	    make_capture(  &to->b.value.heap2stack_ref, from, 1 , g_cur_thread);
	 }
     } else {
	assert(0);
     }	
     return;
   }
  
 
   to->b.value_flags_ref = 0;
   to->b.value_type = from->b.value_type;
   switch( from->b.value_type ) {
    case S_VAR_INT:
      to->b.value.long_value = from->b.value.long_value;
      break;
    case S_VAR_DOUBLE:
      to->b.value.double_value = from->b.value.double_value;
      break;
    case S_VAR_STRING:
      VALSTRING_init( &to->b.value.string_value );
      VALSTRING_copy( &to->b.value.string_value, &from->b.value.string_value );
      break;
    case S_VAR_CODE:
      VALFUNCTION_copy( &to->b.value.func_value, &from->b.value.func_value  );
      break;
    case S_VAR_HASH:
      VALHASH_init( &to->b.value.hash_value );
      VALHASH_copy( &to->b.value.hash_value, &from->b.value.hash_value, 1 );
      break;
    case S_VAR_LIST:
      VALARRAY_init( &to->b.value.array_value );
      VALARRAY_copy( &to->b.value.array_value, &from->b.value.array_value, 1 );
      break;
    case S_VAR_NULL:
      break;
    default:
      assert(0);
      break;
   }
 }

#if 0
void BINDING_DATA_unlink_ref( BINDING_DATA *src, BINDING_DATA *ref )
{
   SRING *ring = (SRING *) &src->b.backlink; //container;
   SRING *cur, *prev;
   VALFUNCTION_CAPTURE *capture_cur;
   BINDING_DATA *bdata;

   for( cur = ring->next, prev = cur ;  
	cur != ring;  
	prev = cur, cur = CLEAR_LOW_BIT( cur->next )  ) {

   
     if (PTR_LOW_BIT_SET( cur ) ) {
        capture_cur = _OFFSETOF( CLEAR_LOW_BIT( cur ), VALFUNCTION_CAPTURE, next );
 	bdata = (BINDING_DATA *) _OFFSETOF( capture_cur , BINDING_DATA_VALUE, value ); 		
     
	if (bdata == ref) {
	    SRING_unlink_after( prev );
	    return;
	}
     } 
   }
}
#endif

// a captured variable points to a local; this local goes out of scope; 
// Now the local is 
//	- moved to the gc heap.
//	- all capture reference now point to the heap entry. More then one reference may be linked up in list.
void BINDING_DATA_release_captures( BINDING_DATA *binding )
{
   DDDLIST *ring = &binding->b.backlink; //container;
   DDDLIST *cur, *cnext;
   BINDING_DATA *mem;
   VALFUNCTION_CAPTURE *capture_cur;
   BINDING_DATA_VALUE *bdata;

   mem = BINDING_DATA_MEM_new( binding->b.value_type );

   DDDLIST_FOREACH_SAVE(cur, cnext, ring ) {
     capture_cur = _OFFSETOF( cur , VALFUNCTION_CAPTURE, next );
 
     if ((capture_cur->value.stack_ref & SIZE_HIGH_BIT) != 0) {
	 bdata = _OFFSETOF( capture_cur , BINDING_DATA_VALUE, value ); 		
         bdata->value_flags_ref = S_VAR_REF_HEAP;
     }
   
     capture_cur->value.ref  = mem;
     DDDLIST_init( &capture_cur->next );

    
   }

   BINDING_DATA_move( mem, binding );

}

void BINDING_DATA_free( BINDING_DATA *binding )
{
   unsigned char tmp;

   // if this is a pointer to a stack value and pointers to captures exist - then move this value to the stack and adjust capture reference to point to new heap entry.
   if (IS_STACK_VALUE( binding ) && binding->b.backlink.next != 0) { //container != 0 ) {
     BINDING_DATA_release_captures( binding );
     BINDING_DATA_init( binding, S_VAR_NULL );
     return;
   }
   
   if (binding->b.backlink.next != 0) {
     DDDLIST_unlink( &binding->b.backlink );
   }

   switch( binding->b.value_type ) {
    case S_VAR_STRING:
      VALSTRING_free( &binding->b.value.string_value );
      break;
    case S_VAR_CODE:
      VALFUNCTION_free( &binding->b.value.func_value );
      break;
    case S_VAR_HASH: {
      VALHASH *vhash;
      BINDING_DATA *finalize_value;

      vhash = &binding->b.value.hash_value;

      if (! VALHASH_find( vhash, &finalize_key_name, &finalize_value )) {
        if ( finalize_value->b.value_type & S_VAR_CODE ) {
	  VALFUNCTION *func;
	  size_t frame_start;
		
	  // call finalizer on the object.
	  func = &finalize_value->b.value.func_value;
	  frame_start = EVAL_THREAD_prepare_func_call( g_cur_thread, func );
  	  EVAL_THREAD_proceed_func_call( g_cur_thread, frame_start, func );
        }
      }
      VALHASH_free( &binding->b.value.hash_value );
    }
      break;
    case S_VAR_LIST:
      VALARRAY_free( &binding->b.value.array_value );
      break;
    default:
      break;
   }

   tmp = binding->b.value_flags_val;
   BINDING_DATA_init( binding, S_VAR_NULL );
   binding->b.value_flags_val = tmp;
}

size_t BINDING_DATA_hash( BINDING_DATA *binding )
{
   uint32_t *uptr;

   switch( binding->b.value_type ) {
    case S_VAR_INT:
#if POOH_INT_SIZE == 4
     return (size_t) binding->b.value.long_value;
#else
     return (size_t) binding->b.value.long_value ^ (size_t) (binding->b.value.long_value >> 32 );
#endif
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


int  BINDING_DATA_cmp( BINDING_DATA *acmpa, BINDING_DATA *acmpb )
{
  BINDING_DATA *cmpa, *cmpb;

  cmpa = BINDING_DATA_follow_ref( acmpa );
  cmpb = BINDING_DATA_follow_ref( acmpb );

  if (IS_REF( acmpa ) && IS_REF( acmpb ) ) {
     if (cmpa == cmpb) {
       return 0;
     }
  }

  if (cmpa->b.value_type < cmpb->b.value_type) {
    return -1;
  }
  if (cmpa->b.value_type > cmpb->b.value_type) {
    return 1;
  }

  // both values of the same type, can compare them.
  switch( cmpa->b.value_type ) {
    case S_VAR_INT:
      if (cmpa->b.value.long_value < cmpb->b.value.long_value ) {
        return -1;
      }
      if (cmpa->b.value.long_value > cmpb->b.value.long_value ) {
        return 1;
      }
      return 0;

    case S_VAR_DOUBLE:
      if (cmpa->b.value.double_value < cmpb->b.value.double_value ) {
        return -1;
      }
      if (cmpa->b.value.double_value > cmpb->b.value.double_value ) {
        return 1;
      }
      return 0;
    case S_VAR_STRING:
      if (VALSTRING_cmp( &cmpa->b.value.string_value, &cmpb->b.value.string_value ) == 0) {
        return 0;
      }
      break;
    
    case S_VAR_CODE:
      return VALFUNCTION_equal( &cmpa->b.value.func_value, &cmpb->b.value.func_value ); 

    case S_VAR_HASH:
      return VALHASH_equal( &cmpa->b.value.hash_value, &cmpb->b.value.hash_value ); 
    
    case S_VAR_LIST:
      return VALARRAY_equal( &cmpa->b.value.array_value, &cmpb->b.value.array_value );

    default:  
      EVAL_CONTEXT_runtime_error( g_context, "Invalid type %d", cmpa->b.value_type );
   }

   if (cmpa < cmpb) {
     return -1;
   }
   if (cmpa > cmpb) {
     return 1;
   }
   return 0;
}

int BINDING_DATA_check_ref( BINDING_DATA *data, int stage )
{
  BINDING_DATA *ref;


  if (stage == 0) {

    if (data->b.value_flags_val & S_VAR_PRINT_CHECK_LOOPS) {
      return 0;
    }
    data->b.value_flags_val |= S_VAR_PRINT_CHECK_LOOPS;	 

    switch( data->b.value_type ) {
      case S_VAR_HASH:
        g_cur_thread->loop_collection_cnt ++;  
        VALHASH_check_ref( &data->b.value.hash_value, 0 );
	break;
      case S_VAR_LIST:
        g_cur_thread->loop_collection_cnt ++;  
	VALARRAY_check_ref( &data->b.value.array_value, 0);
	break;
      default:
        if (IS_REF(data)) {
          ref = BINDING_DATA_follow_ref( data );

	  if (! is_collection( ref->b.value_type ) ) {  
	    return 0;
	  }

          if (ref->b.value_flags_val & S_VAR_PRINT_CHECK_LOOPS /*|| 
	     ref->b.value_flags & S_VAR_COLL_ENTRY*/) {
            EVAL_THREAD_print_ref_add_loop( data, ref );
	  }
	  BINDING_DATA_check_ref( ref, 0 );
        }
        break;
    }
  } else {
    switch( data->b.value_type ) {
      case S_VAR_HASH:
        VALHASH_check_ref( &data->b.value.hash_value, 1 );
	break;
      case S_VAR_LIST:
	VALARRAY_check_ref( &data->b.value.array_value, 1 );
	break;
      default:
        ref = BINDING_DATA_follow_ref( data );

	if (is_collection(data->b.value_type)) {
	  BINDING_DATA_check_ref( ref, 1 );
	} else {
	  if (data != ref && ref->b.value_flags_val & S_VAR_PRINT_CHECK_LOOPS /*|| 
	    ref->b.value_flags & S_VAR_COLL_ENTRY*/) {
            EVAL_THREAD_print_ref_add_loop( data, ref );
	  }
	}
     }	
  }
  return 0;
}

void BINDING_DATA_print_check_loops( BINDING_DATA *data )
{
  
  g_cur_thread->loop_collection_cnt = 0;
 
  BINDING_DATA_check_ref( data, 0 );
  
  if (g_cur_thread->loop_collection_cnt) {
    BINDING_DATA_check_ref( data, 1 );
  }

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

  // ??? self eferential structures - what is the format to print them ???
  if (level == 0) {
     g_cur_thread->has_loops = 0;
     if (data->b.value_type == S_VAR_HASH || data->b.value_type == S_VAR_LIST || IS_REF(data)) {
       BINDING_DATA_print_check_loops( data );
       free_check_loops = 1;
     }
  }

  data->b.value_flags_val &= ~S_VAR_PRINT_CHECK_LOOPS;

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
#if POOH_INT_SIZE == 4    
        fprintf( out, "%ld", data->b.value.long_value );
#else	
        fprintf( out, "%lld", data->b.value.long_value );
#endif

	break;
    case S_VAR_DOUBLE:
        fprintf( out, "%e", data->b.value.double_value );
	break;
    case S_VAR_STRING:
        VALSTRING_print( out, &data->b.value.string_value  );
	break;
    case S_VAR_CODE:
    case (S_VAR_CODE | S_VAR_CODE_THREAD) :
        VALFUNCTION_print( out, &data->b.value.func_value  );
	break;
    case S_VAR_HASH:
        VALHASH_print( out, &data->b.value.hash_value, level + 1  );
	break;
    case S_VAR_LIST:
	VALARRAY_print( out, &data->b.value.array_value, level + 1  );
	break;
    case S_VAR_NULL:
        fprintf( out, "Null" );
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
//_IO_cookie_io_functions_t impl;
  cookie_io_functions_t impl;
  FILE *fp;
  
  memset( &impl, 0, sizeof( impl ) );

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

  mem->data.b.value_flags_val = S_VAR_HEAP_VALUE;

#if 0
  fprintf(stderr,"new %p\n", &mem->data);
#endif

  return &mem->data;
}

void BINDING_DATA_MEM_free( BINDING_DATA *pdata ) 
{
  BINDING_DATA_MEM *mem;

  mem = _OFFSETOF( pdata, BINDING_DATA_MEM, data );
  DRING_unlink( &mem->heap_entry );
  free(mem);
}

int BINDING_DATA_get_double( BINDING_DATA *data, double *val )
{
  char buf[50];
  VALSTRING *vstr;

  if (IS_REF( data ) ) {
    data = BINDING_DATA_follow_ref( data );
  }
 
  if ((data->b.value_type & S_VAR_NULL) != 0) {
    *val = 0;
    return 0;
  }

  if ((data->b.value_type & (S_VAR_DOUBLE | S_VAR_INT | S_VAR_STRING)) == 0) {
    *val = 0;
    return -1;
  }

  if (data->b.value_type & S_VAR_DOUBLE) {
    *val = data->b.value.double_value;
    return 0;
  }
  
  if (data->b.value_type & S_VAR_INT) {
    *val = data->b.value.long_value;
    return 0;
  }
  
  assert( (data->b.value_type & S_VAR_STRING ) != 0 );
  vstr = &data->b.value.string_value;  
 
  if (vstr->length > sizeof(buf)) {
    EVAL_CONTEXT_runtime_error( g_context, "String literal too long");
  }
  memcpy( buf, vstr->string, vstr->length );
  buf[ vstr->length ] = '\0';

  *val = strtod( buf, 0 );

  return 0;
}

/* ==================================================================================== */


void DATA_REF_set( DATA_REF *ref, BINDING_DATA *data )
{
  ref->data = data;
  ref->is_stack = 0;
      if (IS_STACK_VALUE( data )) {
    ARRAY *b = &g_cur_thread->binding_data_stack;
    ref->is_stack = 1;
    assert( data >= (BINDING_DATA *) ARRAY_at( b, 0 ) && data <= (BINDING_DATA *) ARRAY_at( b, ARRAY_size( b ) - 1 ) );
    ref->idx = data - (BINDING_DATA *) ARRAY_at( b, 0 ); 
    ref->thread = g_cur_thread;
  }
}

BINDING_DATA *DATA_REF_get( DATA_REF *ref )
{
  if (!ref->is_stack)
    return ref->data;
  return (BINDING_DATA *) ARRAY_at( &ref->thread->binding_data_stack, ref->idx );
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

#if __WORDSIZE == 32
  return (uint32_t) key;
#else
  return (uint32_t) ((uint64_t) key) ^ (uint32_t) ( ((uint64_t) key) >> 32);
#endif
}

int EVAL_THREAD_init(EVAL_THREAD *thread, EVAL_THREAD *parent,  EVAL_CONTEXT *context )
{
   VALACTIVATION *activation_record;

   if (ARRAY_init( &thread->binding_data_stack, sizeof(BINDING_DATA), 10 ) ) {
     EVAL_CONTEXT_runtime_error( g_context, "out of memory");
     return -1;
   }
   thread->context = context;
  
#if 0  
   thread->tstate = TSTATE_INIT;
   thread->parent = parent; 
#endif

   thread->yield_rvalue = 0;
   thread->resume_value = 0;

   if (parent) {
      thread->yield_rvalue = BINDING_DATA_MEM_new( S_VAR_LIST );
      thread->resume_value = BINDING_DATA_MEM_new( S_VAR_LIST );
      
      VALARRAY_set_capacity( &thread->yield_rvalue->b.value.array_value, 2 );
      VALARRAY_set_capacity( &thread->resume_value->b.value.array_value, 2 );
   }   

   DRING_init( &thread->threads );
   DRING_push_back( &context->threads, &thread->threads );
 
   if (HASH_init( &thread->print_check_ref_loops, 10, 0, check_loop_cmp, check_loop_hash )) {
     return -1;
   }

   EVAL_THREAD_push_stack( thread , S_VAR_NULL );
   EVAL_THREAD_push_stack( thread , S_VAR_NULL );;
   if (!parent)
     EVAL_THREAD_push_stack( thread , S_VAR_NULL );

   activation_record =  (VALACTIVATION *) thread->binding_data_stack.buffer;

   memset( activation_record, 0, sizeof( VALACTIVATION ));
   activation_record->function_frame_start = 0;
   activation_record->parent_function = (size_t) -1;
   thread->current_activation_record = 0;
   thread->current_function_frame_start = 0;
   thread->instr = 0;
   thread->eval_impl = 0;
   return 0;
}

void EVAL_THREAD_set_current_thread( EVAL_THREAD *thread )
{
   g_context->current_thread = g_cur_thread = thread;
}

EVAL_THREAD * EVAL_THREAD_get_current_thread()
{
   return g_cur_thread;
}


/* suspend the current co-routine, activate the parent co-routine 
 * IN-VALUES:
 *	value_to_yield - value to pass to parent of this thread.
 * OUT-VALUES:
 *	next_msg_from_thread - 	once this co-routine is resumed, this is the value that parent passed to EVAL_THREAD_resume
 *      force_exit -  once this co-routine is resumed - true if co-routine must exit and cleanup
 */
EVAL_THREAD * EVAL_THREAD_yield( BINDING_DATA *value_to_yield, BINDING_DATA **resume_msg, int *force_exit )
{
   EVAL_THREAD *cur_thread = g_cur_thread;

   if (g_context->current_thread == &g_context->main) {
     EVAL_CONTEXT_runtime_error( g_context, "Can't call yield from the main thread, call mkthread to turn this function object into a thread");
   }
  
   *force_exit = g_context->thread_yield_cb( value_to_yield, resume_msg, g_context->thread_ctx );

   return cur_thread;
   
}


/* suspend current runing  co-routine, activate the argument co-routine 
 *  IN-VALUES:
 *	thread - suspended co-routine that will be resumed with this call
 *	msg_to_coroutine - msg that is passed to co-routine, this item will be a return value of EVAL_THREAD_yield
 *  OUT-VALUES:	
 *      is_coroutine_exit - != 0 if coroutine does not return value and has exited
 */      
EVAL_THREAD * EVAL_THREAD_resume(BINDING_DATA *function_obj, BINDING_DATA *resume_msg, BINDING_DATA **yield_value, int *is_coroutine_exit )
{
   EVAL_THREAD *cur_thread = g_cur_thread;


   *is_coroutine_exit = g_context->thread_resume_cb( function_obj, resume_msg, yield_value, g_context->thread_ctx );

   return cur_thread ;


}

EVAL_THREAD * EVAL_THREAD_thread_yield_value( BINDING_DATA *function_obj, BINDING_DATA **yield_value, int *is_coroutine_exit )
{
   VALFUNCTION *func = BINDING_DATA_get_fun( function_obj );
   EVAL_THREAD *thread = func->thread;

   if (thread != 0 ) {
     *is_coroutine_exit = 1;
     *yield_value = thread->yield_rvalue;
   } else {
     *is_coroutine_exit = 0;
     *yield_value = 0;
   }
   return thread;
}

void EVAL_THREAD_stopthread( BINDING_DATA *function )
{
   EVAL_THREAD *cur_thread = g_cur_thread;

   g_context->thread_kill_cb( function, g_context->thread_ctx );
 
   EVAL_THREAD_set_current_thread( cur_thread );
}

void EVAL_THREAD_threadexit()
{
  if (g_context->current_thread == &g_context->main) {
     EVAL_CONTEXT_runtime_error( g_context, "Can't call threadexit from the main thread");
  }
  
  // exit from current thread.
  g_context->thread_exit_cb( g_context->thread_ctx );

  assert(0); // should not get here.
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
  //free_binding_array( &thread->binding_data_stack );
  while( EVAL_THREAD_pop_frame ( thread ) == 0 );
 
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
   
   if (HASH_size( &thread->print_check_ref_loops ) ) {
     HASH_DELETEALL( cur, &thread->print_check_ref_loops )
       free( cur );
     HASH_DELETEALL_END

     //HASH_free( &thread->print_check_ref_loops ); 
    // HASH_init( &thread->print_check_ref_loops, 10, 0, check_loop_cmp, check_loop_hash );
  }   


}

#define FRAME_START_SIZE 2

void EVAL_THREAD_reserve_stack( EVAL_THREAD *thread, size_t frame_size )
{
  if ((thread->binding_data_stack.elmcount + frame_size + 2 ) >  thread->binding_data_stack.elmmaxcount) {
    if (ARRAY_resize( &thread->binding_data_stack, 2 * (thread->binding_data_stack.elmcount + frame_size + 2) ) ) {
       EVAL_CONTEXT_gc( g_context );
       if (ARRAY_resize( &thread->binding_data_stack, 2 * (thread->binding_data_stack.elmcount + frame_size + 2) ) ) {
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

void EVAL_THREAD_copy_binding( BINDING_DATA *to, BINDING_DATA *from )
{
    BINDING_DATA *tmp;
    
    BINDING_DATA_init( to, S_VAR_NULL );

    if ( (from->b.value_flags_ref & (S_VAR_REF_HEAP | S_VAR_REF_GLOB)) != 0 ) { 
	BINDING_DATA_copy( to, from, CP_REF ); 
    } else {
        tmp = BINDING_DATA_MEM_new( S_VAR_NULL );
	BINDING_DATA_copy( tmp, from, CP_REF ); 
#if 0
	if (tmp->b.value_flags_ref & S_VAR_REF_HEAP2STACK) {
	  tmp->b.value.heap2stack_ref.value.thread = thread;
	}
#endif	
 	BINDING_DATA_copy( to, tmp, CP_REF ); // how about the threads ?
    }
}


EVAL_THREAD * EVAL_THREAD_prepare_coroutine( EVAL_THREAD *thread, size_t frame_start, AST_BASE *fdcl )
{
  EVAL_THREAD *new_thread;
  AST_XFUNC_DECL *xfdecl;
  AST_FUNC_DECL * fdecl;
  BINDING_DATA *param, *newparam;
  size_t i, idx, num_params, nsize, ncount;
  VALACTIVATION *arecord;


  // create new thread
  new_thread = (EVAL_THREAD *) malloc( sizeof( EVAL_THREAD ) );
  if (!new_thread) {
    return 0; 
  }
  if (EVAL_THREAD_init( new_thread, thread, g_context )) {
    return 0;
  }

  arecord = (VALACTIVATION *) new_thread->binding_data_stack.buffer ;
  
  // prepare top level stack frame of co-routine.
  arecord->fdecl = fdcl;

  if (fdcl->type == S_XFUN_DECL) {
    xfdecl = (AST_XFUNC_DECL *) fdcl;
    num_params = xfdecl->nparams;
 
    // make room for this function call
    EVAL_THREAD_reserve_stack( new_thread, FRAME_START_SIZE + num_params );
  } else {
    fdecl = (AST_FUNC_DECL *) fdcl;
    num_params = AST_VECTOR_size( fdecl->func_params );
   
    // make room for this function call
    EVAL_THREAD_reserve_stack( new_thread, FRAME_START_SIZE + num_params +  HASH_size( &fdecl->scope_map_name_to_binding ));
  }
 
 
  // allocate stack slot for return value.
  BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) , S_VAR_NULL );

  for( i = 0; i < num_params; i++ ) {
    idx =  frame_start + FRAME_START_SIZE + i; 
    param = ((BINDING_DATA *) thread->binding_data_stack.buffer) +  idx;
    newparam = ((BINDING_DATA *) new_thread->binding_data_stack.buffer) + i + FRAME_START_SIZE;  
 
    EVAL_THREAD_copy_binding( newparam, param );
  }
  new_thread->binding_data_stack.elmcount += num_params; 
  
  
  if (fdcl->type == S_FUN_DECL) {
      nsize = ARRAY_size( &new_thread->binding_data_stack );
      ncount = HASH_size( &fdecl->scope_map_name_to_binding );
      for( i = 0; i < ncount; i++) {
        BINDING_DATA_init( ((BINDING_DATA *) new_thread->binding_data_stack.buffer) + nsize + i /* + 1*/ , S_VAR_NULL );
      }
      new_thread->binding_data_stack.elmcount += ncount; 
  }

  return new_thread;  
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

size_t EVAL_THREAD_prepare_func_call( EVAL_THREAD *thread, VALFUNCTION *func  )
{
  AST_BASE *fdecl = func->fdecl;
  
  if (fdecl->type == S_FUN_DECL) {
      AST_FUNC_DECL *ffdecl = (AST_FUNC_DECL *) fdecl;
      
      return EVAL_THREAD_prepare_call( thread, ffdecl );
  } else {
      assert( fdecl->type == S_XFUN_DECL);
      
      return EVAL_THREAD_prepare_xcall( thread, (AST_XFUNC_DECL *) fdecl );
  }
}

BINDING_DATA * EVAL_THREAD_proceed_func_call( EVAL_THREAD *thread, size_t frame_start, VALFUNCTION *func  )
{
  AST_BASE *fdecl = func->fdecl;
  
  if (fdecl->type == S_FUN_DECL) {
      g_context->invoke_function_cb( frame_start, fdecl, func, g_context->thread_ctx );
  } else {
      assert( fdecl->type == S_XFUN_DECL);
      EVAL_THREAD_call_xfunc( thread, frame_start, (AST_XFUNC_DECL *) fdecl, func );
  }

  return EVAL_THREAD_stack_offset( thread, frame_start );
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
  for( i = 0; i < (ncount + 1); i++) {
    BINDING_DATA_init( ((BINDING_DATA *) thread->binding_data_stack.buffer) + nsize + i  , S_VAR_NULL );
  }

  thread->current_function_frame_start = frame_start;
  thread->binding_data_stack.elmcount += ncount + 1;


}


void EVAL_THREAD_call_xfunc( EVAL_THREAD *thread, size_t frame_start, AST_XFUNC_DECL *decl, VALFUNCTION *function  )
{
   size_t nsize;
   XCALL_DATA xdata;
   
   nsize = ARRAY_size( &thread->binding_data_stack );
   assert( nsize == frame_start + FRAME_START_SIZE + decl->nparams );


   EVAL_THREAD_make_activation_record( thread, &decl->base, function, frame_start );
   
   thread->current_function_frame_start = frame_start;
 //thread->binding_data_stack.elmcount += 1;
 

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
  //
  for( i = top + 2; i < nsize ; i++ ) {
     data = ((BINDING_DATA *) thread->binding_data_stack.buffer) + i; 
     BINDING_DATA_free( data ); 
  }
  
  parent_activation_record = activation_record->parent_function;
  if (parent_activation_record == (size_t) -1) {
    return 1;
  }
  thread->current_activation_record = parent_activation_record;


//thread->instr = activation_record->ret_instr;

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

  EVAL_CONTEXT *ctx = thread->context;
  SHOW_SOURCE_LINE show_lines = ctx->show_source_line; 


  for( frame = 1 , arecord = (VALACTIVATION *) EVAL_THREAD_stack_offset( thread, thread->current_activation_record); 
       arecord->parent_function != (size_t) -1; 
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

     if (fdcl->type == S_FUN_DECL) {
	// show locals.
        fdecl = (AST_FUNC_DECL *) fdcl;

	if (HASH_size( &fdecl->scope_map_name_to_binding ) > AST_VECTOR_size( fdecl->func_params) ) {
	    HASH_Entry *cur;

	    fprintf( out, "\tLocals:\n" );
	    HASH_FOREACH(cur, &fdecl->scope_map_name_to_binding) 	  
		BINDING_ENTRY *entry = (BINDING_ENTRY *) cur;


		if ((size_t) (entry->stack_offset - 2) >= AST_VECTOR_size( fdecl->func_params) ) {

	 	  BINDING_DATA  *value = ((BINDING_DATA *) thread->binding_data_stack.buffer) + arecord->function_frame_start + entry->stack_offset; 
	
	          fprintf( out, "\t\t%s = ", entry->binding_name );
		  BINDING_DATA_print( out, value , 0 );
		  fprintf( out, "\n" );
		}

	    HASH_FOREACH_END 
	}
     }
   
     if ( show_lines ) {
        if (arecord->ret_instr)
	{
	  show_lines( &arecord->ret_instr->location, ctx->show_source_line_ctx );
	  fprintf(out, "\n" );
	}  
     }
     

     if (arecord->parent_function == (size_t) -1) {
       break;
     }
     //fprintf("%s:%d %s\n",  LEXER_get_file_name( LEXCONTEXT *pc, location->file_id), location->first_line,  activation_record->value.func_activation_record.fdecl->fname );
  }
  return 0;
}

// returns 1 if this function is called as a co-routine.
int EVAL_THREAD_is_threadmain( EVAL_THREAD *thread )
{
  return  &g_context->main != thread && thread->current_activation_record == 0;
}


/* ==================================================================================== */
void EVAL_CONTEXT_mark_binding( BINDING_DATA *binding, int is_stack );


int EVAL_CONTEXT_init( EVAL_CONTEXT *context, size_t num_globals )
{
  size_t i;
  BINDING_DATA *data;

  num_globals += 1;

  if (ARRAY_init( &context->bindings_global, sizeof(BINDING_DATA), num_globals ) ) {
    return -1;
  }
  DRING_init( &context->gc_heap );
  DRING_init( &context->threads );

  context->is_jmpbuf_set = 0;
  context->current_thread = &context->main;
  context->show_source_line = 0;
  context->show_source_line_ctx = 0;

  if (EVAL_THREAD_init( &context->main, 0, context )) {
    return -1;
  }

  for( i = 0; i < num_globals ; i++) {
    data = ((BINDING_DATA *) context->bindings_global.buffer) + i;
    BINDING_DATA_init( data , S_VAR_NULL );
    data->b.value_flags_val = S_VAR_GLOB_VALUE;
  }
  STATIC_NULL = (BINDING_DATA *) context->bindings_global.buffer;
  context->bindings_global.elmcount = num_globals;
  context->trace_on = 0; 
  context->event_loop = 0;

  BINDING_DATA_set_tmp_string( &finalize_key_name, "finalizer", -1 );

  disable_sigpipe(); 

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


extern char **environ;

void EVAL_CONTEXT_start( EVAL_CONTEXT *context, char *argv[], int argc )
{

  BINDING_DATA *data;
  int i;

  g_context = context;
  g_cur_thread = context->current_thread; 

  // init ARGV global builtin.
  //
  data = ((BINDING_DATA *) context->bindings_global.buffer) + 3;
  BINDING_DATA_init( data, S_VAR_LIST );
  data->b.value_flags_val = S_VAR_GLOB_VALUE;
  g_context->argv = data;
 
  if (argv) {
    VALARRAY *arval;
    BINDING_DATA *svalue;
    VALSTRING *str_value;

    arval = &data->b.value.array_value;
    for( i = 0; i < argc; i ++ ) {
      svalue = BINDING_DATA_MEM_new( S_VAR_STRING );

      str_value = &svalue->b.value.string_value;
      VALSTRING_appends( str_value, argv[ i ] );
      VALARRAY_set( arval, i, svalue, CP_VALUE );
    }
  }
 
  data = ((BINDING_DATA *) context->bindings_global.buffer) + 4;
  BINDING_DATA_init( data, S_VAR_HASH );
  data->b.value_flags_val = S_VAR_GLOB_VALUE;
  g_context->env = data;

  if (environ) {
    char *s, *sep;
    BINDING_DATA key, *value;
    VALHASH  *hash;
    VALSTRING *str_value;
    

    hash = &data->b.value.hash_value;

    for( i = 0; environ[ i ] != 0; ++ i ) {
        s = environ[ i ];
	sep = strchr( s, '=' );
 
	BINDING_DATA_set_tmp_string( &key, s, sep - s );
	
	value = VALHASH_set_entry( hash, &key );
        BINDING_DATA_init( value, S_VAR_STRING );
	value->b.value_flags_val = S_VAR_HEAP_VALUE;

	
        str_value = &value->b.value.string_value;
 	VALSTRING_appends( str_value, sep + 1 );
    }
  }
  
}

void EVAL_CONTEXT_do_mark_binding( BINDING_DATA *binding )
{
  size_t i;    
  VALARRAY    *parray_value;
  VALHASH     *phash_value;
  VALFUNCTION *pfunc_value;
  BINDING_DATA *key, *value,*pcur;
  VALHASHPOS pos_iter;
  VALFUNCTION_CAPTURE *capture;

 
  switch( binding->b.value_type ) {

    case S_VAR_CODE:
      pfunc_value = &binding->b.value.func_value;
      EVAL_CONTEXT_mark_binding( pfunc_value->this_environment, 0 ); 

      // visit captures as well (may include pointers to heap values).
      for( i = 0; i < pfunc_value->ncaptures; i ++ ) {
         capture = &pfunc_value->pcaptures[ i ];
         if (capture->next.next == 0) {
	    EVAL_CONTEXT_mark_binding( capture->value.ref, 0 );
	 }
      }

      for( i = 0; i < pfunc_value->nclosures; i ++ ) {
         value = pfunc_value->pnested_closures[ i ];
	 if (value != 0 ) {
	   EVAL_CONTEXT_mark_binding( value, 0 );
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
//    for( pcur = (BINDING_DATA *) parray_value->data, pend = pcur + parray_value->size; pcur != pend; ++pcur )
      for( i = 0; i < parray_value->size; i ++ ) {
        pcur = parray_value->data[ i ]; 
	if (pcur) {
	 EVAL_CONTEXT_mark_binding( pcur, 1 ); 
        }
      }
      break;

    default:
      break;
  }
}
   

void EVAL_CONTEXT_mark_binding( BINDING_DATA *binding, int is_stack )
{
  (void) is_stack;
 
  while ( (binding->b.value_flags_val & S_VAR_HEAP_VALUE) == 0)   {

    if (binding->b.value_flags_ref & S_VAR_REF_HEAP) {  
      binding = binding->b.value.value_ref;
    } else {
      if ((binding->b.value_type & (S_VAR_CODE | S_VAR_HASH | S_VAR_LIST)) != 0) {
	EVAL_CONTEXT_do_mark_binding( binding );
      }
      return;
    }
  }

ismarked:

  if ( binding->b.value_flags_val & S_VAR_HEAP_GC_MARK ) {
    return;
  }

#if 0
  fprintf(stderr,"mark %p\n", binding);  
#endif  
  binding->b.value_flags_val |= S_VAR_HEAP_GC_MARK; 
  
  if (binding->b.value_flags_ref & S_VAR_REF_HEAP) {  
      binding = binding->b.value.value_ref;
      assert( binding->b.value_flags_val & S_VAR_HEAP_VALUE);
      goto ismarked;
  }
   
  EVAL_CONTEXT_do_mark_binding( binding );

#if 0
  if (!is_stack && binding->b.container != 0) {
     EVAL_CONTEXT_mark_binding( (BINDING_DATA *) binding->b.container, 0 ); 
  }
#endif  
}


void EVAL_CONTEXT_mark_bindings( ARRAY *bindings)
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
    EVAL_CONTEXT_mark_binding( binding, 0 );

    if (binding == start) {
      break;
    }  
    binding --;

  } while( 1 );
}

void EVAL_CONTEXT_mark_stack( EVAL_THREAD *cur_thread )
{
  BINDING_DATA *binding;
  VALACTIVATION *pactivation_record;
  size_t activation_record, eof, i;
  ARRAY *bindings;

  activation_record = cur_thread->current_activation_record; 
  eof = ARRAY_size( &cur_thread->binding_data_stack );

  bindings = &cur_thread->binding_data_stack; 
  for(;;) {
    //binding = (BINDING_DATA *) ARRAY_at( bindings, activation_record );
    //EVAL_CONTEXT_mark_binding( binding, 1 );

    //
    //for( i = activation_record + 2; i < eof; i++) {
    for( i = eof - 1; i > (activation_record + 1) ; i--) {

	binding = (BINDING_DATA *) ARRAY_at( bindings, i );
	EVAL_CONTEXT_mark_binding( binding, 1 );
    }


    pactivation_record =  (VALACTIVATION *) ( ((BINDING_DATA *) bindings->buffer) + activation_record + 1);
    //if (pactivation_record->function_object

    if (activation_record == 0) {
	break;
    }

    eof = activation_record;
    activation_record = pactivation_record->parent_function;
  } 
}


void EVAL_CONTEXT_mark( EVAL_CONTEXT *context )
{
   DRING *cur_thread_base;
   EVAL_THREAD *cur_thread;
   
   EVAL_CONTEXT_mark_bindings( &context->bindings_global );	
   
   // for each thread
   DRING_FOREACH( cur_thread_base, &context->threads )
   {  
     cur_thread = _OFFSETOF( cur_thread_base, EVAL_THREAD, threads );

     if (cur_thread->yield_rvalue){
	EVAL_CONTEXT_mark_binding( cur_thread->yield_rvalue, 0 );
        EVAL_CONTEXT_mark_binding( cur_thread->resume_value, 0 );
     }	
     EVAL_CONTEXT_mark_stack( cur_thread );	 
   }

   EVAL_CONTEXT_mark_binding( context->env, 0 );
   EVAL_CONTEXT_mark_binding( context->argv, 0 );


}

void EVAL_CONTEXT_sweep( EVAL_CONTEXT *context )
{
  DRING *cur, *next;
  BINDING_DATA_MEM *mem;

  DRING_FOREACH_SAVE( cur, next, &context->gc_heap )
  {
    mem  = (BINDING_DATA_MEM *) cur;
    if ( (mem->data.b.value_flags_val & S_VAR_HEAP_GC_MARK) == 0) {

#if 0
      fprintf(stderr,"gc-free-%p\n", &mem->data);    
#endif

      // container and its members are marked independently, so do not
      // need to delete contained items for container. sweep will take care of them.s
      DRING_unlink( cur );
      free(cur);
    } else {
      mem->data.b.value_flags_val &= ~S_VAR_HEAP_GC_MARK;
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

  if (format != 0)
  {
    va_start(ap, format);
    len = vsnprintf( msg, sizeof(msg) - 1, format, ap );
    fprintf( stderr, "%s\n", msg );
  }

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

