/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef	_VBUCKETHASH_H_
#define _VBUCKETHASH_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <cutils/sring.h>
#include <cutils/hashfunction.h>

/**
 * @defgroup HASH_entry
 * @brief Entry in bucket hash table, add to structure as member in order to make structure storable in hash table.
 * Each hash table entry has to embed a VBUCKETHASH_Entry as part of its structure.
 * The user of this hash thing can please key/value wherever he likes.  
 * @{
 */
typedef struct  {
	SRING	    entry;
	HASH_VALUE  hash;
}
	HASH_Entry;



/**
 * compute hash value
 */ 
typedef HASH_VALUE	(*HASH_FUNCTION) 	(const void *data, ssize_t length);


/**
 * Compare argument key with hash key embedded in entry.
 */
typedef int     	(*HASH_COMPARE_KEY)	(HASH_Entry *, const void *key, ssize_t key_size);

/** 
 * Visit a hash entry.
 */
typedef int		(*HASH_VISITOR)		(HASH_Entry *, void *context);


/*
 * @}
 */

/**
 * @defgroup HASH
 * @brief Hash table that is implemented as bucket hash table.

 * Element of has is an user defined entry which embeds HASH_Entry structure.
 * 
 * @{				
 */
typedef struct {

	SRING *buckets;

	int     ismultimap;
	size_t	buckets_size;
	size_t	elmcount;
	size_t  resize_threshold;

	int     mult_load_factor,div_load_factor;
	

	HASH_FUNCTION  hash_func;
	HASH_COMPARE_KEY compare_key;
}
  HASH;




/** 
 *  @brief Object constructor; initialise the hash that allows one entry for a given key.

 *  @param hash			(out) the object.
 *  @param buckets		(in) number of buckets - will be rounded to closes power of two (i.e. number of arrays of linked lists structures, each separate list holds elements with same hash number).
 *  @param hash_func		(in) pointer to hash function
 *  @param compare		(in) pointer to compare function; compares if two keys are equal, (called when hash value of two entries is equal)
 
 *  @returns 0 if ok
 */                                                         	
int HASH_init(
					HASH	*hash, 
					
					size_t	buckets,
					int     ismultimap,

					HASH_COMPARE_KEY compare_key,
					HASH_FUNCTION hash_func
					);





/** 
 * @brief resize the hash table

 * Change number of buckets in hash table. 
 * The function pass over each element in the hash table and insert it into the resized table.
 
 * @param hash (in out) the object
 * @param buckets (in) new number of buckets
 */
int HASH_resize(HASH *hash, size_t buckets);

/**
 * @brief Object destructor; frees a hash table.

 * All elements currently contained in hash table will remain in memory.

 * @param hash (in out) the object
 */
M_INLINE void	HASH_free(HASH	*hash)
{
   if (hash->buckets) {
	free(hash->buckets);
		hash->buckets = 0;
   }
}



/** 
 * @brief returns number of elements in hash table
 */
M_INLINE size_t HASH_size( HASH *hash) 
{
	return hash->elmcount;
}


/**
 * @brief find entry with given key in hash table; for multimaps this will return the first occurence of the key.

 * @param phash		(in) the object
 * @param key		(in) pointer to key
 * @param key_size	(in) size of key 
 
 * @return pointer to entry if found; NULL if not found
 */
HASH_Entry *HASH_find( HASH *phash, const void *key, ssize_t key_size );


/**
 * @brief for multimaps - find next occurence of key
 *
 * Usage to retrive all occurences of a key 
 *
 *
 * HASH_Entry * entry;
 * for( entry = HASH_find(hash, key, sizeof(key) ) ; entry; 
 *	entry =  HASH_find_next(hash, entry, key, sizeof(key) ) {
 *
 * }
 *
 *
 * @param phash (in) the object
 * @param prev	(in) value returned by first invocation of VBUCKETHASH_find or previous invocation of VBUCKETHASH_find_next
 * @param key	(in) pointer to key
 * @param key_size (in) size of key. If key is a null terminated string then pass VHASH_STRING as value.
 * @return pointer to entry if found; NULL if not found
 */
