/* Copyright (c) Michael Moser (2011) . 3-clause BSD License applies */

#include "hashfunction.h"



HASH_VALUE	HASHFUNCTION_sample_hash_func(const void * keydata, ssize_t key_length)
{
	unsigned char * key = (unsigned char *) keydata;
	HASH_VALUE hash_val = 0;
		
	if (key_length != -1 ) {
		do {
			hash_val = (hash_val<<5) + hash_val + *key++;	
			key_length --;
		} while (key_length != 0);
		return hash_val;
	} else {
		

		while (*key  ) {
			hash_val = (hash_val<<5) + hash_val + *key++;

		}
		return hash_val;
	}

}

HASH_VALUE	HASHFUNCTION_PJW(const void * keydata, ssize_t key_length)
{
	unsigned char * key = (unsigned char *) keydata;
	HASH_VALUE g = 0, hash_val = 0;

	if (key_length != -1) {
		do {
			hash_val = (hash_val<<4) + hash_val + *key++;	
			g = hash_val & 0xF0000000;
			if (g) {
				hash_val = hash_val ^ ( g >> 24);
				hash_val = hash_val ^ g;
			}
			key_length --;
		} while (key_length != 0);

		return hash_val;
	} else {
		

		while (*key ) {
			hash_val = (hash_val<<4) + hash_val + *key++;	
			g = hash_val & 0xF0000000;
			if (g) {
				hash_val = hash_val ^ ( g >> 24);
				hash_val = hash_val ^ g;
			}
		}
		return hash_val;
	}
}

HASH_VALUE HASHFUNCTION_rotating(const void * keydata, ssize_t key_length)
{
  HASH_VALUE hash;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;

  if (key_length != -1) {
		for (hash = key_length, i=0; i<key_length; ++i) {
			hash = (hash<<4)^(hash>>28) ^ key[i];
		}
		return hash;
  } else {
		

		for (hash = ((size_t)*key) << 16; *key  ; ++key) {
			hash = (hash<<4)^(hash>>28) ^ (*key);
		}
		return hash;
  }
}

HASH_VALUE HASHFUNCTION_shift_and_xor(const void * keydata, ssize_t key_length)
{
  HASH_VALUE hash;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;

  if (key_length != -1) {
		for (hash = key_length, i=0; i<key_length; ++i) {
			hash ^= (hash<<5) + (hash>>2)  + key[i];
		}
		return hash;
  } else {
		

		for (hash = ((size_t)*key) << 16; *key ; ++key) {
			hash ^= (hash<<5) + (hash>>2)  + (*key);
		}
		return hash;
  }
}


HASH_VALUE HASHFUNCTION_Fowler_Noll_Vo(const void * keydata, ssize_t key_length)
{
  HASH_VALUE hash = 2166136261U;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;

  if (key_length != -1) {
		for (hash = 0, i=0; i<key_length; ++i) {
			hash = ( hash * 16777619U ) ^ key[i];
		}
		return hash;
  } else {
		

		for (hash = 0; *key ; ++key) {
			hash = ( hash * 16777619U ) ^ (*key);
		}
		return hash;
  }
}

HASH_VALUE HASHFUNCTION_Bob_Jenkins_one_at_a_time(const void * keydata, ssize_t key_length)
{
  HASH_VALUE h = 2166136261U;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;


  if (key_length != -1) {
	
	for ( i = 0; i < key_length; i++ ) {
		h += key[i];
		h += ( h << 10 );
		h ^= ( h >> 6 );
	}

	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );

	return h;

  } else {
	

	for ( ; *key ; ++key ) {
		h += *key;
		h += ( h << 10 );
		h ^= ( h >> 6 );
	}

	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );

	return h;
  }
}

