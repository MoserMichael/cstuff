/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include <cutils/bhash.h>
#include <cutils/util.h>
#include <cutils/hashfunction.h>



#define ADJUST_HASH(hash,buckets) ((hash) & ((buckets) - 1))



int HASH_init(
					HASH			*hash,
					size_t				 buckets, 
					int                  ismultimap,

					HASH_COMPARE_KEY compare_key,
					HASH_FUNCTION hash_func
					)
{
	size_t i;

	buckets = UTIL_round_to_power_of_n( buckets );


	hash->elmcount = 0;

        if (!hash_func) {
	  hash->hash_func = HASHFUNCTION_Bob_Jenkins_one_at_a_time;
        } else {
	  hash->hash_func = hash_func;
        }
	hash->compare_key = compare_key;

	hash->ismultimap = ismultimap;

	hash->buckets = (SRING *) malloc( sizeof(SRING) * buckets );
	if (!hash->buckets) {
		return -1;
	}
	hash->buckets_size = buckets;
	
	for(i=0;i<buckets;i++) {
	  SRING_init(  &hash->buckets[i] );
  	}


	hash->mult_load_factor = 2;
        hash->div_load_factor = 3;

	hash->resize_threshold = (buckets * hash->mult_load_factor) / hash->div_load_factor;

	return 0;
}




int HASH_resize(HASH	*hash, size_t buckets)
{
	SRING *pbuckets;
	size_t i;
	HASH_Entry *cur;
	size_t elmcount;

	buckets = UTIL_round_to_power_of_n( buckets );

	pbuckets = malloc( sizeof(SRING) * buckets);
	if (!pbuckets) {
		return -1;
	}
	for(i=0;i<buckets;i++) {
		SRING_init( pbuckets + i );
	}

	elmcount = hash->elmcount;

	HASH_DELETEALL(cur,hash)
		i = ADJUST_HASH(  cur->hash, buckets);
		SRING_push_front( pbuckets + i, (SRING *) cur);
	HASH_DELETEALL_END

	free( hash->buckets );

	hash->buckets = pbuckets;
	hash->buckets_size = buckets;
	hash->elmcount = elmcount;


	hash->resize_threshold = (buckets * hash->mult_load_factor) / hash->div_load_factor;

	return 0;

}

HASH_Entry *HASH_find_in_bucket(	
					
					SRING		*abucket, 
					HASH_VALUE   	 hash, 
					HASH_COMPARE_KEY compare_key,
					void		*key, 
					ssize_t		 key_size )
{
	SRING	*pos;

	SRING_FOREACH(pos, abucket) {
		HASH_Entry *entry = (HASH_Entry *) pos;
		if (entry->hash == hash) 
		{
			if (compare_key( entry, key, key_size ) == 0) {
				return entry;
			}
		}
	}
 	return 0;
}

HASH_Entry *HASH_find( HASH *phash, void *key, ssize_t key_size )
{
	HASH_VALUE hash = phash->hash_func( key, key_size );
	int		bucket =   ADJUST_HASH(hash, phash->buckets_size);
	SRING	*abucket = &phash->buckets[ bucket ];   
	
	return HASH_find_in_bucket( abucket, hash, phash->compare_key, key, key_size );
}

HASH_Entry *HASH_find_next( HASH *phash, HASH_Entry *prev, void *key, ssize_t key_size )
{
	HASH_Entry *next;

	if (!phash->ismultimap) {
		return 0;
	}

	next = (HASH_Entry *) prev->entry.next;
	if (!next) {
		return 0;
	}
	if (next->hash != prev->hash) {
		return 0;
	}
	if (phash->compare_key( next, key, key_size ) != 0) {
		return 0;
	}
	return next;
}

int HASH_insert( HASH *phash, HASH_Entry *entry, void *key, ssize_t key_size  )
{
	HASH_VALUE hash;
	int		bucket;
	SRING	*abucket,*fnd;

	if ((phash->elmcount + 1) >= phash->resize_threshold) {

	   if (HASH_resize(phash, 2 * phash->buckets_size)) {
	     return -1;
	   }
	}

	hash = phash->hash_func( key, key_size );	
	entry->hash = hash;

	bucket =   ADJUST_HASH(hash, phash->buckets_size);
	abucket = &phash->buckets[ bucket ];   
	
	
	fnd = (SRING *) HASH_find_in_bucket( abucket, hash, phash->compare_key, key, key_size );

	/* multimap allows only one entry with the given key */
	if (!phash->ismultimap && fnd) {
		return -1;
	}

	if (fnd) {
		SRING_insert_after( fnd, &entry->entry );
	} else {
		SRING_push_front( abucket,  &entry->entry );
	}
	phash->elmcount++;

	return 0;
}

HASH_Entry * HASH_unlink( HASH *phash, void *key, ssize_t key_size )
{
	HASH_VALUE hash;
	int		bucket;
	SRING	*abucket;
	SRING	*pos,*prev;
	HASH_Entry *ret = 0;


	hash =  phash->hash_func( key, key_size );	
	bucket =   ADJUST_HASH(hash, phash->buckets_size);
	abucket = &phash->buckets[ bucket ];   

	prev = abucket;
	SRING_FOREACH(pos, abucket) {
		HASH_Entry *entry = (HASH_Entry *) pos;

		if (entry->hash == hash) {
		  
			if (phash->compare_key( entry, key, key_size ) == 0) {
				ret = (HASH_Entry *) SRING_unlink_after(prev);
				phash->elmcount--;	
				return ret;
			}
		}
		prev = pos;
	}

	return 0;
}