HASH_Entry *HASH_find_next( HASH *phash, HASH_Entry *prev, const void *key, ssize_t key_size );

/**
 * @brief insert new entry in hash table
 * @param hash (in) the object
 * @param key  (in) key 
 * @param size (in) size of key. If key is a null terminated string then pass VHASH_STRING as value.
 */
int HASH_insert( HASH *phash, HASH_Entry *entry, const void *key, ssize_t key_size  );

/**
 * @brief find first hash table entry and unlink it from its bucket. The caller of this function has to free memory held by hash table entry.
 * @param hash
 * @param key
 * @param size
 * @returns if entry with given keys exists - pointer to hash table entry.
 */
 HASH_Entry * HASH_unlink( HASH *hash, const void *key, ssize_t key_size );


/* internal macro not directly used by user */
#define HASH_BUCKET_FOREACH(cur,bucket) \
	for((cur) = (HASH_Entry *) ((bucket)->next);\
		(SRING *) (cur) != (bucket);\
		(cur) = (HASH_Entry *) (cur)->entry.next )


/**
 * @brief Macro: iterate over all elements that match a given key (multimap)
 *
 * Usage:
 *		HASH_Entry *cur;
 *
 *		HASH_FOREACH_KEY(cur,hash)  {
 *			=do something with cur - don't delete it=
 *		}
 *
 * @param cur  (type HASH_Entry *) current entry
 * @param hash (type HASH *) the object
 * @param key  (type void *) the key
 * @param key_size  (type size_t) length of key
 */
#define HASH_FOREACH_KEY(cur,hash,key,key_size) \
	 for( (cur) = HASH_find( (hash), (key), (key_size ) ) ; \
		  (cur); \
   		  (cur) =  HASH_find_next( (hash), (cur), (key), (key_size) ) )


/**
 * @brief Macro: unlink al entries that match a given key (for multimap); the user has to free the memory in loop code. 
 
 * @param cur  (type HASH_Entry *) current entry
 * @param hash (type HASH *) the object
 * @param key  (type void *) the key
 * @param key_size  (type size_t) length of key
 
 */