HASH_VALUE HASHFUNCTION_ELF(const void * keydata, ssize_t key_length)
{
  HASH_VALUE h = 2166136261U,g;
  unsigned char * key = (unsigned char *) keydata;
  ssize_t i;

  if (key_length != -1) {
	
	for ( i = 0; i < key_length; i++ ) {
		h = ( h << 4 ) + key[i];
		g = h & 0xf0000000L;

		if ( g != 0 )
			h ^= g >> 24;
		
		h &= ~g;
	}
	return h;
  
  } else {
		

	for ( ; *key ;key++ ) {
		h = ( h << 4 ) + *key;
		g = h & 0xf0000000L;

		if ( g != 0 )
			h ^= g >> 24;
		
		h &= ~g;

		return h;
	}

	return h;
  }
}

 #define hashsize(n) ( 1U << (n) )
 #define hashmask(n) ( hashsize ( n ) - 1 )
 
 #define mix(a,b,c) \
 { \
   a -= b; a -= c; a ^= ( c >> 13 ); \
   b -= c; b -= a; b ^= ( a << 8 ); \
   c -= a; c -= b; c ^= ( b >> 13 ); \
   a -= b; a -= c; a ^= ( c >> 12 ); \
   b -= c; b -= a; b ^= ( a << 16 ); \
   c -= a; c -= b; c ^= ( b >> 5 ); \
   a -= b; a -= c; a ^= ( c >> 3 ); \
   b -= c; b -= a; b ^= ( a << 10 ); \
   c -= a; c -= b; c ^= ( b >> 15 ); \
 }
 
#define BJ_INIT_VAL 2166136261U

HASH_VALUE HASHFUNCTION_Bob_Jenkins(const void * keydata, ssize_t length)
{
    unsigned char *k =  (unsigned char *) keydata;
    unsigned a, b;
    unsigned c = BJ_INIT_VAL;
    unsigned len = length;
 
    a = b = 0x9e3779b9;

	if (length != -1) {
 
		while ( len >= 12 ) {
		   a += ( k[0] + ( (unsigned)k[1] << 8 ) 
			 + ( (unsigned)k[2] << 16 )
			 + ( (unsigned)k[3] << 24 ) );
		   b += ( k[4] + ( (unsigned)k[5] << 8 ) 
			 + ( (unsigned)k[6] << 16 )
			 + ( (unsigned)k[7] << 24 ) );
		   c += ( k[8] + ( (unsigned)k[9] << 8 ) 
			 + ( (unsigned)k[10] << 16 )
			 + ( (unsigned)k[11] << 24 ) );
 
		   mix ( a, b, c );
 
		   k += 12;
		   len -= 12;
		}
	} else {
		size_t pos_k = 0;
		unsigned char *next_k;
			

		length = 0;

		while(1) {

			for(pos_k = 0, next_k = (unsigned char *) k;
				*next_k && pos_k<12 ;
				pos_k++, next_k++);

			length += pos_k;
				
			if (pos_k < 12) {
				len = pos_k;
				break;
			}
	
			a += ( k[0] + ( (unsigned)k[1] << 8 ) 
			  + ( (unsigned)k[2] << 16 )
			  + ( (unsigned)k[3] << 24 ) );
			b += ( k[4] + ( (unsigned)k[5] << 8 ) 
			  + ( (unsigned)k[6] << 16 )
			  + ( (unsigned)k[7] << 24 ) );
			c += ( k[8] + ( (unsigned)k[9] << 8 ) 
			  + ( (unsigned)k[10] << 16 )
			  + ( (unsigned)k[11] << 24 ) );

			mix ( a, b, c );

			k += 12;

		}
	}
 
    c += length;
 
    switch ( len ) {
    case 11: c += ( (unsigned)k[10] << 24 );
    case 10: c += ( (unsigned)k[9] << 16 );
    case 9 : c += ( (unsigned)k[8] << 8 );
    /* First byte of c reserved for length */
    case 8 : b += ( (unsigned)k[7] << 24 );
    case 7 : b += ( (unsigned)k[6] << 16 );
    case 6 : b += ( (unsigned)k[5] << 8 );
    case 5 : b += k[4];
    case 4 : a += ( (unsigned)k[3] << 24 );
    case 3 : a += ( (unsigned)k[2] << 16 );
    case 2 : a += ( (unsigned)k[1] << 8 );
    case 1 : a += k[0];
    }
 
    mix ( a, b, c );
 
    return c;
}
