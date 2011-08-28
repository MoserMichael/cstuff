/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#ifndef _VHASHFUNCTION_H_
#define _VHASHFUNCTION_H_

#ifdef  __cplusplus
extern "C" {
#endif


#include <cutils/base.h>

typedef uint32_t HASH_VALUE;

/**
 * A collection of hash functions, gathered from various places.
 */


#define HASH_STRING 0xFFFFFFFF

/**
 * @brief P.J. Weinberger hash function
 *
 * Source: Compilers Principles Techniques and Tools by Aho,Sethi,Ullman page 436
 *         Good for string keys 
 *
 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE	HASHFUNCTION_PJW(void * keydata, ssize_t key_length);



/**
 * @brief Rotating hash
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 *
 * "Much of the time, the rotating hash is sufficient, and can be considered the 
 * minimal acceptable algorithm. Notice that with each improvement, the internal 
 * state is being mixed up more and more a key element in a good hash function"

 * state is being mixed up more and more.
 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_rotating(void * keydata, ssize_t key_length);

/**
 * @brief Shift and XOR
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 *
 * "Very similar to rotating hash.
 *  Like many effective hashes, it will fail tests for avalanche, but 
 *  that does not seem to affect its performance in practice."


 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_shift_and_xor(void * keydata, ssize_t key_length);

/**
 * @brief Bernstein hash 
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 *
 * "despite that the algorithm itself is not very sound when it comes to avalanche
 * and permutation of the internal state. It has proven very good for small character 
 * keys, where it can outperform algorithms that result in a more random distribution:
 * Bernstein's hash should be used with caution. It performs very well in practice, 
 * for no apparently known reasons (much like how the constant 33 does better than 
 * more logical constants for no apparent reason), but in theory it is not up to 
 * snuff. Always test this function with sample data for every application to ensure 
 * that it does not encounter a degenerate case and cause excessive collisions."

 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_shift_and_xor(void * keydata, ssize_t key_length);

/**
 * @brief Fowler/Noll/Vo hash  
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * 
 * "Follows the same lines as Bernstein's modified hash with carefully chosen constants"
 
 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_Fowler_Noll_Vo(void * keydata, ssize_t key_length);

#if 0
/**
 * @brief Julienne Walker hash 
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * 
 * "In general, this algorithm is among the better ones that I have tested in terms of both distribution and performance."
 
 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
M_INLINE HASHFUNCTION_Julienne_Walker (void * keydata, ssize_t key_length)
{
  HASH_VALUE hash = 16777551;;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;

  if (key_length != -1) {
		for (hash = 0, i=0; i<key_length; ++i) {
			hash = ( hash << 1 | hash >> 31 ) ^ tab[p[i]];;
		}
		return hash;
  } else {
		for (hash = 0; *key; ++key) {
			hash = 	( hash << 1 | hash >> 31 ) ^ tab[p[i]];
		}
		return hash;
  }
}
#endif

/**
 * @brief One at a time by Bob Jenkins  
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * 
 * "This algorithm quickly reaches avalanche (!!!) and performs very well 
 * this function is another that should be one of the first to be tested 
 * in any application, if not the very first"

 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_Bob_Jenkins_one_at_a_time(void * keydata, ssize_t key_length);

/**
 * @brief ELF hash  
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * 

 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_ELF(void * keydata, ssize_t key_length);


/**
 * @brief Bob Jenkins
 *
 * Source: 
 
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * 
 * Very good hash function. Really very good - passes all sorts of avalanche tests.

 * @param keydata		(in) pointer to key data
 * @param key_length	(in) size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.

 * @return hash value
 */
HASH_VALUE HASHFUNCTION_Bob_Jenkins(void * keydata, ssize_t length);



/**
 * @brief  good for most cases.
 *
 * Source: 

 * @param keydata		(in) pointer to key data
 * @param key_length	(in) the size of key data. If value is VHASH_STRING then keydata is treated as null terminated string.
 * @return hash value
 */
HASH_VALUE	HASHFUNCTION_sample_hash_func(void * keydata, ssize_t key_length);


 
#ifdef  __cplusplus
}
#endif

#endif