#define HASH_DELETEALL_KEY(cur,hash,key,key_size) \
{  \
	HASH_Entry * HASH_DELETEALL_KEY##next = (HASH_Entry *) 0xFFFFFFFF; \
	for( (cur) = HASH_unlink( (hash), (key), (key_size ) ) ; \
		 (cur) && (HASH_DELETEALL_KEY##next); \
		 (HASH_DELETEALL_KEY##next) =  HASH_find_next( (hash), (cur), (key), (key_size) ) ) { \
		 if ((HASH_DELETEALL_KEY##next) != (HASH_Entry *) 0xFFFFFFFF) { \
			 SRING_unlink_after( (SRING *) (cur) ); \
			 (hash)->elmcount--; \
			 (cur) = (HASH_DELETEALL_KEY##next); \
		 }

#define HASH_DELETEALL_KEY_END \
	} \
}	


/**
 * @brief Macro: iterate over all elements in hash table
 * @param cur (type HASH_Entry *)
 * @param hash (type HASH *)
 */
#define HASH_FOREACH(cur,hash) \
{ \
	size_t HASH_foreach_i##cur = 0; \
	for(HASH_foreach_i##cur = 0; HASH_foreach_i##cur < (hash)->buckets_size; HASH_foreach_i##cur ++) \
		HASH_BUCKET_FOREACH( cur, &(hash)->buckets[ HASH_foreach_i##cur ] ) {
		


/** @brief Macro: close block opened by HASH_FOREACH
 */
#define HASH_FOREACH_END \
	}\
}

/**
 * @brief Macro: iterate over all elements in hash table and delete them from the table; allow the user to free the memory of each element.
 * @param cur (type HASH_Entry *)
 * @param hash (type HASH *)
 */
#define HASH_DELETEALL(cur,hash) \
{ \
	size_t HASH_delallforeach_i##cur = 0; \
	for(HASH_delallforeach_i##cur = 0; HASH_delallforeach_i##cur < (hash)->buckets_size; HASH_delallforeach_i##cur ++) \
	while( !SRING_isempty( &(hash)->buckets[ HASH_delallforeach_i##cur ]) ) { \
		(cur) = (HASH_Entry *) SRING_unlink_after( &(hash)->buckets[ HASH_delallforeach_i##cur ] );\
		(hash)->elmcount--;

/** 
 * @brief Macro: close block opened by HASH_DELETEALL
 */			
#define HASH_DELETEALL_END \
	}\
}


/** 
 * @brief iterate over all entries of the hash table that match a given key and invoke callback with those elements.
 *
 * @param hash		(in) the object

 * @param key		(in) key
 * @param key_size	(in) size of key

 * @param eval_func	(in) function invoked for every element in hash table.
 * @param context	(in) data pointer passed to every invocation of eval_func
 */

M_INLINE void HASH_foreach_key( HASH *hash, void *key, ssize_t key_size, HASH_VISITOR eval_func, void *context )
{
	HASH_Entry *cur;

	if (!eval_func) {
		return;
	}
	HASH_FOREACH_KEY(cur,hash,key,key_size) {
		eval_func( cur, context);
	}
}


/** 
 * @brief iterate over all entries of the hash table and invoke callback with each element.
 * Equivalent of Lisp foldl,mapcar and friends.
 * @param hash 
 * @param eval_func  function invoked for every element in hash table.
 * @param context data pointer passed to every invocation of eval_func
 */
M_INLINE void HASH_foreach( HASH *hash, HASH_VISITOR eval_func, void *context )
{
	HASH_Entry *cur;

	if (!eval_func) {
		return;
	}

	HASH_FOREACH(cur,hash)
		eval_func( cur, context);
	HASH_FOREACH_END
}


/**
 * @brief find an element within the hash. callback is invoked for each element of the list, when the callback returns non zero value the iteration stops as we have found what we searched for. 
 * 
 * @param hash (in) the object.
 * @param eval_func (in) callback that is called to visit each set (or cleared) bit.
 * @param context (in) pointer that is passed as context on each invocation of callback.
 * @param retval (out) optional - the first non zero value returned by eval callback, if NULL then ignored.
 * @return the list element that has matched (i.e. element that has been found).
 *
 */
M_INLINE HASH_Entry * HASH_findif( HASH *hash, HASH_VISITOR eval_func, void *context, int *retval)
{
	size_t i;
	SRING *cur;
	int ret;	

	if (!eval_func) {
		return 0;
	}

	for(i=0; i<hash->buckets_size; i++) {
			SRING_FOREACH( cur, &hash->buckets[i] ) {
					ret = eval_func( (HASH_Entry*) cur, context);
					if (ret) {
						if (retval) {
							*retval = ret;
						}
						return (HASH_Entry *) cur;
					}		
			}
	}	

	return 0;
}

/** 
 * @brief iterate over all entries of the hash table and delete them.
 * @param hash			 (in) the object 
 * @param on_delete		 (in) optional callback, call prior to freing object.
 * @param offset_of_link (in) offset of HASH_Entry in embedded structure.
 */
M_INLINE void HASH_deleteall( HASH *hash, int offset_of_link, HASH_VISITOR on_delete, void *context )
{
	HASH_Entry *cur;

	HASH_DELETEALL(cur,hash);
		if (on_delete) {
			on_delete(cur,context);
		} 
		free( _MEMBEROF(cur,offset_of_link) );
	HASH_DELETEALL_END
}


/**
 * @brief check consistency of bucket hash structure.
 */
M_INLINE int HASH_check(HASH *hash)
{
	size_t	i,nbuck = hash->buckets_size;
	size_t  count_of_elem = 0;
	HASH_Entry  *cur;
	
	for(i=0; i < nbuck; i++) {
		if (!SRING_check(hash->buckets + i)) {
		   return 0;
		}
	}

	HASH_FOREACH(cur,hash)
		count_of_elem++;
	HASH_FOREACH_END
	
	if (count_of_elem != hash->elmcount) {
		return 0;
	}

	return 1;
}

/**
 @}
*/

#ifdef  __cplusplus
}
#endif

#endif
